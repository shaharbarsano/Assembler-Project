#include <stdio.h>
#include "config.h"
#include "structs.h"
#include "parser.h"
#include "commands.h"

/**
 * Initial main function.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    int i;
    cmdPtr *commands;

    if (argc < 2) {
        printf(PREFIX_ERROR "no files entered");
        return EXIT_ABNORMAL;
    }

    /* Init all our commands */
    commands = initCmds();

    /* Loop through received arguments */
    for (i = 1; i < argc; i++) {
        parseFile(argv[i], commands);
    }

    freeCmds(commands);

    return EXIT_OK;
}
