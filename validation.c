/*
/ Validations functions
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include "validation.h"
#include "config.h"

int firstLineValidate(parsedLinePtr parsed, char fileName[], int line) {
    if (parsed->error == LINE_ERROR_OK)
        return 0;

    printf(PREFIX_ERROR "at %s on line %d (ERR#%d)\n", fileName, line, parsed->error);

    /** Label **/
    if ((parsed->error & LINE_ERROR_LABEL_CMD) == LINE_ERROR_LABEL_CMD) {
        printf(ERROR_LIST_ITEM "Label \"%s\" is not allowed, please choose another name.\n", parsed->label);
    }

    /** Command **/
    if ((parsed->error & LINE_ERROR_CMD) == LINE_ERROR_CMD)
        printf(ERROR_LIST_ITEM "Must enter valid command/instruction.\n");

    /** Operands **/
    if ((parsed->error & LINE_ERROR_OPERANDS) == LINE_ERROR_OPERANDS) {
        printf(ERROR_LIST_ITEM "Operands invalid syntax. make sure no extra redundant commas and no comma at the end of the line. \n");
        printf(ERROR_LIST_SUBITEM "String operand must be opened and closed with quotes (escaping quotes is allowed with '%c' character).\n",
               LINE_STRING_ESCAPE);
    }

    return 1;
}

int cmdOpValidate(parsedLinePtr parsed, char fileName[], int line, cmdPtr cmd) {
    int i, validateWith;

    /* if opNum is not multi so both counters should be equals */
    if (cmd->opNum != parsed->opNum && cmd->opNum != OP_NUM_MULTI) {
        printf(
                PREFIX_ERROR "Invalid amount of operands at %s on line %d. \"%s\" command must have %d operands.\n",
                fileName,
                line,
                cmd->name,
                cmd->opNum
        );
        return 1;
    }

    /* if the command's opNum is multi then the other counter must be at least 1 */
    if (cmd->opNum == OP_NUM_MULTI && parsed->opNum < 1) {
        printf(
                PREFIX_ERROR "Invalid amount of operands at %s on line %d. \"%s\" command must have at least one operand.\n",
                fileName,
                line,
                cmd->name
        );
        return 1;
    }

    /* will try to validate both type and amount of operands */
    for (i = 0; i < parsed->opNum; i++) {
        validateWith = cmd->opValidation[cmd->opNum != OP_NUM_MULTI ? i : 0];

        switch (validateWith) {
            case OP_VAL_REGISTER:
                if (!isValidRegister(parsed->operands[i])) {
                    printf(
                            PREFIX_ERROR "Invalid operand at %s on line %d. Operand number %d must be of type: %s\n",
                            fileName,
                            line,
                            i + 1,
                            "Register"
                    );
                    return 1;
                }
                break;
            case OP_VAL_NUMBER:
                if (!isValidNumber(parsed->operands[i], cmd->opcode)) {
                    printf(
                            PREFIX_ERROR "Invalid number at %s on line %d. Operand number %d must be of type: %s\n",
                            fileName,
                            line,
                            i + 1,
                            "Number"
                    );
                    return 1;
                }
                break;
            case OP_VAL_LABEL:
                if (!isValidLabel(parsed->operands[i])) {
                    printf(
                            PREFIX_ERROR "Invalid label at %s on line %d. Operand number %d must be of type: %s\n",
                            fileName,
                            line,
                            i + 1,
                            "Label"
                    );
                    return 1;
                }
                break;
            case OP_VAL_STRING:
                if (!isValidString(parsed->operands[i])) {
                    printf(
                            PREFIX_ERROR "Invalid String at %s on line %d. Operand number %d must be of type: %s\n",
                            fileName,
                            line,
                            i + 1,
                            "String"
                    );
                    return 1;
                }
                break;
            case OP_VAL_REG_OR_LAB:
                if (!isValidRegister(parsed->operands[i]) && !isValidLabel(parsed->operands[i])) {
                    printf(
                            PREFIX_ERROR "Invalid label or register at %s on line %d. Operand number %d must be of type: %s\n",
                            fileName,
                            line,
                            i + 1,
                            "Label or Register"
                    );
                    return 1;
                }
                break;
        }
    }

    return 0;
}

/**
 * Validates register operand:
 * length (2 or 3 chars)
 * must start with $
 * must have digits afterwards
 * digits must be between 0 and 31
 */
int isValidRegister(char operand[]) {
    unsigned int length = strlen(operand);
    int regVal = 0;

    if (
            (length != 2 && length != 3) ||
            operand[0] != '$' ||
            !isdigit(operand[1]) ||
            (length == 3 && !isdigit(operand[2]))
            )
        return 0;

    operand++;
    regVal = atoi(operand);

    if (regVal < 0 || regVal > 31)
        return 0;

    return 1;
}

/**
 * Validates Number operand:
 * optional - start with + or -
 * must have digits afterwards
 * must be an integer, size depends on command type
 */
int isValidNumber(char operand[], int opCode) {
    int i;
    unsigned int length = strlen(operand);
    unsigned int leadingZero;
    long numVal;

    /* if there's a sign +/- before the number */
    int sign = (operand[0] == '+' || operand[0] == '-' ? 1 : 0);

    /* must be digits after sign */
    if (sign && length == 1)
        return 0;

    /* checks all characters are digits */
    for (i = sign; i < length; i++) {
        if (!isdigit(operand[i]))
            return 0;
    }

    /* skips sign and subtracts from length */
    if (sign) {
        sign = (operand[0] == '+' ? 1 : -1);
        operand++;
        length--;
    }

    /* counts leading zeros */
    leadingZero = strspn(operand, "0");

    /* if there's only zeros */
    if (leadingZero == length) {
        length = 1;
        leadingZero = 0;
    }

    /* skips leading zeros and subtracts from length */
    for (i = leadingZero; i > 0; i--) {
        operand++;
        length--;
    }

    /* must be at most 10 digits in int */
    if (length > 10)
        return 0;

    numVal = (!sign ? atol(operand) : sign * atol(operand));

    /* checks for sized integer overflow */
    /* size of 32 bits - for commands type addi, subi, andi, ori, nori (op 10-14) and directive type .dw (op 71) */
    if ((opCode >= OPCODE_ADDI && opCode <= OPCODE_NORI) || (opCode == OPCODE_DW)) {
        if (numVal > INT_MAX || numVal < INT_MIN)
            return 0;
    }

    /* size of 16 bits - for commands type lb, sb, lw, lh, sh (op 19-24) and directive type .dh (op 70) */
    if ((opCode >= OPCODE_LB && opCode <= OPCODE_SH) || (opCode == OPCODE_DH)) {
        if (numVal > SHRT_MAX || numVal < SHRT_MIN)
            return 0;
    }

    /* size of 8 bits - for directive type .db (op 72) */
    if (opCode == OPCODE_DB) {
        if (numVal > SCHAR_MAX || numVal < SCHAR_MIN)
            return 0;
    }

    return 1;
}

/**
 * Validates String operand:
 * all characters must be printable and inside quotation
 */
int isValidString(char operand[]) {
    int i;
    unsigned int length = strlen(operand);

    /* must stats and ends with quotation, at least one character between */
    if (
            (length <= 2) ||
            (operand[0] != '"') ||
            (operand[length - 1] != '"')
            )
        return 0;

    /* all characters must be printable */
    for (i = 1; i < length - 1; i++) {
        if (!isprint(operand[i]))
            return 0;
    }

    return 1;
}

/**
 * Validates Label operand:
 * maximum size - 31
 * must start with letter
 * other characters must be letters or digits
 */
int isValidLabel(char operand[]) {
    int i;
    unsigned int length = strlen(operand);

    if (
            (length > MAX_LABEL_SIZE) ||
            (!isalpha(operand[0]))
            )
        return 0;

    for (i = 1; i < length; i++) {
        if ((!isalpha(operand[i])) && (!isdigit(operand[i])))
            return 0;
    }

    return 1;
}
