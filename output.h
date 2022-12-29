/**
 * Responsible for creating output files
 */

#ifndef HSC_PROJECT_OUTPUT_H
#define HSC_PROJECT_OUTPUT_H

#include "structs.h"

#define FILE_BUILD_STATUS(t) (t ? COLOR_RED "FAIL" COLOR_RESET : COLOR_GREEN "OK" COLOR_RESET)

/**
 * Create output files with compilation data
 * @param dbInstructsHead Head of instructions table
 * @param dbDataHead Head of data table
 * @param name filename without extension
 * @param totalIns - count instructions rows
 * @param totalData - count data rows
 * @return 1 on error, 0 on success
 */
int outputExport(dbInstructsPtr *dbInstructsHead, dbDataPtr *dbDataHead, char name[], int totalIns, int totalData,
                 dbSymbolsPtr symbolsHead);

/**
 * Create output object file
 * @return 1 on error, 0 on success
 */
int outputExportObject(dbInstructsPtr *dbInstructsHead, dbDataPtr *dbDataHead, char name[], int totalIns, int totalData);

/**
 * Create output entries file
 * @return 1 on error, 0 on success
 */
int outputExportEntries(dbSymbolsPtr symbolsHead, char name[]);

/**
 * Create output externals file
 * @return 1 on error, 0 on success
 */
int outputExportExternals(dbSymbolsPtr symbolsHead, dbInstructsPtr *dbInstructsHead, char name[]);

/**
 * Convert number into HEX string.
 * @param decAddress address data to add (decimal). if less then 0 it won't be added
 * @param str pointer to string, in which we need to save the result to
 * @param data input number
 */
void outputLineIns(char *str, int decAddress, unsigned long data);

#endif
