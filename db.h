/**
 * Database handler
 * Responsible for insert / read of the data table, symbols table and instructions table
 */

#ifndef HSC_PROJECT_DB_H
#define HSC_PROJECT_DB_H

#include "config.h"
#include "structs.h"
#include "parser.h"
#include "commands.h"

/**
 * Get label by name
 * @param label - label name
 * @param symbolsHead  - head node of symbols table
 * @return NULL on failure or dbSymbolsPtr on success
 */
dbSymbolsPtr getSymbol(char label[], dbSymbolsPtr *symbolsHead);

/**
 * Add instruction into instructions table
 * @param lineNumber - line number
 * @param dbInstructsHead - Head node of the linked list
 * @param lastIns - last inserted node of the list
 * @param parsed - parsed line syntax
 * @param cmdPtr - pointer to found command
 * @return pointer to the new created row at instructions table
 */
dbInstructsPtr addInstruction(int lineNumber, dbInstructsPtr *dbInstructsHead, dbInstructsPtr *lastIns,
                               parsedLinePtr parsed, cmdPtr cmd);

/**
 * Add directives into data table
 * @param lineNumber - line number
 * @param dbDataHead - Head node of the linked list
 * @param lastData - last inserted node of the list
 * @param parsed - parsed line syntax
 * @param cmdPtr - pointer to found command
 * @return pointer to the new created row at data table
 */
dbDataPtr addData(int lineNumber, dbDataPtr *dbDataHead, dbDataPtr *lastData, parsedLinePtr parsed, cmdPtr cmd);

/**
 * Add symbols into symbosl table
 * @param address - address
 * @param attribute - one attribute
 * @param lineNumber
 * @param dbSymbolsHead - Head node of the symbols linked list
 * @param lastSym - last inserted symbol
 * @param symbolName - symbol name to add
 * @param fileName - filename
 * @return 1 on error, 0 on success
 */
int addSymbol(int address, int attribute, int lineNumber, dbSymbolsPtr *dbSymbolsHead, dbSymbolsPtr *lastSym,
              char symbolName[], char fileName[]);

/**
 * Free head node of Instructions Table
 * @param head
 */
void freeInstructions(dbInstructsPtr *head);

/**
 * Free head node of Data Table
 * @param head
 */
void freeDataTable(dbDataPtr *head);

/**
 * Free head node of Symbols Table
 * @param head
 */
void freeSymbols(dbSymbolsPtr *head);

#endif
