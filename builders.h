/*
 * Build result for each command
 */

#include "db.h"

#ifndef HSC_PROJECT_BUILDERS_H
#define HSC_PROJECT_BUILDERS_H

/**
 * Building for add, sub, and, or, nor
 * @return 1 on failure, 0 on success
 */
int buildRArithmetic(cmdPtr cmd, dbInstructsPtr insRow, dbSymbolsPtr *symbolsHead, char fileName[]);

/**
 * Building for move, mvhi, mvlo
 * @return 1 on failure, 0 on success
 */
int buildRCopy(cmdPtr cmd, dbInstructsPtr insRow, dbSymbolsPtr *symbolsHead, char fileName[]);

/**
 * Building for addi, subi, andi, ori, nori, lb, sb, lw, sw, lh, sh
 * @return 1 on failure, 0 on success
 */
int buildIArithmeticAndMemory(cmdPtr cmd, dbInstructsPtr insRow, dbSymbolsPtr *symbolsHead, char fileName[]);

/**
 * Building for beq, bne, blt, bgt
 * @return 1 on failure, 0 on success
 */
int buildIConditional(cmdPtr cmd, dbInstructsPtr insRow, dbSymbolsPtr *symbolsHead, char fileName[]);

/**
 * Building for jmp, la, call, stop
 * @return 1 on failure, 0 on success
 */
int buildJCommand(cmdPtr cmd, dbInstructsPtr insRow, dbSymbolsPtr *symbolsHead, char fileName[]);

/**
 * Get specific byte from data row
 * @param row Data row pointer
 * @param pos Index position of the byte
 * @return byte as int
 */
int buildDataRowByte(dbDataPtr row, int pos);

/**
 * Receives cmdTypePtr structure and converts its fields into one data number.
 * @param ptr Cmd Type Pointer
 * @param type Cmd Type, 'R', 'I' or 'J'
 * @return number which is bundle of all structure fields
 */
unsigned long bundleStructIns(cmdTypePtr ptr, char type);

#endif

