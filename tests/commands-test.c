/*
 * Test commands functions
 */
#include <stdio.h>
#include "commands-test.h"
#include "tests.h"
#include "../config.h"
#include "../builders.h"

void cmdTest(cmdPtr *commands) {
    dbInstructsPtr dbInstructsHead = NULL, lastIns = NULL;
    dbSymbolsPtr dbSymbolsHead = NULL, lastSym = NULL, tstSym = NULL;

    /* INS type R */
    cmdHelperRTest("customLabel: add $1, $2, $3", 0, 1, 1, 2, 3, commands,
                   &dbInstructsHead, &lastIns, &dbSymbolsHead, &lastSym, __LINE__);

    tstSym = lastSym;

    cmdHelperRTest("add $31, $31, $31", 0, 1, 31, 31, 31, commands,
                   &dbInstructsHead, &lastIns, &dbSymbolsHead, &lastSym, __LINE__);

    cmdHelperRTest("move $21, $2", 1, 1, 21, 0, 2, commands,
                   &dbInstructsHead, &lastIns, &dbSymbolsHead, &lastSym, __LINE__);

    cmdHelperRTest("mvhi $30, $5", 1, 2, 30, 0, 5, commands,
                   &dbInstructsHead, &lastIns, &dbSymbolsHead, &lastSym, __LINE__);

    cmdHelperRTest("mvlo $11, $18", 1, 3, 11, 0, 18, commands,
                   &dbInstructsHead, &lastIns, &dbSymbolsHead, &lastSym, __LINE__);

    /* INS type I */
    cmdHelperITest("sb $21, 45, $3", 20, 45, 21, 3, commands,
                   &dbInstructsHead, &lastIns, &dbSymbolsHead, &lastSym, __LINE__);
    cmdHelperITest("beq $12, $4, customLabel", 16, tstSym->address - (lastIns->address + 4), 12, 4, commands,
                   &dbInstructsHead, &lastIns, &dbSymbolsHead, &lastSym, __LINE__);

    /* INS type J */
    cmdHelperJTest("jmp $12", 30, 1, 12, commands, &dbInstructsHead,
                   &lastIns, &dbSymbolsHead, &lastSym, __LINE__);

    freeInstructions(&dbInstructsHead);
    freeSymbols(&dbSymbolsHead);
}

void cmdHelperRTest(char line[], int opcode, int funct, int rs, int rt, int rd, cmdPtr *commands,
                    dbInstructsPtr *dbInstructsHead, dbInstructsPtr *lastIns, dbSymbolsPtr *dbSymbolsHead,
                    dbSymbolsPtr *lastSym,
                    int lineCounter) {
    cmdPtr cmd;
    parsedLinePtr parsed1;
    dbInstructsPtr row = NULL;

    parsed1 = calloc(1, sizeof(parsedLine));
    parseLine(parsed1, line, commands);
    cmd = getCmd(parsed1->command, 0, commands);
    row = addInstruction(lineCounter, dbInstructsHead, lastIns, parsed1, cmd);
    row->cmdTypePtr.R = calloc(1, sizeof(cmdR));
    (*(cmd->builder))(cmd, row, dbSymbolsHead, __FILE__);

    if ( strcmp(parsed1->label, "") != 0 ) {
        addSymbol(row->address, SYM_TYPE_INS, __LINE__, dbSymbolsHead, lastSym, parsed1->label, __FILE__);
    }

    TST_EQ(row->cmdTypePtr.R->opcode, opcode, "%d");
    TST_EQ(row->cmdTypePtr.R->funct, funct, "%d");
    TST_EQ(row->cmdTypePtr.R->rs, rs, "%d");
    TST_EQ(row->cmdTypePtr.R->rt, rt, "%d");
    TST_EQ(row->cmdTypePtr.R->rd, rd, "%d");
}

void cmdHelperITest(char line[], int opcode, int immed, int rs, int rt, cmdPtr *commands, dbInstructsPtr *dbInstructsHead,
               dbInstructsPtr *lastIns, dbSymbolsPtr *dbSymbolsHead, dbSymbolsPtr *lastSym, int lineCounter) {
    dbInstructsPtr row = NULL;
    cmdPtr cmd;
    parsedLinePtr parsed1;

    parsed1 = calloc(1, sizeof(parsedLine));
    parseLine(parsed1, line, commands);
    cmd = getCmd(parsed1->command, 0, commands);
    row = addInstruction(++lineCounter, dbInstructsHead, lastIns, parsed1, cmd);
    row->cmdTypePtr.I = calloc(1, sizeof(cmdI));

    (*(cmd->builder))(cmd, row, dbSymbolsHead, __FILE__);

    TST_EQ(row->cmdTypePtr.I->opcode, opcode, "%d");
    TST_EQ(row->cmdTypePtr.I->immed, immed, "%d");
    TST_EQ(row->cmdTypePtr.I->rs, rs, "%d");
    TST_EQ(row->cmdTypePtr.I->rt, rt, "%d");
}

void cmdHelperJTest(char line[], int opcode, int reg, long address, cmdPtr *commands, dbInstructsPtr *dbInstructsHead,
                    dbInstructsPtr *lastIns, dbSymbolsPtr *dbSymbolsHead, dbSymbolsPtr *lastSym, int lineCounter) {
    dbInstructsPtr row = NULL;
    cmdPtr cmd;
    parsedLinePtr parsed1;

    parsed1 = calloc(1, sizeof(parsedLine));
    parseLine(parsed1, line, commands);

    cmd = getCmd(parsed1->command, 0, commands);
    row = addInstruction(++lineCounter, dbInstructsHead, lastIns, parsed1, cmd);
    row->cmdTypePtr.J = calloc(1, sizeof(cmdJ));

    (*(cmd->builder))(cmd, row, dbSymbolsHead, __FILE__);

    TST_EQ(row->cmdTypePtr.J->opcode, opcode, "%d");
    TST_EQ(row->cmdTypePtr.J->reg, reg, "%d");
    TST_EQ(row->cmdTypePtr.J->address, address, "%lu");
}
