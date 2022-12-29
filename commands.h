/*
 * Responsible for all existing commands
 */

#ifndef HSC_PROJECT_COMMANDS_H
#define HSC_PROJECT_COMMANDS_H

#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "db.h"

/**
 * Function that finds command pointer by cmd name
 * @param name - Command\Directive name
 * @param skipDot - when 1 will skip first dot if exists
 * @return cmdPtr
 */
cmdPtr getCmd(char name[], int skipDot, cmdPtr *commands);

/**
 * Populates all available commands
 */
cmdPtr *initCmds();

/**
 * Free all created commands (with allocated memory)
 * @param commands
 */
void freeCmds(cmdPtr *commands);

/**
 * creates cmdPtr by given parameters
 * @param name Command name
 * @param funct FUNCT command value
 * @param opcode OPCODE command value
 * @param cmdInsType - whether its 'R', 'I', or 'J'
 * @param opNum - number of operands
 * @param opValidation - mask for operands to validate against
 * @param builder - Pointer to builder function
 */
cmdPtr createCmdPtr(char name[], int funct, int opcode, char cmdInsType,  int opNum, int opValidation[], int (*builder)(cmdPtr cmd, dbInstructsPtr insRow, dbSymbolsPtr *symbolsHead, char fileName[]));

#endif

