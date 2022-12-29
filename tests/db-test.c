#include <stdio.h>
#include "tests.h"
#include "db-test.h"

void dbSymbolTest(cmdPtr *commands) {
    dbSymbols head;
    dbSymbolsPtr headPtr = &head, lastSym = NULL, res;
    parsedLine parsed1, parsed2;

    parseLine(&parsed1, "label: stop", commands);
    parseLine(&parsed2, "loopStart: stop", commands);

    /* entry allowed before even "label" is declared */
    TST_EQ(addSymbol(ADDRESS_UNDEFINED, SYM_ENTRY, 2, &headPtr, &lastSym, "label", "testing.mock"), 0, "%d")

    /* Mock data */
    TST_EQ(addSymbol(100, SYM_TYPE_DIR, 1, &headPtr, &lastSym, parsed1.label, "testing.mock"), 0, "%d")
    TST_EQ(addSymbol(104, SYM_TYPE_DIR, 2, &headPtr, &lastSym, parsed2.label, "testing.mock"), 0, "%d")
    TST_EQ(addSymbol(104, SYM_TYPE_DIR, 2, &headPtr, &lastSym, parsed2.label, "testing.mock"), 1, "%d")

    /* Test that the data was inserted */
    res = getSymbol("DontExists", &headPtr);
    TST_EQ(0, res ? 1 : 0, "%d")

    res = getSymbol("label", &headPtr);
    TST_STR(res ? res->symbol : "false", "label")

    res = getSymbol("loopStart", &headPtr);
    TST_STR(res ? res->symbol : "false", "loopStart")

    res = getSymbol("DontExists", &headPtr);
    TST_EQ(0, res ? 1 : 0, "%d")

    TST_EQ(addSymbol(ADDRESS_UNDEFINED, SYM_ENTRY, 2, &headPtr, &lastSym, "label", "testing.mock"), 0, "%d")
    TST_EQ(addSymbol(ADDRESS_UNDEFINED, SYM_ENTRY, 2, &headPtr, &lastSym, "label", "testing.mock"), 0, "%d")
    TST_EQ(addSymbol(ADDRESS_UNDEFINED, SYM_ENTRY, 2, &headPtr, &lastSym, "label", "testing.mock"), 0, "%d")
    res = getSymbol("label", &headPtr);
    TST_EQ(res->attributes, (SYM_TYPE_DIR | SYM_ENTRY), "%d")

    TST_EQ(addSymbol(ADDRESS_UNDEFINED, SYM_EXTERNAL, 2, &headPtr, &lastSym, "label", "testing.mock"), 1, "%d")
    TST_EQ(addSymbol(ADDRESS_UNDEFINED, SYM_EXTERNAL, 2, &headPtr, &lastSym, "labelFromOut", "testing.mock"), 0, "%d")
    TST_EQ(addSymbol(ADDRESS_UNDEFINED, SYM_EXTERNAL, 2, &headPtr, &lastSym, "labelFromOut", "testing.mock"), 0, "%d")
    res = getSymbol("labelFromOut", &headPtr);
    TST_EQ(res->attributes, SYM_EXTERNAL, "%d")
    TST_EQ(res->address, 0, "%d")

    free(headPtr->next);
    free(headPtr);
}

void dbInstructionTest(cmdPtr *commands) {
    int lineCounter = 0;
    dbInstructsPtr dbInstructsHead = NULL;
    dbInstructsPtr lastIns = NULL;
    cmdPtr cmd;
    parsedLinePtr parsed1, parsed2;

    parsed1 = calloc(1, sizeof(parsedLine));
    parseLine(parsed1, "label: add $1, $2, $3", commands);
    cmd = getCmd(parsed1->command, 0, commands);
    addInstruction(++lineCounter, &dbInstructsHead, &lastIns, parsed1, cmd);

    parsed2 = calloc(1, sizeof(parsedLine));
    parseLine(parsed2, "label2: jmp $1, $2, $3", commands);
    cmd = getCmd(parsed2->command, 0, commands);
    addInstruction(++lineCounter, &dbInstructsHead, &lastIns, parsed2, cmd);

    TST_STR(dbInstructsHead->parsed->command, "add");
    TST_EQ(dbInstructsHead->address, 100, "%d");
    TST_STR(dbInstructsHead->next->parsed->command, "jmp");
    TST_EQ(dbInstructsHead->next->address, 104, "%d");

    free(dbInstructsHead->next);
    free(dbInstructsHead);
    freeParsedLine(parsed1, 1);
    freeParsedLine(parsed2, 1);
}

void dbDataTest(cmdPtr *commands) {

}
