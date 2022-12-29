/**
 * Define all structs
 */

#ifndef HSC_PROJECT_STRUCTS_H
#define HSC_PROJECT_STRUCTS_H

#include "config.h"

/**
 * Pointers for structs at top so we can use them inside other structs
 */
typedef struct cmd *cmdPtr;
typedef struct dbDataNode *dbDataPtr;
typedef struct dbSymbolsNode *dbSymbolsPtr;
typedef struct dbInstructsNode *dbInstructsPtr;
typedef struct parsedLine *parsedLinePtr;

/**
 * Parsed line syntax
 */
typedef struct parsedLine {
    /**
     * ignore - will put 1 when this line empty or comment.
     *          will put 0 on any other case.
     */
    int ignore: 2;
    /**
     * error - bit sum of LINE_ERROR... constants
     */
    int error: 5;
    /** label - will put line label name or null */
    char label[MAX_LABEL_SIZE + 2];
    /** command - will put line command name or null */
    char command[MAX_CMD_SIZE + 1];
    /**
     * operands - will fill array with line operands or empty array
     * */
    char *operands[MAX_CHUNKS];

    /**
     * Number of operands
     */
    int opNum;
} parsedLine;

/**
 * Command type R
 */
typedef struct cmdR *cmdRPtr;
typedef struct cmdR {
    int opcode : 6;
    int rs : 6;
    int rt : 6;
    int rd : 6;
    int funct : 5;
} cmdR;

/**
 * Command type I
 */
typedef struct cmdI *cmdIPtr;
typedef struct cmdI {
    int opcode : 6;
    int rs : 6;
    int rt : 6;
    int immed;
} cmdI;

/**
 * Command type J
 */
typedef struct cmdJ *cmdJPtr;
typedef struct cmdJ {
    int opcode : 6;
    unsigned int reg : 1;
    long address;
} cmdJ;

/**
 * Union for having one of the commands type pointers
 */
typedef union cmdTypePtr {
    cmdRPtr R;
    cmdIPtr I;
    cmdJPtr J;
} cmdTypePtr;

/**
 * Instructions table
 */
typedef struct dbInstructsNode {
    int address;
    int lineNumber;
    cmdPtr cmd;
    parsedLinePtr parsed;
    cmdTypePtr cmdTypePtr;
    dbInstructsPtr next;
} dbInstructs;

/**
 * Data table
 */
typedef struct dbDataNode {
    int address;
    int countBytes;
    cmdPtr cmd;
    parsedLinePtr parsed;
    dbDataPtr next;
} dbData;

/**
 * Commands structure
 */
typedef struct cmd {
    char name[MAX_CMD_SIZE + 1];
    /** cmdType - 0 for instruction
     *            1 for directive
     */
    int cmdType;

    /**
     * If instruction, set type: 'R', 'I', or 'J'
     */
    char cmdInsType;

    /**
     * For validation. tell how much operands command can have.
     * opNum -     one of OP_NUM_... constants.
     *             if OP_NUM_MULTI opValidation must be length 1 to tell which type of operands to accept. (f.e. directive type .dh/.dw/.db)
     *             any other value- opNum must match opValidation length!
     */
    int opNum;

    /**
     * Validate commands operands. possible to validate both operands type and operands number.
     *
     * if opNum=-1 - opValidation will validate only type.
     *    on any other case, it will try to validate both type and amount of operands.
     */
    int opValidation[MAX_NUM_OF_OP];

    /**
     * Builder function pointer
     * this function responsible for creating correct output compiled data for each command.
     * @return 1 on failure, 0 on success
     */
    int (*builder)(cmdPtr cmd, dbInstructsPtr insRow, dbSymbolsPtr *symbolsHead, char fileName[]);

    /**
     * Command OPCODE value.
     * when -1 no OPCODE value
     */
    int opcode;

    /**
     * Command FUNCT value.
     * when -1 no FUNCT value
     */
    int funct;
} cmd;

/**
 * Symbols table
 */
typedef struct dbSymbolsNode {
    char symbol[MAX_LABEL_SIZE + 2];
    int address;
    int attributes;
    int lineNumber;
    dbSymbolsPtr next;
} dbSymbols;


#endif
