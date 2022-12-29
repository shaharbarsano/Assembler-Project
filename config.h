/*
Project main config file
*/

#ifndef HSC_PROJECT_CONFIG_H
#define HSC_PROJECT_CONFIG_H

#define INPUT_FILE_EXT ".as" /* Extension string of allowed input file */

/**
 * Output constants
 */
#define OUTPUT_MAX_LINE_LENGTH 20
#define OUTPUT_EXT_OBJECT "ob"
#define OUTPUT_EXT_ENTRIES "ent"
#define OUTPUT_EXT_EXTERNALS "ext"

#define MAX_NUM_OF_COMMANDS 34 /* Maximum number of commands (instructs + directives) */
#define MAX_NUM_OF_OP 5 /* Maximum num and types of operands */

#define INS 0 /* refer to cmd type instructions */
#define DIR 1 /* refer to cmd type directive */

#define OP_NUM_THREE 3 /* exact 3 operands */
#define OP_NUM_TWO 2 /* exact 2 operands */
#define OP_NUM_ONE 1 /* exact 1 operands */
#define OP_NUM_NONE 0 /* no operands at all */
#define OP_NUM_MULTI -1 /* refer to directive type .dh/.dw/.db */

#define OP_VAL_REGISTER 2 /* refer to operand type register */
#define OP_VAL_NUMBER 3 /* refer to operand type number */
#define OP_VAL_LABEL 4 /* refer to operand type label */
#define OP_VAL_STRING 5 /* refer to operand type string */
#define OP_VAL_REG_OR_LAB 6 /* refer to operands type register or label */

#define MAX_LINE_SIZE 82 /* Max line length */
#define MAX_LABEL_SIZE 31 /* Max label name length */
#define MAX_CMD_SIZE 8 /* Max command name length */
#define MAX_CHUNK_SIZE 60 /* Max one chunk\operand length */
#define MAX_CHUNKS 43 /* Max amount of chunks\operands per line */
#define MAX_ATTRIBUTES_SIZE 10 /* Max attributes name length */

#define LINE_COMMENT_START ';' /* The char to declare comment */
#define LINE_LABEL_END ':' /* The char at the end of each label */
#define LINE_OPERAND_SEP ',' /* Operands separator */
#define LINE_STRING_ESCAPE '\\' /* Operands separator */

/* Parser Each line error constant has one bit only */
#define LINE_ERROR_OK 0
#define LINE_ERROR_CMD 1
#define LINE_ERROR_OPERANDS 2
#define LINE_ERROR_LABEL_CMD 4

/* opcode for commands */
#define OPCODE_ADD 0
#define OPCODE_SUB 0
#define OPCODE_AND 0
#define OPCODE_OR 0
#define OPCODE_NOR 0
#define OPCODE_MOVE 1
#define OPCODE_MVHI 1
#define OPCODE_MVLO 1
#define OPCODE_ADDI 10
#define OPCODE_SUBI 11
#define OPCODE_ANDI 12
#define OPCODE_ORI 13
#define OPCODE_NORI 14
#define OPCODE_BNE 15
#define OPCODE_BEQ 16
#define OPCODE_BLT 17
#define OPCODE_BGT 18
#define OPCODE_LB 19
#define OPCODE_SB 20
#define OPCODE_LW 21
#define OPCODE_SW 22
#define OPCODE_LH 23
#define OPCODE_SH 24
#define OPCODE_JMP 30
#define OPCODE_LA 31
#define OPCODE_CALL 32
#define OPCODE_STOP 63
#define OPCODE_DH 710
#define OPCODE_DW 711
#define OPCODE_DB 712
#define OPCODE_ASCIZ 713
#define OPCODE_ENTRY 714
#define OPCODE_EXTERN 715


/*
 * Custom attributes for symbols records on the symbols table
 */
#define SYM_TYPE_UNDEFINED 0
#define SYM_TYPE_INS 1
#define SYM_TYPE_DIR 2
#define SYM_ENTRY 4
#define SYM_EXTERNAL 8
#define ADDRESS_UNDEFINED 0 /* no address for symbols type .entry, .extern */

/* Main exit status codes */
#define EXIT_OK 0
#define EXIT_ABNORMAL 1

/* Console colors */
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_RESET   "\x1b[0m"
#define PREFIX_WARNING COLOR_BLUE "WARNING: " COLOR_RESET
#define PREFIX_ERROR COLOR_RED "ERROR: " COLOR_RESET

#define ERROR_LIST_ITEM "\t" COLOR_RED "(*)" COLOR_RESET
#define ERROR_LIST_SUBITEM "\t" COLOR_RED "   >>" COLOR_RESET

#endif
