/*
 * Responsible for scanning .as files
 * both first scan and second scan
 */

#ifndef HSC_PROJECT_SCANNERS_H
#define HSC_PROJECT_SCANNERS_H

#include "parser.h"
#include "db.h"

/**
  * First scan of .as file
  * Running on each line of the file
  * parsing line syntax intro accommodate structure
  * trying to find command (instruction or directive)
  * adding symbols if exists
  *
  * @param file - Pointer to opened file stream
  * @param fileName - File name
  * @param hasErrors - To tell if we had errors while compiling
  * @param dbInstructsHead - Pointer to head node of instructions table.
  * @param dbSymbolsPtr - Pointer to head node of symbols table.
  * @param commands - List of available commands
  * @param totalIns - count instructions rows
  * @param totalData - count data rows
  * @return EXIT_OK on success, EXIT_ABNORMAL on failure
  */
void firstScan(FILE *file, char fileName[], int *hasErrors, dbInstructsPtr *dbInstructsHead, dbDataPtr *dbDataHead,
               dbSymbolsPtr *dbSymbolsHead, cmdPtr *commands, int *totalIns, int *totalData);

/**
  * Second scan of .as file
  * update address on data table
  * run through instructions and try to build their data
  *
  * @param fileName - source file name
  * @param hasErrors - To tell if we had errors while compiling
  * @param dbInstructsHead - Pointer to head node of instructions table.s
  * @param dbSymbolsPtr - Pointer to head node of symbols table.
  * @param totalIns - count instructions rows
  * @return EXIT_OK on success, EXIT_ABNORMAL on failure
  */
void secondScan(char fileName[], int *hasErrors, dbInstructsPtr *dbInstructsHead, dbDataPtr *dbDataHead,
                dbSymbolsPtr *dbSymbolsHead, int *totalIns);


#endif

