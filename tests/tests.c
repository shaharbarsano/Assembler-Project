#include <stdio.h>
#include <stdlib.h>
#include "../config.h"
#include "../structs.h"
#include "../commands.h"
#include "parser-test.h"
#include "validation-test.h"
#include "db-test.h"
#include "commands-test.h"
#include "output-test.h"

/**
 * Global tests status variables
 */
int GLOBAL_testsPassed = 0;
int GLOBAL_testsFailed = 0;

int main() {
    /* Init all our commands */
    cmdPtr *commands = initCmds();

    /* Start tests: */
    dataByteTest(commands);
    convertTest();
    parseLineTest(commands);
    cmdOpValidateTest(commands);
    dbSymbolTest(commands);
    dbInstructionTest(commands);
    dbDataTest(commands);
    cmdTest(commands);

    /* Free memory */
    freeCmds(commands);

    /* print results */
    printf("\n************************\n");
    printf("Tests result: %s\n", GLOBAL_testsFailed == 0 ? COLOR_GREEN "All tests passed!" COLOR_RESET : "");
    printf(COLOR_GREEN "%d" COLOR_RESET " Passed\n", GLOBAL_testsPassed);
    printf(COLOR_RED "%d" COLOR_RESET " Failed\n", GLOBAL_testsFailed);
    printf("************************\n");

    return GLOBAL_testsFailed == 0 ? EXIT_OK : EXIT_ABNORMAL;
}
