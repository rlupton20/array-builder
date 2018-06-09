#pragma once
/* Macros to provide a basic test infrastructure */
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdio.h>

atomic_uintmax_t failed = 0;


#define ASSERT(cond) { if(!(cond)) \
      { printf("\x1b[31m\x1b[1mFAIL\x1b[0m \n  %s:%d assertion failed: %s\n", __FILE__, __LINE__, #cond); return false; } }

#define ASSERT_NOT(cond) ASSERT(!(cond))

#define RUN_TEST(test) { printf("%-50s \t", #test);                    \
                         if ((test)() == true) { printf("\x1b[32m\x1b[1mPASS\x1b[0m\n"); } \
                         else { ++failed;} }

void summarise_results(void) {
  if(failed) {
    printf("%zu tests failed\n", failed);
    exit(1);
  }
  else {
    printf("Success!\n");
  }
}
