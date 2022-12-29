/**
 * test creating output files
 */
#include <stdio.h>
#include <stdlib.h>
#include "output-test.h"
#include "tests.h"
#include "../structs.h"
#include "../output.h"
#include "../parser.h"
#include "../builders.h"

void convertTest() {
    char str[OUTPUT_MAX_LINE_LENGTH + 1];
    unsigned long expected;
    cmdTypePtr ptr;

    outputLineIns(str, 100, strtol("00000000011001010100100001000000", NULL, 2));
    TST_STR(str, "0100 40 48 65 00");

    outputLineIns(str, 5, strtol("00110101001000101111111111111011", NULL, 2));
    TST_STR(str, "0005 FB FF 22 35");

    outputLineIns(str, -1, strtol("01111100000000000000000000000000", NULL, 2));
    TST_STR(str, "00 00 00 7C");

    /* Test cmdTypePtr bundling */
    /*
     * R
     */
    ptr.R = calloc(1, sizeof(cmdR));
    if (!ptr.R) {
        printf("can not allocate memory");
        exit(EXIT_ABNORMAL);
    }
    ptr.R->opcode = 0;
    ptr.R->funct = 1;
    ptr.R->rs = 9;
    ptr.R->rt = 12;
    ptr.R->rd = 31;

    expected = strtol("00000001001011001111100001000000", NULL, 2);
    TST_EQ(bundleStructIns(ptr, 'R'), expected, "%lu");

    free(ptr.R);

    /*
     * I
     */
    ptr.I = calloc(1, sizeof(cmdI));
    if (!ptr.I) {
        printf("can not allocate memory");
        exit(EXIT_ABNORMAL);
    }
    ptr.I->opcode = 17;
    ptr.I->rs = 6;
    ptr.I->rt = 16;
    ptr.I->immed = 60912;
    expected = strtol("01000100110100001110110111110000", NULL, 2);
    TST_EQ(bundleStructIns(ptr, 'I'), expected, "%lu");

    ptr.I->immed = -28;
    expected = strtol("01000100110100001111111111100100", NULL, 2);
    TST_EQ(bundleStructIns(ptr, 'I'), expected, "%lu");

    free(ptr.I);

    /*
     * J
     */
    ptr.J = calloc(1, sizeof(cmdJ));
    if (!ptr.J) {
        printf("can not allocate memory");
        exit(EXIT_ABNORMAL);
    }
    ptr.J->opcode = 24;
    ptr.J->reg = 1;
    ptr.J->address = 33554431;

    expected = strtol("01100011111111111111111111111111", NULL, 2);
    TST_EQ(bundleStructIns(ptr, 'J'), expected, "%lu");

    /* J overflow test */
    ptr.J->opcode = 24;
    ptr.J->reg = 1;
    ptr.J->address = 1677721599;

    expected = strtol("01100011111111111111111111111111", NULL, 2);
    TST_EQ(bundleStructIns(ptr, 'J'), expected, "%lu");

    /* J overflow test */
    ptr.J->opcode = 24;
    ptr.J->reg = 0;
    ptr.J->address = -250;

    expected = strtol("01100001111111111111111100000110", NULL, 2);
    TST_EQ(bundleStructIns(ptr, 'J'), expected, "%lu");

    free(ptr.J);
}

void dataByteTest(cmdPtr *commands) {
    parsedLinePtr parsed = calloc(1, sizeof(parsedLine));
    cmdPtr cmd;
    dbDataPtr head = calloc(1, sizeof(dbData)), last = calloc(1, sizeof(dbData));
    int lineNumber = 0;

    parseLine(parsed, ".asciz \"aBcd\"", commands);
    cmd = getCmd(parsed->command, 0, commands);
    addData(++lineNumber, &head, &last, parsed, cmd);

    TST_EQ(last->countBytes, 5, "%d")
    TST_EQ(buildDataRowByte(last, 0), 'a', "%c")
    TST_EQ(buildDataRowByte(last, 1), 'B', "%c")
    TST_EQ(buildDataRowByte(last, 2), 'c', "%c")
    TST_EQ(buildDataRowByte(last, 3), 'd', "%c")
    TST_EQ(buildDataRowByte(last, 4), 0, "%c")

    parseLine(parsed, ".db 6, -9", commands);
    cmd = getCmd(parsed->command, 0, commands);
    addData(++lineNumber, &head, &last, parsed, cmd);
    TST_EQ(last->countBytes, 2, "%d")
    TST_EQ(buildDataRowByte(last, 0), (int)strtol("00000110", NULL, 2), "%X")
    TST_EQ(buildDataRowByte(last, 1), (int)strtol("11110111", NULL, 2), "%X")
    TST_EQ(buildDataRowByte(last, 2), 0, "%X")

    parseLine(parsed, ".dh 1235, -12345", commands);
    cmd = getCmd(parsed->command, 0, commands);
    addData(++lineNumber, &head, &last, parsed, cmd);
    TST_EQ(last->countBytes, 4, "%d")
    TST_EQ(buildDataRowByte(last, 0), (int)strtol("11010011", NULL, 2), "%X")
    TST_EQ(buildDataRowByte(last, 1), (int)strtol("00000100", NULL, 2), "%X")
    TST_EQ(buildDataRowByte(last, 2), (int)strtol("11000111", NULL, 2), "%X")
    TST_EQ(buildDataRowByte(last, 3), (int)strtol("11001111", NULL, 2), "%X")

    parseLine(parsed, ".dw 31, -12, -291214021, 921941924", commands);
    cmd = getCmd(parsed->command, 0, commands);
    addData(++lineNumber, &head, &last, parsed, cmd);
    TST_EQ(last->countBytes, 16, "%d")
    TST_EQ(buildDataRowByte(last, 0), (int)strtol("00011111", NULL, 2), "%X")
    TST_EQ(buildDataRowByte(last, 1), (int)strtol("00000000", NULL, 2), "%X")
    TST_EQ(buildDataRowByte(last, 2), (int)strtol("00000000", NULL, 2), "%X")
    TST_EQ(buildDataRowByte(last, 3), (int)strtol("00000000", NULL, 2), "%X")

    TST_EQ(buildDataRowByte(last, 4), (int)strtol("11110100", NULL, 2), "%X")
    TST_EQ(buildDataRowByte(last, 5), (int)strtol("11111111", NULL, 2), "%X")
    TST_EQ(buildDataRowByte(last, 6), (int)strtol("11111111", NULL, 2), "%X")
    TST_EQ(buildDataRowByte(last, 7), (int)strtol("11111111", NULL, 2), "%X")

    TST_EQ(buildDataRowByte(last, 8), (int)strtol("00111011", NULL, 2), "%X")
    TST_EQ(buildDataRowByte(last, 9), (int)strtol("01101101", NULL, 2), "%X")
    TST_EQ(buildDataRowByte(last, 10), (int)strtol("10100100", NULL, 2), "%X")
    TST_EQ(buildDataRowByte(last, 11), (int)strtol("11101110", NULL, 2), "%X")

    TST_EQ(buildDataRowByte(last, 12), (int)strtol("10100100", NULL, 2), "%X")
    TST_EQ(buildDataRowByte(last, 13), (int)strtol("10110111", NULL, 2), "%X")
    TST_EQ(buildDataRowByte(last, 14), (int)strtol("11110011", NULL, 2), "%X")
    TST_EQ(buildDataRowByte(last, 15), (int)strtol("00110110", NULL, 2), "%X")
}
