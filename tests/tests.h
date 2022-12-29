#ifndef HSC_PROJECT_TESTS_H
#define HSC_PROJECT_TESTS_H

#include <stdio.h>
#include <string.h>
#include "../config.h"

extern int GLOBAL_testsPassed;
extern int GLOBAL_testsFailed;

/**
 * This will show error message with the line and file name.
 */
#define TST_ERROR_MSG(value, expected, format) printf(COLOR_RED "TEST FAIL: %s:%d > \n\tActual:   "#format" \n\tExpected: "#format" \n" COLOR_RESET, __FILE__, __LINE__, value, expected);

/**
 * Test strings
 * allow to test using strcmp function.
 */
#define TST_STR(value, expected) if (strcmp(value, expected) != 0) { TST_ERROR_MSG(value, expected, "%s") \
    GLOBAL_testsFailed++;                                                                                 \
} else {                                                                                                  \
    GLOBAL_testsPassed++;                                                                                 \
}


/**
 * Test equal
 * allow to test using != operator.
 */
#define TST_EQ(value, expected, format) if ((value) != (expected)) { TST_ERROR_MSG(value, expected, format) \
    GLOBAL_testsFailed++;                                                                                   \
} else {                                                                                                    \
    GLOBAL_testsPassed++;                                                                                   \
}

#endif

