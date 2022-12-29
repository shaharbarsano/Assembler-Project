/*
 * Responsible for all existing commands
 */

#include <string.h>
#include <stdio.h>
#include "commands.h"
#include "builders.h"

cmdPtr *initCmds() {
    cmdPtr *commands = calloc(MAX_NUM_OF_COMMANDS, sizeof(cmdPtr));

    int i = 0;
    int opValNone[1] = {0};
    int opVal1Lab[1] = {OP_VAL_LABEL};
    int opVal1Str[1] = {OP_VAL_STRING};
    int opValRegOrLab[1] = {OP_VAL_REG_OR_LAB};
    int opValMultiNum[1] = {OP_VAL_NUMBER};
    int opVal2Reg[2] = {OP_VAL_REGISTER, OP_VAL_REGISTER};
    int opVal3Reg[3] = {OP_VAL_REGISTER, OP_VAL_REGISTER, OP_VAL_REGISTER};
    int opVal2Reg1Num[3] = {OP_VAL_REGISTER, OP_VAL_NUMBER, OP_VAL_REGISTER};
    int opVal2Reg1Lab[3] = {OP_VAL_REGISTER, OP_VAL_REGISTER, OP_VAL_LABEL};

    /* INS type R */
    commands[i++] = createCmdPtr("add", 1, OPCODE_ADD, 'R', OP_NUM_THREE, opVal3Reg, &buildRArithmetic);
    commands[i++] = createCmdPtr("sub", 2, OPCODE_SUB, 'R', OP_NUM_THREE, opVal3Reg, &buildRArithmetic);
    commands[i++] = createCmdPtr("and", 3, OPCODE_AND, 'R', OP_NUM_THREE, opVal3Reg, &buildRArithmetic);
    commands[i++] = createCmdPtr("or", 4, OPCODE_OR, 'R', OP_NUM_THREE, opVal3Reg, &buildRArithmetic);
    commands[i++] = createCmdPtr("nor", 5, OPCODE_NOR, 'R', OP_NUM_THREE, opVal3Reg, &buildRArithmetic);
    commands[i++] = createCmdPtr("move", 1, OPCODE_MOVE, 'R', OP_NUM_TWO, opVal2Reg, &buildRCopy);
    commands[i++] = createCmdPtr("mvhi", 2, OPCODE_MVHI, 'R', OP_NUM_TWO, opVal2Reg, &buildRCopy);
    commands[i++] = createCmdPtr("mvlo", 3, OPCODE_MVLO, 'R', OP_NUM_TWO, opVal2Reg, &buildRCopy);
    /* INS type I */
    commands[i++] = createCmdPtr("addi", -1, OPCODE_ADDI, 'I', OP_NUM_THREE, opVal2Reg1Num, &buildIArithmeticAndMemory);
    commands[i++] = createCmdPtr("subi", -1, OPCODE_SUBI, 'I', OP_NUM_THREE, opVal2Reg1Num, &buildIArithmeticAndMemory);
    commands[i++] = createCmdPtr("andi", -1, OPCODE_ANDI, 'I', OP_NUM_THREE, opVal2Reg1Num, &buildIArithmeticAndMemory);
    commands[i++] = createCmdPtr("ori", -1, OPCODE_ORI, 'I', OP_NUM_THREE, opVal2Reg1Num, &buildIArithmeticAndMemory);
    commands[i++] = createCmdPtr("nori", -1, OPCODE_NORI, 'I', OP_NUM_THREE, opVal2Reg1Num, &buildIArithmeticAndMemory);
    commands[i++] = createCmdPtr("bne", -1, OPCODE_BNE, 'I', OP_NUM_THREE, opVal2Reg1Lab, &buildIConditional);
    commands[i++] = createCmdPtr("beq", -1, OPCODE_BEQ, 'I', OP_NUM_THREE, opVal2Reg1Lab, &buildIConditional);
    commands[i++] = createCmdPtr("blt", -1, OPCODE_BLT, 'I', OP_NUM_THREE, opVal2Reg1Lab, &buildIConditional);
    commands[i++] = createCmdPtr("bgt", -1, OPCODE_BGT, 'I', OP_NUM_THREE, opVal2Reg1Lab, &buildIConditional);
    commands[i++] = createCmdPtr("lb", -1, OPCODE_LB, 'I', OP_NUM_THREE, opVal2Reg1Num, &buildIArithmeticAndMemory);
    commands[i++] = createCmdPtr("sb", -1, OPCODE_SB, 'I', OP_NUM_THREE, opVal2Reg1Num, &buildIArithmeticAndMemory);
    commands[i++] = createCmdPtr("lw", -1, OPCODE_LW, 'I', OP_NUM_THREE, opVal2Reg1Num, &buildIArithmeticAndMemory);
    commands[i++] = createCmdPtr("sw", -1, OPCODE_SW, 'I', OP_NUM_THREE, opVal2Reg1Num, &buildIArithmeticAndMemory);
    commands[i++] = createCmdPtr("lh", -1, OPCODE_LH, 'I', OP_NUM_THREE, opVal2Reg1Num, &buildIArithmeticAndMemory);
    commands[i++] = createCmdPtr("sh", -1, OPCODE_SH, 'I', OP_NUM_THREE, opVal2Reg1Num, &buildIArithmeticAndMemory);
    /* INS type J */
    commands[i++] = createCmdPtr("jmp", -1, OPCODE_JMP, 'J', OP_NUM_ONE, opValRegOrLab, &buildJCommand);
    commands[i++] = createCmdPtr("la", -1, OPCODE_LA, 'J', OP_NUM_ONE, opVal1Lab, &buildJCommand);
    commands[i++] = createCmdPtr("call", -1, OPCODE_CALL, 'J', OP_NUM_ONE, opVal1Lab, &buildJCommand);
    commands[i++] = createCmdPtr("stop", -1, OPCODE_STOP, 'J', OP_NUM_NONE, opValNone, &buildJCommand);

    /* DIRECTIVE */
    commands[i++] = createCmdPtr(".dh", -1, OPCODE_DH, '\0', OP_NUM_MULTI, opValMultiNum, NULL);
    commands[i++] = createCmdPtr(".dw", -1, OPCODE_DW, '\0', OP_NUM_MULTI, opValMultiNum, NULL);
    commands[i++] = createCmdPtr(".db", -1, OPCODE_DB, '\0', OP_NUM_MULTI, opValMultiNum, NULL);
    commands[i++] = createCmdPtr(".asciz", -1, OPCODE_ASCIZ, '\0', OP_NUM_ONE, opVal1Str, NULL);
    commands[i++] = createCmdPtr(".entry", -1, OPCODE_ENTRY, '\0', OP_NUM_ONE, opVal1Lab, NULL);
    commands[i++] = createCmdPtr(".extern", -1, OPCODE_EXTERN, '\0', OP_NUM_ONE, opVal1Lab, NULL);

    return commands;
}

void freeCmds(cmdPtr *commands) {
    int i;
    for (i = 0; i < MAX_NUM_OF_COMMANDS; i++) {
        if (commands[i]) {
            free(commands[i]);
        }
    }

    free(commands);
}

cmdPtr createCmdPtr(char name[], int funct, int opcode, char cmdInsType, int opNum, int opValidation[], int (*builder)(cmdPtr cmd, dbInstructsPtr insRow,
        dbSymbolsPtr *symbolsHead, char fileName[])) {
    int i;
    cmdPtr ptr = calloc(1, sizeof(cmd));

    if (!ptr) {
        printf("can not allocate memory.");
        exit(EXIT_ABNORMAL);
    }

    /* make sure we have enough space to store command name */
    if (strlen(name) > MAX_CMD_SIZE) {
        printf(PREFIX_ERROR "CMD NAME TOO LONG: %s\n", name);
        exit(EXIT_ABNORMAL);
    }

    strcpy(ptr->name, name);
    ptr->cmdType = name[0] == '.' ? DIR : INS;
    ptr->opNum = opNum;
    ptr->cmdInsType = cmdInsType;
    ptr->builder = builder;
    ptr->opcode = opcode;
    ptr->funct = funct;

    /* initializing array with 0 */
    for (i = 0; i < MAX_NUM_OF_OP; i++) {
        ptr->opValidation[i] = 0;
    }

    if (opNum > 0) {
        for (i = 0; i < opNum; i++) {
            ptr->opValidation[i] = opValidation[i];
        }
    } else if (opNum == -1) {
        ptr->opValidation[0] = opValidation[0];
    }

    return ptr;
}

cmdPtr getCmd(char name[], int skipDot, cmdPtr *commands) {
    int i;
    char *cmdCheckName = calloc(MAX_CMD_SIZE + 3, sizeof(char)), *cmdCheckNameHolder;

    if (!cmdCheckName) {
        printf("can not allocate memory");
        exit(EXIT_ABNORMAL);
    }

    for (i = 0; i < MAX_NUM_OF_COMMANDS; i++) {
        if (commands[i]) {
            strncpy(cmdCheckName, commands[i]->name, MAX_CMD_SIZE);
            cmdCheckName[MAX_CMD_SIZE] = '\0';
            cmdCheckNameHolder = cmdCheckName;

            if (skipDot == 1 && cmdCheckName[0] == '.' && strlen(cmdCheckName) > 1) {
                cmdCheckNameHolder++;
            }

            if (strcmp(cmdCheckNameHolder, name) == 0) {
                free(cmdCheckName);
                return commands[i];
            }
        }
    }

    free(cmdCheckName);
    return NULL;
}

