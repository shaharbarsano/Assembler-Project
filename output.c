/**
 * Responsible for creating output files
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "config.h"
#include "structs.h"
#include "output.h"
#include "builders.h"

int outputExport(dbInstructsPtr *dbInstructsHead, dbDataPtr *dbDataHead, char name[], int totalIns, int totalData,
                 dbSymbolsPtr symbolsHead) {
    int res = 0, tmp, hasEnt = 0, hasExt = 0;
    char sep[] = "====================================";
    dbSymbolsPtr symNode = symbolsHead;

    /**
     * Detect if we have externals \ entries files
     */
    while (symNode != NULL) {
        hasEnt |= (symNode->attributes & SYM_ENTRY) ? 1 : 0;
        hasExt |= (symNode->attributes & SYM_EXTERNAL) ? 1 : 0;
        symNode = symNode->next;
    }

    printf("%s\nCreating output files for \"" COLOR_BLUE "%s%s" COLOR_RESET "\":\n", sep, name, INPUT_FILE_EXT);

    /** OUTPUT OBJECT **/
    tmp = outputExportObject(dbInstructsHead, dbDataHead, name, totalIns, totalData);
    printf(COLOR_BLUE "%15s" COLOR_RESET " %s (%s.%s)\n", "OBJECT", FILE_BUILD_STATUS(tmp), name, OUTPUT_EXT_OBJECT);
    res |= tmp;

    /** OUTPUT ENTRIES **/
    if (hasEnt) {
        tmp = outputExportEntries(symbolsHead, name);
        printf(COLOR_BLUE "%15s" COLOR_RESET " %s (%s.%s)\n", "ENTRIES", FILE_BUILD_STATUS(tmp), name,
               OUTPUT_EXT_ENTRIES);
        res |= tmp;
    }

    /** OUTPUT EXTERNALS **/
    if (hasExt) {
        tmp = outputExportExternals(symbolsHead, dbInstructsHead, name);
        printf(COLOR_BLUE "%15s" COLOR_RESET " %s (%s.%s)\n", "EXTERNALS", FILE_BUILD_STATUS(tmp), name,
               OUTPUT_EXT_EXTERNALS);
        res |= tmp;
    }

    printf("%s\n", sep);

    return res;
}

int
outputExportObject(dbInstructsPtr *dbInstructsHead, dbDataPtr *dbDataHead, char name[], int totalIns, int totalData) {
    FILE *fp;
    char *fileName, line[OUTPUT_MAX_LINE_LENGTH + 1], buffer[60];
    int i, j, tmpAddData;
    dbInstructsPtr insLine;
    dbDataPtr dataLine;

    /** The name for the file to create: */
    fileName = calloc(strlen(name) + strlen(OUTPUT_EXT_OBJECT) + 4, sizeof(char));
    sprintf(fileName, "%s.%s", name, OUTPUT_EXT_OBJECT);

    /** Create the file for writing */
    fp = fopen(fileName, "w");

    if (fp == NULL) {
        free(fileName);
        return 1;
    }

    /** File title (add counters for how much bytes in each table) */
    sprintf(buffer, "%7d %d", totalIns, totalData);
    for (i = 0; i < strlen(buffer); i++)
        putc(buffer[i], fp);

    /** Loop on instructions rows */
    insLine = *dbInstructsHead;
    while (insLine != NULL) {
        putc('\n', fp);
        outputLineIns(line, insLine->address, bundleStructIns(insLine->cmdTypePtr, insLine->cmd->cmdInsType));

        /* Add each char to file */
        i = 0;
        while (line[i] != '\0' && line[i] > 0) {
            putc(line[i], fp);
            i++;
        }

        insLine = insLine->next;
    }

    /** Loop on data rows */
    j = 0;
    dataLine = *dbDataHead;
    if (dataLine != NULL)
        tmpAddData = dataLine->address;

    while (dataLine != NULL) {
        for (i = 0; i < dataLine->countBytes; i++) {
            /* start new line with address every 4 bytes */
            if (j == 0) {
                putc('\n', fp);
                fprintf(fp, "%04d", tmpAddData);
                tmpAddData += 4;
            }

            /* print to file value in hex */
            fprintf(fp, " %02X", buildDataRowByte(dataLine, i));

            if (++j == 4) {
                /* update j to start new line */
                j = 0;
            }
        }

        dataLine = dataLine->next;
    }

    fclose(fp);
    free(fileName);

    return 0;
}

int outputExportEntries(dbSymbolsPtr symbolsHead, char name[]) {
    FILE *fp;
    char *fileName;
    dbSymbolsPtr symPtr = symbolsHead;
    int i = 0;

    /** The name for the file to create: */
    fileName = calloc(strlen(name) + strlen(OUTPUT_EXT_ENTRIES) + 4, sizeof(char));
    sprintf(fileName, "%s.%s", name, OUTPUT_EXT_ENTRIES);

    /** Create the file for writing */
    fp = fopen(fileName, "w");

    if (fp == NULL) {
        free(fileName);
        return 1;
    }

    while (symPtr != NULL) {
        if ((symPtr->attributes & SYM_ENTRY) == SYM_ENTRY) {
            if (i++ > 0) {
                putc('\n', fp);
            }

            fprintf(fp, "%s %04d", symPtr->symbol, symPtr->address);
        }
        symPtr = symPtr->next;
    }

    fclose(fp);
    free(fileName);

    return 0;
}

int outputExportExternals(dbSymbolsPtr symbolsHead, dbInstructsPtr *dbInstructsHead, char name[]) {
    FILE *fp;
    char *fileName;
    dbSymbolsPtr symPtr = symbolsHead;
    dbInstructsPtr insPtr;
    int i = 0, j = 0;

    /** The name for the file to create: */
    fileName = calloc(strlen(name) + strlen(OUTPUT_EXT_EXTERNALS) + 4, sizeof(char));
    sprintf(fileName, "%s.%s", name, OUTPUT_EXT_EXTERNALS);

    /** Create the file for writing */
    fp = fopen(fileName, "w");

    if (fp == NULL) {
        free(fileName);
        return 1;
    }

    while (symPtr != NULL) {
        if ((symPtr->attributes & SYM_EXTERNAL) == SYM_EXTERNAL) {
            /**
             * loop through instructions to find usages
             */
            insPtr = *dbInstructsHead;
            while (insPtr != NULL) {
                if ((insPtr->cmd->cmdInsType == 'J') &&
                    insPtr->parsed->opNum >= 1 &&
                    (strcmp(insPtr->parsed->operands[0], symPtr->symbol) == 0)) {
                    if (i++ != 0 || j++ != 0) {
                        putc('\n', fp);
                    }

                    fprintf(fp, "%s %04d", symPtr->symbol, insPtr->address);
                }
                insPtr = insPtr->next;
            }
        }
        symPtr = symPtr->next;
    }

    fclose(fp);
    free(fileName);

    return 0;
}

void outputLineIns(char *str, int decAddress, unsigned long data) {
    char tmpStr[10];
    int tmpNum, i, j = 0;

    /* RESET:*/
    str[0] = '\0';

    /**
     * Prefix with decimal address value (if provided...)
     */
    if (decAddress >= 0) {
        sprintf(tmpStr, "%04d ", decAddress);
        for (i = 0; i < strlen(tmpStr); i++)
            str[j++] = tmpStr[i];

        str[j] = '\0';
    }

    /**
     * Loop to run 4 bytes
     */
    for (i = 0; i < 4; i++) {
        /*
         * Save one byte into tmpNum
         * and shift data by one byte for next loop iteration
         */
        tmpNum = data & 0xFF;
        data = data >> 8;

        /*
         * Convert tmpNum into hex
         */
        sprintf(tmpStr, "%02X", tmpNum);

        /*
         * Save result
         */
        str[j++] = tmpStr[0];
        str[j++] = tmpStr[1];

        if (i != 3)
            str[j++] = ' ';

        str[j] = '\0';
    }
}
