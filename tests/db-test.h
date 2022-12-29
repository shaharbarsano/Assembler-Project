/*
 * Test database tables
 */


#ifndef HSC_PROJECT_DB_TEST_H
#define HSC_PROJECT_DB_TEST_H

#include "../structs.h"
#include "../commands.h"

void dbSymbolTest(cmdPtr *commands);
void dbInstructionTest(cmdPtr *commands);
void dbDataTest(cmdPtr *commands);

#endif
