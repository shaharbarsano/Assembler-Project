/*
// Parse .as files content
*/

#ifndef HSC_PROJECT_PARSER_H
#define HSC_PROJECT_PARSER_H

#include "config.h"
#include "commands.h"

/**
 * Full parse of .as file
 * the entry point for each file entered for the compiler
 *
 * Validates file name
 * Calls for first scan on file content
 * Calls for second scan on file content
 * If no errors were found, tries to export compiled data
 */
int parseFile(char name[], cmdPtr *commands);

/**
 * Helps to parse label when parsing line.
 */
void parseLineLabel(char line[], int *index, parsedLinePtr parsed, cmdPtr *commands);

/**
 * Helps to parse operands when parsing line. non-public use function
 */
void parseLineOperands(char line[], int *index, parsedLinePtr parsed);

/**
 * Will parse the line string into accommodate variables of parsedLine structure.
 * need to free memory for struct operands (use freeParsedLine())
 */
void parseLine(parsedLinePtr parsed, char line[], cmdPtr *commands);

/**
 * Calling this function will free memory that was allocated for the operands in parsedLine struct
 * @param parsed
 * @param freeSelf when 1, will also free self pointer
 */
void freeParsedLine(parsedLinePtr parsed, int freeSelf);

#endif
