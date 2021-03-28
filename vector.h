#ifndef _VECTOR_H
#define _VECTOR_H

#include <stddef.h>

/**
 * Don't forget to initialize this structure with vector_init() if you
 * are not using vector_new() to create the vector.
 **/
struct vector {
  void *data;
  size_t capacity, size, element_size;
};

/**
 * Create a new vector. Don't forget to call vector_free() and free()
 * on the returned pointer when you stop using it.
 **/
struct vector* vector_new(size_t capacity, size_t element_size);

/**
 * Initialize the vector when you allocate it on the stack.
 **/
int vector_init(struct vector *v, size_t capacity, size_t element_size);

void* vector_get(struct vector *v, size_t index);


/**
 * Free the memory allocated to the vector.
 **/
void vector_free(struct vector*);

/**
 * Add element at the end of the vector
 **/
int vector_push_back(struct vector *v, void *element);

#endif /* _VECTOR_H */
