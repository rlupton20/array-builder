#pragma once
/* Macros to provide a basic test infrastructure */
#include <stdio.h>
#include <stdbool.h>

#define ASSERT(cond) { if(!(cond)) \
      { printf("\x1b[31m\x1b[1mFAIL\x1b[0m \n  %s:%d assertion failed: %s\n", __FILE__, __LINE__, #cond); return false; } }

#define ASSERT_NOT(cond) ASSERT(!(cond))

#define RUN_TEST(test) { printf("%-50s \t", #test);                    \
    if ((test)() == true) { printf("\x1b[32m\x1b[1mPASS\x1b[0m\n"); } }
