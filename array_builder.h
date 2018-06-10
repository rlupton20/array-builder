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


/* Obtain a new array builder consisting of elements of the provided size */
array_builder_t _new_array_builder(size_t element_size);

/* Allocate a fixed size array with the same contents as the builder */
int32_t freeze(const array_builder_t* const ab, void **dst, size_t *const size);
/* Allocate an aligned fixed size array with the same contents as the builder */
int32_t freeze_aligned(const array_builder_t* const ab, void **dst, size_t *const size, size_t alignment);
/* Allocate an L1 cache aligned fixed size array with the
   same contents as the builder */
int32_t freeze_cache_aligned(const array_builder_t* const ab, void **dst, size_t *const size);

/* Copy an element onto the end of the builder */
int32_t push(array_builder_t* const ab, const void *const value);

/* Empty the contents of the builder (also frees it's buffer) */
void empty(array_builder_t* const ab);
