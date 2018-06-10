#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "array_builder.h"


/* Helper functions */
void* allocate_new_buffer(const array_builder_t * const ab, size_t length);
void* allocate_new_buffer_aligned(const array_builder_t * const ab, size_t length, size_t alignment);
void* duplicate_buffer(const array_builder_t * const ab, size_t length);
void copy_buffer(void* target, const array_builder_t *const ab);


/* Obtain a new array builder consisting of elements of the provided size */
array_builder_t _new_array_builder(size_t element_size) {
  return (array_builder_t) {
    .array = 0,
    .members = 0,
    .size = 0,
    .element_size = element_size,
  };
}


/* Allocate a fixed size array with the same contents as the builder */
int32_t freeze(const array_builder_t* const ab, void **dst, size_t *const size) {
  void* frozen = NULL;

  if(!ab->members) {
    *dst = NULL;
    return ARRAY_BUILDER_SUCCESS;
  }

  if (frozen = duplicate_buffer(ab, ab->members)) {
    *dst = frozen;
    *size = ab->members;
    return ARRAY_BUILDER_SUCCESS;
  }
  else {
    return ARRAY_BUILDER_ALLOCATION_ERROR;
  }
}

/* Allocate an aligned fixed size array with the same contents as the builder */
int32_t freeze_aligned(const array_builder_t* const ab, void **dst, size_t *const size, size_t alignment) {
  if(!ab->members) {
    *dst = NULL;
    return ARRAY_BUILDER_SUCCESS;
  }

  void* new_buffer = allocate_new_buffer_aligned(ab, ab->members, alignment);
  if (new_buffer) {
    copy_buffer(new_buffer, ab);
    *dst = new_buffer;
    *size = ab->members;
    return ARRAY_BUILDER_SUCCESS;
  }
  else {
    return ARRAY_BUILDER_ALLOCATION_ERROR;
  }
}


/* Allocate an L1 cache aligned fixed size array with the
   same contents as the builder */
int32_t freeze_cache_aligned(const array_builder_t* const ab, void **dst, size_t *const size) {
  size_t alignment = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
  return freeze_aligned(ab, dst, size, alignment);
}


/* Copy an element onto the end of the builder */
int32_t push(array_builder_t* const ab, const void *const value) {
  if (ab->members < ab->size) {
    /* Do a generic copy by using the size of the elements */
    void* position = ab->array + (ab->members++ * ab->element_size);
    memcpy(position, value, ab->element_size);
    return ARRAY_BUILDER_SUCCESS;
  }
  else {
    void* new_buffer = NULL;
    size_t new_size = 2 * ab->size + 1;
    size_t new_size_bytes = new_size * ab->element_size;

    if(new_buffer = realloc(ab->array, new_size_bytes)) {
      ab->array = new_buffer;
      ab->size = new_size;
      return push(ab, value);
    }
    else {
      return ARRAY_BUILDER_ALLOCATION_ERROR;
    }
  }
}


/* Empty the contents of the builder (also frees it's buffer) */
void empty(array_builder_t* const ab) {
  if(ab->array) {
    free(ab->array);
  }
  ab->array = NULL;
  ab->size = 0;
  ab->members = 0;
}


/* Helper function definitions */


void* duplicate_buffer(const array_builder_t * const ab, size_t length) {
  /* Make sure we're trying to duplicate to a buffer with enough space */
  if (length < ab->members) {
    return NULL;
  }

  void* new_buffer = allocate_new_buffer(ab, length);

  if(new_buffer) {
    copy_buffer(new_buffer, ab);
  }

  return new_buffer;
}


void* allocate_new_buffer(const array_builder_t * const ab, size_t length) {
  return calloc(length, ab->element_size);
}


void* allocate_new_buffer_aligned(const array_builder_t * const ab, size_t length, size_t alignment) {
  void *new_buffer = NULL;
  if(posix_memalign(&new_buffer, alignment, length * ab->element_size) == 0) {
    return new_buffer;
  }
  else {
    return NULL;
  }
}


void copy_buffer(void* target, const array_builder_t *const ab) {
  memcpy(target, ab->array, ab->members * ab->element_size);
}
