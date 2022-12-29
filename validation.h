/*
// First-scan Validations functions
*/
#ifndef HSC_PROJECT_VALIDATION_H
#define HSC_PROJECT_VALIDATION_H

#include "parser.h"
#include "db.h"
#include "commands.h"


/**
 * Validate first scan line
 * @return 1 if was error, 0 if no error
 */
int firstLineValidate(parsedLinePtr parsed, char fileName[], int line);

/**
 * Validate command operands after parsing
 * @return 1 - for error, 0 - none
 */
int cmdOpValidate(parsedLinePtr parsed, char fileName[], int line, cmdPtr cmd);

/**
 * validates operand type register
 * @param operand
 * @return 1 when valid register string, 0 otherwise
 */
int isValidRegister(char operand[]);

/**
 * validates operand type number
 * @param operand
 * @return 1 when valid number, 0 otherwise
 */
int isValidNumber(char operand[], int opCode);

/**
 * validates operand type string
 * @param operand
 * @return 1 when valid string, 0 otherwise
 */
int isValidString(char operand[]);

/**
 * validates operand type Label
 * @param operand
 * @return 1 when valid label, 0 otherwise
 */
int isValidLabel(char operand[]);

#endif
