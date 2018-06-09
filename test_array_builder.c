#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

#include "test.h"
#include "array_builder.h"


bool test_populates_correct_size_uint8(void);
bool test_frozen_initial_vector_is_null(void);
bool test_frozen_cache_aligned_initial_vector_is_null(void);
bool test_frozen_aligned_initial_vector_is_null(void);
bool test_push_of_struct(void);
bool test_push_of_struct_of_many_structs(void);
bool test_push_of_struct_of_many_structs_aligned(void);
bool test_push_of_struct_of_many_structs_cache_aligned(void);


int main(int argc, char **argv) {
  RUN_TEST(test_populates_correct_size_uint8);

  RUN_TEST(test_frozen_initial_vector_is_null);
  RUN_TEST(test_frozen_aligned_initial_vector_is_null);
  RUN_TEST(test_frozen_cache_aligned_initial_vector_is_null);

  RUN_TEST(test_push_of_struct);

  RUN_TEST(test_push_of_struct_of_many_structs);
  RUN_TEST(test_push_of_struct_of_many_structs_aligned);
  RUN_TEST(test_push_of_struct_of_many_structs_cache_aligned);
}


bool test_populates_correct_size_uint8(void) {
  array_builder_t uint8array = new_array_builder(uint8_t);
  ASSERT(uint8array.element_size == sizeof(uint8_t));
  return true;
}


bool test_frozen_initial_vector_is_null(void) {
  uint64_t *dst = (uint64_t*) 1;
  size_t len = 0;
  array_builder_t array_b = new_array_builder(uint64_t);
  ASSERT(freeze(&array_b, (void*) &dst, &len) == 0);
  ASSERT(dst == 0);
  return true;
}

bool test_frozen_aligned_initial_vector_is_null(void) {
  uint64_t *dst = (uint64_t*) 1;
  const size_t alignment = 64;
  size_t len = 0;
  array_builder_t array_b = new_array_builder(uint64_t);
  ASSERT(freeze_aligned(&array_b, (void*) &dst, &len, alignment) == 0);
  ASSERT(dst == 0);
  return true;
}

bool test_frozen_cache_aligned_initial_vector_is_null(void) {
  uint64_t *dst = (uint64_t*) 1;
  size_t len = 0;
  array_builder_t array_b = new_array_builder(uint64_t);
  ASSERT(freeze_cache_aligned(&array_b, (void*) &dst, &len) == 0);
  ASSERT(dst == 0);
  return true;
}


bool test_push_of_struct(void) {
  typedef struct test_t {
    uint64_t a;
    uint32_t b;
  } test_t;

  test_t expected = (test_t) { 10, 20 };

  array_builder_t array_b = new_array_builder(test_t);
  ASSERT(push(&array_b, &expected) == ARRAY_BUILDER_SUCCESS);

  test_t *dst = NULL;
  size_t len = 0;
  ASSERT(freeze(&array_b, (void*) &dst, &len) == ARRAY_BUILDER_SUCCESS);

  ASSERT(len == 1);
  ASSERT(dst != NULL);
  ASSERT(dst->a == expected.a);
  ASSERT(dst->b == expected.b);

  free(dst);
  empty(&array_b);
  return true;
}

bool test_push_of_struct_of_many_structs(void) {
  const size_t iterations = 1000000;
  typedef struct test_t {
    uint64_t a;
    uint32_t b;
  } test_t;

  array_builder_t array_b = new_array_builder(test_t);
  for (size_t i = 0; i < iterations; ++i) {
    test_t item = (test_t) { i, i * 2 };
    ASSERT(push(&array_b, &item) == ARRAY_BUILDER_SUCCESS);
  }

  test_t *dst = NULL;
  size_t len = 0;
  ASSERT(freeze(&array_b, (void*) &dst, &len) == ARRAY_BUILDER_SUCCESS);

  ASSERT(len == iterations);
  ASSERT(dst != NULL);
  for (size_t i = 0; i < iterations; ++i) {
    ASSERT(dst[i].a == i);
    ASSERT(dst[i].b == i*2);
  }

  free(dst);
  empty(&array_b);
  return true;
}

bool test_push_of_struct_of_many_structs_aligned(void) {
  const size_t iterations = 1000000;
  const size_t alignment = 64;
  typedef struct test_t {
    uint64_t a;
    uint32_t b;
  } test_t;

  array_builder_t array_b = new_array_builder(test_t);
  for (size_t i = 0; i < iterations; ++i) {
    test_t item = (test_t) { i, i * 2 };
    ASSERT(push(&array_b, &item) == ARRAY_BUILDER_SUCCESS);
  }

  test_t *dst = NULL;
  size_t len = 0;
  ASSERT(freeze_aligned(&array_b, (void*) &dst, &len, alignment) == ARRAY_BUILDER_SUCCESS);

  /* Check our array is aligned in memory correctly */
  ASSERT(((uintptr_t)dst) % alignment == 0);

  ASSERT(len == iterations);
  ASSERT(dst != NULL);
  for (size_t i = 0; i < iterations; ++i) {
    ASSERT(dst[i].a == i);
    ASSERT(dst[i].b == i*2);
  }

  free(dst);
  empty(&array_b);
  return true;
}

bool test_push_of_struct_of_many_structs_cache_aligned(void) {
  const size_t iterations = 1000000;
  size_t alignment = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
  typedef struct test_t {
    uint64_t a;
    uint32_t b;
  } test_t;

  array_builder_t array_b = new_array_builder(test_t);
  for (size_t i = 0; i < iterations; ++i) {
    test_t item = (test_t) { i, i * 2 };
    ASSERT(push(&array_b, &item) == ARRAY_BUILDER_SUCCESS);
  }

  test_t *dst = NULL;
  size_t len = 0;
  ASSERT(freeze_cache_aligned(&array_b, (void*) &dst, &len) == ARRAY_BUILDER_SUCCESS);

  /* Check our array is aligned in memory correctly */
  ASSERT(((uintptr_t)dst) % alignment == 0);

  ASSERT(len == iterations);
  ASSERT(dst != NULL);
  for (size_t i = 0; i < iterations; ++i) {
    ASSERT(dst[i].a == i);
    ASSERT(dst[i].b == i*2);
  }

  free(dst);
  empty(&array_b);
  return true;
}
