#include <stdlib.h>
#include <string.h>

#include "vector.h"
#include "vector_private.h"

vector* vector_new(size_t capacity, size_t element_size) {
  if (capacity == 0 || element_size == 0)
    return NULL;

  vector *v = malloc(sizeof (vector));
  if (!v)
    return v;

  int ret = vector_init(v, capacity, element_size);
  if (ret) {
    free(v);
    return NULL;
  }

  return v;
}

int vector_init(vector *v, size_t capacity, size_t element_size) {
  if (!v || capacity == 0 || element_size == 0) {
    return -1;
  }

  v->data = malloc(sizeof (char) * capacity * element_size);
  if (!v->data) {
    return -1;
  }

  v->capacity = capacity;
  v->element_size = element_size;
  v->size = 0;

  return 0;
}

void* vector_get(struct vector *v, size_t index) {
  return v->data + v->element_size * index;
}

void vector_free (vector *v) {
  if (v->capacity > 0)
    free (v->data);
}

int vector_push_back (vector *v, void *element) {
  if (v->size + 1 > v->capacity) {
    int ret = vector_increase_size(v);
    if (ret) {
      return ret;
    }
  }

  memcpy(v->data + v->size * v->element_size, element, v->element_size);
  v->size++;

  return 0;
}

int vector_increase_size(vector* v) {
  size_t new_size = ((double) v->capacity / (double) v->element_size) * SIZE_MULTIPLICATOR * v->element_size;

  void *new_mem = realloc(v->data, new_size);
  if(!new_mem) {
    return -1;
  }

  v->data = new_mem;
  v->capacity = new_size;

  return 0;
}
