/*
 * First scan of .as file
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "scanners.h"
#include "config.h"
#include "parser.h"
#include "db.h"
#include "commands.h"
#include "validation.h"

void firstScan(FILE *file, char fileName[], int *hasErrors, dbInstructsPtr *dbInstructsHead, dbDataPtr *dbDataHead,
               dbSymbolsPtr *dbSymbolsHead,
               cmdPtr *commands, int *totalIns, int *totalData) {
    char line[MAX_LINE_SIZE];
    int lineNumber = 0, lineErrors, skipSymbolAdd;
    dbInstructsPtr tmpIns, lastIns = NULL;
    dbSymbolsPtr lastSym = NULL;
    parsedLinePtr parsed;
    cmdPtr cmd;
    dbDataPtr tmpData, lastData = NULL;

    while (fgets(line, sizeof(line), file)) {
        ++lineNumber;
        skipSymbolAdd = 0;

        /**
         * Parse line
         */
        parsed = calloc(1, sizeof(parsedLine));
        parseLine(parsed, line, commands);

        /**
         * Detect if we have any errors so we will know to not make final compilation
         */
        lineErrors = firstLineValidate(parsed, fileName, lineNumber);
        *hasErrors |= lineErrors;

        /**
         * Skip this line if we had errors during the parse
         * OR if this line is an empty line or comment line
         */
        if (lineErrors || parsed->ignore) {
            freeParsedLine(parsed, 1);
            continue;
        }

        /**
         * Make sure this command exists
         */
        if (!(cmd = getCmd(parsed->command, 0, commands))) {
            *hasErrors |= 1;
            freeParsedLine(parsed, 1);
            printf(
                    PREFIX_ERROR "Command \"%s\" does not exists, at %s on line %d.\n",
                    parsed->command,
                    fileName,
                    lineNumber
            );
            continue;
        }

        /**
         * checks if the operands fits the command structure
         */
        lineErrors = 0;
        lineErrors = cmdOpValidate(parsed, fileName, lineNumber, cmd);
        *hasErrors |= lineErrors;

        if (lineErrors) {
            /* Syntax error for this line, skip this line */
            freeParsedLine(parsed, 1);
            continue;
        }

        /**
         * Set Pointer to command type
         */
        if (cmd->cmdType == INS) {
            /**
             * Create node in Instructions Table
             */
            tmpIns = addInstruction(lineNumber, dbInstructsHead, &lastIns, parsed, cmd);
            (*totalIns)++;

            switch (cmd->cmdInsType) {
                case 'R':
                    tmpIns->cmdTypePtr.R = calloc(1, sizeof(cmdR));
                    break;
                case 'I':
                    tmpIns->cmdTypePtr.I = calloc(1, sizeof(cmdI));
                    break;
                case 'J':
                    tmpIns->cmdTypePtr.J = calloc(1, sizeof(cmdJ));
                    break;
            }
        } else {
            /**
             * if directive is not .entry or .extern, then create node in Data Table
             */
            if (cmd->opcode != OPCODE_ENTRY && cmd->opcode != OPCODE_EXTERN) {
                tmpData = addData(lineNumber, dbDataHead, &lastData, parsed, cmd);
                (*totalData) += tmpData->countBytes;
            } else {
                skipSymbolAdd = 1;

                /** Show warning because having label on extern\entry directive is redundant */
                if (strcmp(parsed->label, "") != 0 && (cmd->opcode == OPCODE_ENTRY || cmd->opcode == OPCODE_EXTERN)) {
                    printf(
                            PREFIX_WARNING "Unnecessary label defined at %s on line %d.\n",
                            fileName,
                            lineNumber
                    );
                }

                /** add entry or extern value into symbols table */
                *hasErrors |= addSymbol(
                        ADDRESS_UNDEFINED,
                        (cmd->opcode == OPCODE_ENTRY ? SYM_ENTRY : SYM_EXTERNAL),
                        lineNumber,
                        dbSymbolsHead,
                        &lastSym,
                        parsed->operands[0],
                        fileName
                );

                freeParsedLine(parsed, 1);
            }
        }

        /**
         * Save label of this row into symbols table
         */
        if (skipSymbolAdd == 0 && strcmp(parsed->label, "") != 0) {
            *hasErrors |= addSymbol(
                    cmd->cmdType == INS ? tmpIns->address : tmpData->address,
                    cmd->cmdType == INS ? SYM_TYPE_INS : SYM_TYPE_DIR,
                    lineNumber,
                    dbSymbolsHead,
                    &lastSym,
                    parsed->label,
                    fileName
            );
        }
    }
}

void secondScan(char fileName[], int *hasErrors, dbInstructsPtr *dbInstructsHead, dbDataPtr *dbDataHead,
                dbSymbolsPtr *dbSymbolsHead, int *totalIns) {
    dbInstructsPtr insRow;
    dbSymbolsPtr tmpSym;
    dbDataPtr tmpData;

    /** Update total instruction */
    (*totalIns) *= 4;

    /**
     * Update addresses on data table only if there's already an instruction
     */
    tmpData = *dbDataHead;
    if ((*totalIns)) {
        while (tmpData != NULL) {
            tmpData->address += (*totalIns) + 100;
            tmpData = tmpData->next;
        }
    }

    /**
     * Check if we have entry without associated address
     * also update address of symbols type data
     */
    tmpSym = *dbSymbolsHead;
    while (tmpSym != NULL) {
        if (tmpSym->attributes == SYM_ENTRY) {
            /* Those attributes contain only ENTRY attribute */
            printf(PREFIX_ERROR "Entry declared for \"%s\" but no label has been found with that name at %s on line %d.\n",
                   tmpSym->symbol, fileName, tmpSym->lineNumber);
            *hasErrors = 1;
        }

        /* update address only if there's already an instruction */
        if (
                (*totalIns) && (tmpSym->attributes & SYM_TYPE_DIR) == SYM_TYPE_DIR
                ) {
            tmpSym->address += (*totalIns) + 100;
        }

        tmpSym = tmpSym->next;
    }

    /**
     * Run through all instructs and try to build their data
     */
    insRow = *dbInstructsHead;
    while (insRow != NULL) {
        /**
         * try to build:
         */
        *hasErrors |= (*insRow->cmd->builder)(insRow->cmd, insRow, dbSymbolsHead, fileName);

        insRow = insRow->next;
    }

    /*** RUN on directives is made on the output module because there is nothing to build, only to output. */
}
