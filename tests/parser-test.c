/**
 * Test parser functions
 */
#include <stdio.h>
#include "tests.h"
#include "parser-test.h"

void parseLineTest(cmdPtr *commands) {
    parsedLinePtr tmp = calloc(1, sizeof(parsedLine));

    parseLine(tmp, "    \t\t     ", commands);
    TST_EQ(tmp->ignore, 1, "%d")
    TST_EQ(tmp->error, 0, "%d")
    TST_STR(tmp->label, "")
    TST_EQ(tmp->opNum, 0, "%d")
    freeParsedLine(tmp, 0);

    parseLine(tmp, " ;;;; comment", commands);
    TST_EQ(tmp->ignore, 1, "%d")
    TST_EQ(tmp->error, 0, "%d")
    TST_STR(tmp->label, "")
    TST_EQ(tmp->opNum, 0, "%d")
    freeParsedLine(tmp, 0);

    parseLine(tmp, "", commands);
    TST_EQ(tmp->error, 0, "%d")
    TST_EQ(tmp->ignore, 1, "%d")
    TST_STR(tmp->label, "")
    TST_EQ(tmp->opNum, 0, "%d")
    freeParsedLine(tmp, 0);

    parseLine(tmp, "abc", commands);
    TST_EQ(tmp->error, LINE_ERROR_OK, "%d")
    TST_STR(tmp->command, "abc")
    TST_STR(tmp->label, "")
    TST_EQ(tmp->opNum, 0, "%d")
    freeParsedLine(tmp, 0);

    parseLine(tmp, " a: Ok ", commands);
    TST_EQ(tmp->error, 0, "%d")
    TST_STR(tmp->label, "a")
    TST_STR(tmp->command, "Ok")
    TST_EQ(tmp->opNum, 0, "%d")
    freeParsedLine(tmp, 0);

    parseLine(tmp, "lllb:cmd ", commands);
    TST_EQ(tmp->error, 0, "%d")
    TST_STR(tmp->label, "lllb")
    TST_STR(tmp->command, "cmd")
    TST_EQ(tmp->opNum, 0, "%d")
    freeParsedLine(tmp, 0);

    parseLine(tmp, "add:cmd ", commands);
    TST_EQ(tmp->error, LINE_ERROR_LABEL_CMD, "%d")
    TST_STR(tmp->label, "add")
    freeParsedLine(tmp, 0);

    parseLine(tmp, "asciz:cmd ", commands);
    TST_EQ(tmp->error, LINE_ERROR_LABEL_CMD, "%d")
    TST_STR(tmp->label, "asciz")
    freeParsedLine(tmp, 0);

    parseLine(tmp, "   ab: >>>", commands);
    TST_STR(tmp->label, "ab")
    TST_STR(tmp->command, ">>>")
    TST_EQ(tmp->opNum, 0, "%d")
    freeParsedLine(tmp, 0);

    parseLine(tmp, " MyLbl: a b", commands);
    TST_EQ(tmp->ignore, 0, "%d")
    TST_STR(tmp->label, "MyLbl")
    TST_STR(tmp->command, "a")
    TST_EQ(tmp->opNum, 1, "%d")
    TST_STR(tmp->operands[0], "b")
    freeParsedLine(tmp, 0);

    parseLine(tmp, "9bd: a , b", commands);
    TST_EQ(tmp->error, 0, "%d")
    TST_STR(tmp->label, "")
    TST_STR(tmp->command, "9bd:")
    TST_EQ(tmp->opNum, 2, "%d")
    TST_STR(tmp->operands[0], "a")
    TST_STR(tmp->operands[1], "b")
    freeParsedLine(tmp, 0);

    parseLine(tmp, "lbl: cmd a , , \"str  te,xt\" , b     , ttt2        ,    o", commands);
    TST_EQ(tmp->error, LINE_ERROR_OPERANDS, "%d")
    TST_STR(tmp->label, "lbl")
    TST_STR(tmp->command, "cmd")
    TST_EQ(tmp->opNum, 5, "%d")
    TST_STR(tmp->operands[0], "a")
    TST_STR(tmp->operands[1], "\"str  te,xt\"")
    TST_STR(tmp->operands[2], "b")
    TST_STR(tmp->operands[3], "ttt2")
    TST_STR(tmp->operands[4], "o")
    freeParsedLine(tmp, 0);

    parseLine(tmp, "cmd \"long string\"", commands);
    TST_EQ(tmp->error, LINE_ERROR_OK, "%d")
    TST_STR(tmp->command, "cmd")
    TST_EQ(tmp->opNum, 1, "%d")
    TST_STR(tmp->operands[0], "\"long string\"")
    freeParsedLine(tmp, 0);

    parseLine(tmp, "cmd \"long \", string\"", commands);
    TST_EQ(tmp->error, LINE_ERROR_OPERANDS, "%d")
    freeParsedLine(tmp, 0);

    parseLine(tmp, "cmd \"long \\\", string\"", commands);
    TST_EQ(tmp->error, LINE_ERROR_OK, "%d")
    TST_EQ(tmp->opNum, 1, "%d")
    TST_STR(tmp->operands[0], "\"long \", string\"")
    freeParsedLine(tmp, 0);

    parseLine(tmp, "cmd \"long string\\\"  ", commands);
    TST_EQ(tmp->error, LINE_ERROR_OPERANDS, "%d")
    TST_EQ(tmp->opNum, 0, "%d")
    freeParsedLine(tmp, 0);

    parseLine(tmp, "cmd \"long string\\\\\"", commands);
    TST_EQ(tmp->error, LINE_ERROR_OK, "%d")
    TST_EQ(tmp->opNum, 1, "%d")
    TST_STR(tmp->operands[0], "\"long string\\\"")
    freeParsedLine(tmp, 0);

    free(tmp);
}
