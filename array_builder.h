#pragma once

#define ARRAY_BUILDER_SUCCESS 0
#define ARRAY_BUILDER_ALLOCATION_ERROR 1

#define new_array_builder(type) _new_array_builder(sizeof(type))

typedef struct array_builder_t {
  void *array;
  size_t members;
  size_t size;
  size_t element_size;
} array_builder_t;

array_builder_t _new_array_builder(size_t element_size);

int32_t freeze(const array_builder_t* const ab, void **dst, size_t *const size);
int32_t freeze_aligned(const array_builder_t* const ab, void **dst, size_t *const size, size_t alignment);
int32_t freeze_cache_aligned(const array_builder_t* const ab, void **dst, size_t *const size);

int32_t push(array_builder_t* const ab, const void *const value);

void empty(array_builder_t* const ab);
