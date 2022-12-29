/*
// Test validations
*/

#include <stdio.h>
#include "tests.h"
#include "validation-test.h"
#include "../validation.h"

void cmdOpValidateTest(cmdPtr *commands) {
    /* Register validation */
    TST_EQ(isValidRegister("$0"), 1, "%d");
    TST_EQ(isValidRegister("$1"), 1, "%d");
    TST_EQ(isValidRegister("$15"), 1, "%d");
    TST_EQ(isValidRegister("$31"), 1, "%d");
    TST_EQ(isValidRegister("$32"), 0, "%d");
    TST_EQ(isValidRegister("$-1"), 0, "%d");
    TST_EQ(isValidRegister("$91"), 0, "%d");
    TST_EQ(isValidRegister(" $0"), 0, "%d");
    TST_EQ(isValidRegister("$0 "), 0, "%d");
    TST_EQ(isValidRegister("$"), 0, "%d");
    TST_EQ(isValidRegister(""), 0, "%d");

    TST_EQ(isValidNumber("+7655434", OPCODE_DW), 1, "%d");
    TST_EQ(isValidNumber("+48569711", OPCODE_BNE), 1, "%d");
    TST_EQ(isValidNumber("-32765", OPCODE_DH), 1, "%d");
    TST_EQ(isValidNumber("32765", OPCODE_SH), 1, "%d");
    TST_EQ(isValidNumber("120", OPCODE_DB), 1, "%d");
    TST_EQ(isValidNumber("+0", OPCODE_LB), 1, "%d");
    TST_EQ(isValidNumber("-45", OPCODE_DH), 1, "%d");
    TST_EQ(isValidNumber("-000005", OPCODE_ADDI), 1, "%d");
    TST_EQ(isValidNumber("00000", OPCODE_SUBI), 1, "%d");
    TST_EQ(isValidNumber("0", OPCODE_ANDI), 1, "%d");
    TST_EQ(isValidNumber("0000000000000000", OPCODE_ORI), 1, "%d");
    TST_EQ(isValidNumber("00000000000120", OPCODE_DB), 1, "%d");
    TST_EQ(isValidNumber("-000000000000120456", OPCODE_ADDI), 1, "%d");
    TST_EQ(isValidNumber("2147483649", OPCODE_DW), 0, "%d");
    TST_EQ(isValidNumber("-2147483649", OPCODE_NORI), 0, "%d");
    TST_EQ(isValidNumber("+32768", OPCODE_SB), 0, "%d");
    TST_EQ(isValidNumber("+327689", OPCODE_DH), 0, "%d");
    TST_EQ(isValidNumber("+128", OPCODE_DB), 0, "%d");
    TST_EQ(isValidNumber("-1156", OPCODE_DB), 0, "%d");
    TST_EQ(isValidNumber("234.50", OPCODE_LW), 0, "%d");
    TST_EQ(isValidNumber("+no.numbers", OPCODE_SW), 0, "%d");
    TST_EQ(isValidNumber("9999999999", OPCODE_LH), 0, "%d");
    TST_EQ(isValidNumber("-458458.0", OPCODE_SH), 0, "%d");
    TST_EQ(isValidNumber("+", OPCODE_DB), 0, "%d");

    /* String validation */
    TST_EQ(isValidString("\"\"\""), 1, "%d");
    TST_EQ(isValidString("\"ab\"c\""), 1, "%d");
    TST_EQ(isValidString("\"hello world\""), 1, "%d");
    TST_EQ(isValidString("\"no tabs \t\""), 0, "%d");
    TST_EQ(isValidString("\"\""), 0, "%d");
    TST_EQ(isValidString("\"abc"), 0, "%d");
    TST_EQ(isValidString("abc\""), 0, "%d");
    TST_EQ(isValidString("\""), 0, "%d");
    TST_EQ(isValidString("hello world"), 0, "%d");

    /* Label validation */
    TST_EQ(isValidLabel("Asciz"), 1, "%d");
    TST_EQ(isValidLabel("Add"), 1, "%d");
    TST_EQ(isValidLabel("ab123"), 1, "%d");
    TST_EQ(isValidLabel("abcabcabcabcabcabcabcabcabcabca"), 1, "%d");
    TST_EQ(isValidLabel("adder"), 1, "%d");
    TST_EQ(isValidLabel("5abcd"), 0, "%d");
    TST_EQ(isValidLabel("abcdefghijklmnopqrstuvwxyz123456"), 0, "%d");
}
