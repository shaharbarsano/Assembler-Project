/**
 * Database handler
 * Responsible for data table, symbols table and instructions table
 */
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "structs.h"
#include "db.h"
#include "commands.h"

dbSymbolsPtr getSymbol(char label[], dbSymbolsPtr *symbolsHead) {
    dbSymbolsPtr tmp = *symbolsHead;

    while (tmp != NULL) {
        if (strcmp(label, tmp->symbol) == 0) {
            return tmp;
        }

        tmp = tmp->next;
    }

    return NULL;
}

dbInstructsPtr addInstruction(int lineNumber, dbInstructsPtr *dbInstructsHead, dbInstructsPtr *lastIns,
                              parsedLinePtr parsed, cmdPtr cmd) {
    dbInstructsPtr tmpIns = (dbInstructsPtr) calloc(1, sizeof(dbInstructs));
    if (!tmpIns) {
        printf("memory allocation error");
        exit(EXIT_ABNORMAL);
    }

    if (!(*lastIns)) { /* Set as head node of the linked list */
        tmpIns->address = 100;
        *dbInstructsHead = tmpIns;
    } else {/* Append into the linked list */
        tmpIns->address = (*lastIns)->address + 4;
        (*lastIns)->next = tmpIns;
    }

    (*lastIns) = tmpIns;
    tmpIns->parsed = parsed; /** Parsed current line syntax */
    tmpIns->lineNumber = lineNumber;
    tmpIns->next = NULL;
    tmpIns->cmd = cmd;

    return tmpIns;
}

dbDataPtr addData(int lineNumber, dbDataPtr *dbDataHead, dbDataPtr *lastData, parsedLinePtr parsed, cmdPtr cmd) {
    dbDataPtr tmpData = (dbDataPtr) calloc(1, sizeof(dbData));
    int countBytes = 0;
    if (!tmpData) {
        printf("memory allocation error");
        exit(EXIT_ABNORMAL);
    }

    if (!(*lastData)) { /* Set as head node of the linked list */
        tmpData->address = 0;
        *dbDataHead = tmpData;
    } else {/* Append into the linked list */
        tmpData->address = (*lastData)->address + (*lastData)->countBytes;
        (*lastData)->next = tmpData;
    }

    (*lastData) = tmpData;
    tmpData->parsed = parsed; /** Parsed current line syntax */
    tmpData->next = NULL;
    tmpData->cmd = cmd;

    /* checks how many bytes does it takes */
    switch (cmd->opcode) {
        /* directive type .dh - 2 bytes for each operand */
        case OPCODE_DH:
            countBytes = (parsed->opNum) * 2;
            break;
            /* directive type .dw - 4 bytes for each operand */
        case OPCODE_DW:
            countBytes = (parsed->opNum) * 4;
            break;
            /* directive type .db - 1 byte for each operand */
        case OPCODE_DB:
            countBytes = parsed->opNum;
            break;
            /* directive type .asciz - 1 byte for each character - 2 (quotation) + 1 null termination ('\0') */
        case OPCODE_ASCIZ:
            countBytes = strlen(parsed->operands[0]) - 2 + 1;
            break;
    }

    tmpData->countBytes = countBytes;

    return tmpData;

}

int addSymbol(int address, int attribute, int lineNumber, dbSymbolsPtr *dbSymbolsHead, dbSymbolsPtr *lastSym,
              char symbolName[], char fileName[]) {
    dbSymbolsPtr tmpSym, existingSym;
    char extra[30] = "";

    /**
     * when 1 - symbolName is coming from regular command label name
     * when 0 - symbolName is coming from extern\entry directive operand
     */
    int isCmdLabel = (attribute == SYM_TYPE_DIR || attribute == SYM_TYPE_INS) ? 1 : 0;

    /**
     * If we have symbol with this name in symbols table
     */
    if (*lastSym != NULL && (existingSym = getSymbol(symbolName, dbSymbolsHead)) != NULL) {
        if (isCmdLabel == 1) { /* Regular command/instruction label name */
            if (existingSym->attributes == SYM_ENTRY) { /* attributes must contain only SYM_ENTRY value */
                /** At this case, the existing symbol is just an entry symbol, so we need to update its address and attribute */
                existingSym->attributes |= attribute;
                existingSym->address = address;
                return 0;
            }
        } else { /* This symbolName is coming from extern\entry directive operand */
            if (attribute == SYM_EXTERNAL && (existingSym->attributes & SYM_EXTERNAL) == SYM_EXTERNAL) {
                /* multiple definition of extern are allowed */
                return 0;
            } else if (attribute == SYM_ENTRY && !(existingSym->attributes & SYM_EXTERNAL)) {
                /* Mark the existing symbol as entry */
                existingSym->attributes |= SYM_ENTRY;
                return 0;
            }
        }

        /** Clarify errors **/
        if (isCmdLabel == 0) {
            if (existingSym->attributes & SYM_EXTERNAL) {
                strcpy(extra, " to be used as external");
            } else if (existingSym->attributes & SYM_ENTRY) {
                strcpy(extra, " to be used as entry");
            }
        }

        printf(
                PREFIX_ERROR "Label \"%s\" (at %s on line %d) already defined%s.\n",
                symbolName,
                fileName,
                lineNumber,
                extra
        );
        return 1;
    }


    tmpSym = (dbSymbolsPtr) calloc(1, sizeof(dbSymbols));
    if (!tmpSym) {
        printf("memory allocation error");
        exit(EXIT_ABNORMAL);
    }

    if (!(*lastSym)) {/* Set as head node of the linked list */
        *dbSymbolsHead = tmpSym;
    } else {/* Append into the linked list */
        (*lastSym)->next = tmpSym;
    }

    (*lastSym) = tmpSym;

    tmpSym->address = address;
    tmpSym->attributes = attribute;
    tmpSym->lineNumber = lineNumber;
    tmpSym->next = NULL;

    strcpy(tmpSym->symbol, symbolName);

    return 0;
}

void freeInstructions(dbInstructsPtr *head) {
    dbInstructsPtr next = *head, tmp = NULL;
    while (next != NULL) {
        tmp = next;
        next = next->next;

        /* Free cmdTypePtr */
        switch (tmp->cmd->cmdInsType) {
            case 'R':
                free(tmp->cmdTypePtr.R);
                break;
                case 'I':
                    free(tmp->cmdTypePtr.I);
                    break;
                    case 'J':
                        free(tmp->cmdTypePtr.J);
                        break;
        }

        freeParsedLine(tmp->parsed, 1);
        free(tmp);
    }
}

void freeDataTable(dbDataPtr *head) {
    dbDataPtr next = *head, tmp = NULL;
    while (next != NULL) {
        tmp = next;
        next = next->next;

        freeParsedLine(tmp->parsed, 1);
        free(tmp);
    }
}

void freeSymbols(dbSymbolsPtr *head) {
    dbSymbolsPtr next = *head, tmp = NULL;
    while (next != NULL) {
        tmp = next;
        next = next->next;
        free(tmp);
    }
}
