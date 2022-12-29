/*
Parse .as files content
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "config.h"
#include "parser.h"
#include "scanners.h"
#include "db.h"
#include "commands.h"
#include "output.h"

int parseFile(char name[], cmdPtr *commands) {
    FILE *file;
    char *fullName = (char *) calloc(strlen(name) + 1, sizeof(char));
    char *ext = strrchr(name, '.');
    int hasErrors = 0, totalIns = 0, totalData = 0;
    dbInstructsPtr dbInstructsHead = NULL;
    dbDataPtr dbDataHead = NULL;
    dbSymbolsPtr dbSymbolsHead = NULL;

    if (!fullName) {
        printf("can not allocate memory.\n");
        return EXIT_ABNORMAL;
    }

    strcpy(fullName, name);

    /* Validate filename ends with ".as" */
    if (!ext || strcmp(ext, INPUT_FILE_EXT) != 0) {
        printf(PREFIX_ERROR "'%s' is not valid filename. filename must end with '.as'\n", name);
        free(fullName);
        return EXIT_ABNORMAL;
    }

    ext++; /* skip the dot */
    *(ext - 1) = '\0'; /* this will remove the extension name from name var */

    /* Validate that filename without extension is not empty */
    if (!name || strcmp(name, "") == 0) {
        printf(PREFIX_ERROR "'.%s' is not valid filename. filename must not be empty before '.as'\n", ext);
        free(fullName);
        return EXIT_ABNORMAL;
    }

    file = fopen(fullName, "r");
    if (!file) {
        printf(PREFIX_ERROR "can not read file: '%s'\n", fullName);
        free(fullName);
        return EXIT_ABNORMAL;
    }

    /**
     * Scan the file
     */
    firstScan(file, fullName, &hasErrors, &dbInstructsHead, &dbDataHead, &dbSymbolsHead, commands, &totalIns, &totalData);
    secondScan(fullName, &hasErrors, &dbInstructsHead, &dbDataHead, &dbSymbolsHead, &totalIns);

    /**
     * If no errors during scanning, create output files
     * */
    if(hasErrors == 0) {
        outputExport(&dbInstructsHead, &dbDataHead, name, totalIns, totalData, dbSymbolsHead);
    }

    free(fullName);
    fclose(file);

    /** Free rows **/
    freeInstructions(&dbInstructsHead);
    freeDataTable(&dbDataHead);
    freeSymbols(&dbSymbolsHead);

    return EXIT_OK;
}

void parseLine(parsedLinePtr parsed, char line[], cmdPtr *commands) {
    unsigned int lineLength = strlen(line);
    int i = 0, j = 0;

    /**
     * Initial values
     */
    parsed->error = LINE_ERROR_OK;
    parsed->label[0] = '\0';
    parsed->command[0] = '\0';
    parsed->ignore = 0;
    parsed->opNum = 0;

    while (i < lineLength && isspace(line[i])) i++; /* increase i to skip whitespace at start */

    /**
     * Comment or empty line
     */
    if (i >= lineLength || line[i] == LINE_COMMENT_START) {
        parsed->ignore = 1;
        return;
    }

    /**
     * Parse label
     */
    parseLineLabel(line, &i, parsed, commands);

    /**
     * Parse command
     */
    for (j = 0; i < lineLength && j < MAX_CMD_SIZE; j++, i++) {
        if (isspace(line[i])) break;

        parsed->command[j] = line[i];
        parsed->command[j + 1] = '\0';
    }

    /* Must have whitespace after the command */
    if (!parsed->ignore && i < lineLength && !isspace(line[i])) {
        parsed->error |= LINE_ERROR_CMD;
    }

    /* err for command? */
    if (!parsed->ignore && strcmp(parsed->command, "") == 0) {
        parsed->error |= LINE_ERROR_CMD;
    }

    /**
     * Parse operands
     */
    parseLineOperands(line, &i, parsed);
}

void freeParsedLine(parsedLinePtr parsed, int freeSelf) {
    int i;
    for (i = 0; i < parsed->opNum; i++) {
        free(parsed->operands[i]);
    }

    if (freeSelf)
        free(parsed);
}

void parseLineOperands(char line[], int *index, parsedLinePtr parsed) {
    unsigned int lineLength = strlen(line);
    int i = *index, j = 0, k;
    int insideQuotes = 0;
    char tmp[MAX_LINE_SIZE + 1] = "";

    while (i < lineLength && isspace(line[i])) i++; /* trim spaces */

    if (i == lineLength) return;

    while (i <= lineLength) {
        /*
         * If not inside quotes and meet operand end separator.
         * or if end of line
         */
        if ((!insideQuotes && line[i] == LINE_OPERAND_SEP) || i == lineLength) {
            k = j - 1;
            while (k > 0 && isspace(tmp[k])) k--; /* try to trim spaces at the end */
            tmp[k + 1] = '\0';

            if (k < 0 || insideQuotes) {
                /* Empty part or Inside quote (string didnt end)... raise error */
                parsed->error |= LINE_ERROR_OPERANDS;
            } else {
                /* Insert into parsedLinePtr */
                parsed->operands[parsed->opNum] = calloc(k + 2, sizeof(char));
                if (!parsed->operands[parsed->opNum]) {
                    printf("failed to allocate memory [operands]");
                    exit(EXIT_ABNORMAL);
                }
                strcpy(parsed->operands[parsed->opNum], tmp);
                parsed->opNum++;
            }

            /* RESET: */
            j = 0;
            tmp[0] = '\0';
            i++;

            while (i < lineLength && isspace(line[i])) i++; /* trim spaces */
            continue;
        }

        if (line[i] == '"') { /* found quotes sign */
            if (insideQuotes && line[i - 1] == LINE_STRING_ESCAPE) {
                /* we found quotes inside the string which are escaped. */
                j--; /* override the last char which was the escape char */

                if (line[i - 2] == LINE_STRING_ESCAPE) {
                    /* if [i - 2] char was also escape char,
                     * we can count this quotes as closing quotes, because there was a double escape
                     * remember that we are currently inside opened string */
                    insideQuotes = 0;
                }
            } else {
                insideQuotes = insideQuotes ? 0 : 1;
            }
        }

        tmp[j++] = line[i++];
        tmp[j] = '\0';
    }

    *index = i;
}

void parseLineLabel(char line[], int *index, parsedLinePtr parsed, cmdPtr *commands) {
    unsigned int lineLength = strlen(line);
    int k, j, i = *index;

    /**
     * Parse label
     */
    if (isalpha(line[i])) { /* label must start with letter */
        k = i;
        /**
         * Run forward and make sure not more than max label size
         */
        for (j = 0; i < lineLength && j <= MAX_LABEL_SIZE; i++, j++) {
            if (isalpha(line[i]) || isdigit(line[i]) || line[i] == LINE_LABEL_END) {
                parsed->label[j] = line[i];
                parsed->label[j + 1] = '\0';
                if (line[i] == LINE_LABEL_END) {
                    break;
                }
            } else {
                break;
            }
        }

        /* if parsed->label doesnt ends with LINE_LABEL_END its a invalid label */
        if (parsed->label[j] != LINE_LABEL_END) {
            parsed->label[0] = '\0';
            i = k; /* reset i so we try to read it as command */
        } else {
            parsed->label[j] = '\0'; /* remove the end symbol */

            if (getCmd(parsed->label, 1, commands) != NULL) {
                /* Label equals to one of our commands names, that's not valid. */
                parsed->error |= LINE_ERROR_LABEL_CMD;
            }

            if (line[i] == LINE_LABEL_END) /* skip label end symbol */
                i++;

            while (i < lineLength && isspace(line[i])) i++; /* remove whitespaces */
        }
    }

    *index = i;
}
