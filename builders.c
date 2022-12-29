/*
 * Build result for each command
 */

#include <stdio.h>
#include <limits.h>
#include "config.h"
#include "builders.h"
#include "db.h"

int buildRArithmetic(cmdPtr cmd, dbInstructsPtr insRow, dbSymbolsPtr *symbolsHead, char fileName[]) {
    int rsVal, rtVal, rdVal;

    insRow->cmdTypePtr.R->opcode = cmd->opcode;
    insRow->cmdTypePtr.R->funct = cmd->funct;

    rsVal = atoi(1 + (insRow->parsed->operands[0]));
    rtVal = atoi(1 + (insRow->parsed->operands[1]));
    rdVal = atoi(1 + (insRow->parsed->operands[2]));

    insRow->cmdTypePtr.R->rs = rsVal;
    insRow->cmdTypePtr.R->rt = rtVal;
    insRow->cmdTypePtr.R->rd = rdVal;

    return 0;
}

int buildRCopy(cmdPtr cmd, dbInstructsPtr insRow, dbSymbolsPtr *symbolsHead, char fileName[]) {
    insRow->cmdTypePtr.R->opcode = cmd->opcode;
    insRow->cmdTypePtr.R->funct = cmd->funct;

    insRow->cmdTypePtr.R->rs = atoi(1 + (insRow->parsed->operands[0]));
    insRow->cmdTypePtr.R->rt = 0;
    insRow->cmdTypePtr.R->rd = atoi(1 + (insRow->parsed->operands[1]));

    return 0;
}

int buildIArithmeticAndMemory(cmdPtr cmd, dbInstructsPtr insRow, dbSymbolsPtr *symbolsHead, char fileName[]) {
    int rsVal, rtVal, immedVal;

    insRow->cmdTypePtr.I->opcode = cmd->opcode;

    rsVal = atoi(1 + (insRow->parsed->operands[0]));
    immedVal = atoi(insRow->parsed->operands[1]);
    rtVal = atoi(1 + (insRow->parsed->operands[2]));
    insRow->cmdTypePtr.I->rs = rsVal;
    insRow->cmdTypePtr.I->immed = immedVal;
    insRow->cmdTypePtr.I->rt = rtVal;

    return 0;
}

int buildIConditional(cmdPtr cmd, dbInstructsPtr insRow, dbSymbolsPtr *symbolsHead, char fileName[]) {
    int rsVal, rtVal, immedVal = 0;

    dbSymbolsPtr symbol = getSymbol(insRow->parsed->operands[2], symbolsHead);

    /* if symbol doesn't exists */
    if (!symbol) {
        printf(PREFIX_ERROR "Could not find label \"%s\" at %s on line %d\n", insRow->parsed->operands[2], fileName,
               insRow->lineNumber);
        return 1;
    }

    /* if external label was found */
    if ((symbol->attributes & SYM_EXTERNAL) == SYM_EXTERNAL) {
        printf(PREFIX_ERROR "Invalid label was found at %s on line %d\n", fileName, insRow->lineNumber);
        return 1;
    }

    insRow->cmdTypePtr.I->opcode = cmd->opcode;

    rsVal = atoi(1 + (insRow->parsed->operands[0]));
    rtVal = atoi(1 + (insRow->parsed->operands[1]));

    immedVal = symbol->address - insRow->address;
    if (immedVal <= SHRT_MIN || immedVal >= SHRT_MAX) {
        printf(PREFIX_ERROR "Invalid value for distance destination at %s on line %d\n", fileName, insRow->lineNumber);
        return 1;
    }

    insRow->cmdTypePtr.I->rs = rsVal;
    insRow->cmdTypePtr.I->rt = rtVal;
    insRow->cmdTypePtr.I->immed = immedVal;

    return 0;
}

int buildJCommand(cmdPtr cmd, dbInstructsPtr insRow, dbSymbolsPtr *symbolsHead, char fileName[]) {
    int regVal = 0, addressVal = 0;
    dbSymbolsPtr symbol;

    insRow->cmdTypePtr.J->opcode = cmd->opcode;

    /* for instruction type jmp with register */
    if (cmd->opcode == OPCODE_JMP) {
        if (*(insRow->parsed->operands[0]) == '$') {
            insRow->cmdTypePtr.J->reg = 1;
            insRow->cmdTypePtr.J->address = atoi(1 + (insRow->parsed->operands[0]));
            return 0;
        }
    }

    /* for instruction type stop */
    if (cmd->opcode == OPCODE_STOP) {
        insRow->cmdTypePtr.J->reg = regVal;
        insRow->cmdTypePtr.J->address = addressVal;
        return 0;
    }

    symbol = getSymbol(insRow->parsed->operands[0], symbolsHead);

    if (!symbol) {
        printf(PREFIX_ERROR "Could not find label \"%s\" at %s on line %d\n", insRow->parsed->operands[0], fileName,
               insRow->lineNumber);
        return 1;
    }

    insRow->cmdTypePtr.J->address = symbol->address;
    insRow->cmdTypePtr.J->reg = regVal;

    return 0;
}


unsigned long bundleStructIns(cmdTypePtr ptr, char type) {
    unsigned long result = 0;
    /*
     * For each field we take only the needed bits and shift them to align them perfectly in the result
     */
    switch (type) {
        case 'R':
            /* funct: */
            result |= (ptr.R->funct & 0x1F) << 6; /* take 5 bits and shift by 6 */

            /* rd: */
            result |= (ptr.R->rd & 0x1F) << 11; /* take 5 bits and shift by 11 */

            /* rt: */
            result |= (ptr.R->rt & 0x1F) << 16; /* take 5 bits and shift by 16 */

            /* rs: */
            result |= (ptr.R->rs & 0x1F) << 21; /* take 5 bits and shift by 21 */

            /* opcode: */
            result |= (ptr.R->opcode & 0x3F) << 26; /* take 6 bits and shift by 21 */
            break;
        case 'I':
            /* immed: */
            result |= (ptr.I->immed & 0xFFFF); /* take 16 bits and dont shift */

            /* rt: */
            result |= (ptr.I->rt & 0x1F) << 16; /* take 5 bits and shift by 16 */

            /* rs: */
            result |= (ptr.I->rs & 0x1F) << 21; /* take 5 bits and shift by 21 */

            /* opcode: */
            result |= (ptr.I->opcode & 0x3F) << 26; /* take 6 bits and shift by 21 */
            break;
        case 'J':
            /* address: */
            result |= (ptr.J->address & 0x1FFFFFF); /* take 25 bits and dont shift */

            /* reg: */
            result |= (ptr.J->reg & 0x1) << 25; /* take 1 bit and shift by 25 */

            /* opcode: */
            result |= (ptr.J->opcode & 0x3F) << 26; /* take 6 bits and shift by 21 */
            break;
        default:
            result = 0;
            break;
    }

    return result;
}

int buildDataRowByte(dbDataPtr row, int pos) {
    int eachOp, shiftBy;
    long res;

    if ((pos + 1) > row->countBytes)
        return 0; /* when range exceeded */

    switch (row->cmd->opcode) {
        /**
         * Data types (byte, half and word)
         */
        case OPCODE_DB:
        case OPCODE_DH:
        case OPCODE_DW:
            /* counts how many bytes for each operand */
            eachOp = (row->countBytes) / (row->parsed->opNum);

            /* find operand for requested position */
            res = strtol(row->parsed->operands[(int) (pos / eachOp)], NULL, 10);

            /* find how many bits to shift byte, so we can get the specific requested position byte */
            shiftBy = (pos % eachOp) * 8;

            return (res >> shiftBy) & 0xFF;

            /**
             * ASCIZ strings
             */
        case OPCODE_ASCIZ:
            if ((pos + 1) == row->countBytes)
                return 0; /* for null termination as last char */

            /* skip first quote */
            return (*(1 + pos + (row->parsed->operands[0]))) & 0xFF;
    }

    return 0;
}

