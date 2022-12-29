/*
 * Test commands functions
 */
#ifndef HSC_PROJECT_COMMANDS_TEST_H
#define HSC_PROJECT_COMMANDS_TEST_H

#include "../commands.h"

/**
 * Test for R commands
 */
void cmdHelperRTest(char line[], int opcode, int funct, int rs, int rt, int rd, cmdPtr *commands,
                    dbInstructsPtr *dbInstructsHead, dbInstructsPtr *lastIns, dbSymbolsPtr *dbSymbolsHead,
                    dbSymbolsPtr *lastSym, int lineCounter);

/**
 * Test for I commands
 */
void
cmdHelperITest(char line[], int opcode, int immed, int rs, int rt, cmdPtr *commands, dbInstructsPtr *dbInstructsHead,
               dbInstructsPtr *lastIns, dbSymbolsPtr *dbSymbolsHead, dbSymbolsPtr *lastSym, int lineCounter);

/**
 * Test for J commands
 */
void cmdHelperJTest(char line[], int opcode, int reg, long address, cmdPtr *commands, dbInstructsPtr *dbInstructsHead,
                    dbInstructsPtr *lastIns, dbSymbolsPtr *dbSymbolsHead, dbSymbolsPtr *lastSym, int lineCounter);

/**
 * Test commands building
 * @param commands
 */
void cmdTest(cmdPtr *commands);

#endif
