#include <stdio.h>
#include <string.h>

#include <vector.h>
#include "test.c"

void test_vector_new        (void);
void test_vector_init       (void);
void test_vector_push_back  (void);
void test_vector_free       (void);

typedef struct vector vector;

int
main (int argc, char *argv[]) {
  test_vector_new ();
  test_vector_init ();
  test_vector_push_back ();

  return 0;
}

void test_vector_new (void) {
  printf ("test_vector_new()\n");
  size_t cap = 100;
  vector *v = vector_new (cap, 1);
  if (!v) {
    fail ("vector_new returned NULL\n");
  } else if (v->size != 0) {
    fail ("v->size (%d) != 0\n", v->size);
  } else if (v->capacity != cap) {
    fail ("v->capacity (%d) != cap (%d)", v->capacity, cap);
  } else if (v->element_size != 1) {
    fail ("v->element_size (%d) != 1", v->element_size);
  }
  vector_free (v);
}

void test_vector_init (void) {
  printf ("test_vector_init()\n");
  vector v;
  int cap = 33;
  int ret = vector_init (&v, cap, 1);
  if (ret) {
    fail ("vector_init() failed\n");
  } else if (v.capacity != cap) {
    fail ("v.capacity (%d) != %d\n", v.capacity, cap);
  } else if (v.size != 0) {
    fail ("v.size (%d) != 0\n", v.size);
  } else if (v.element_size != 1) {
    fail ("v.element_size (%d) != 1\n", v.element_size);
  }
  free (v.data);
  v.data = NULL;

  cap = 0;
  ret = vector_init (&v, cap, 1);
  if (!ret) {
    fail ("vector_init(%d) should have failed.\n", cap);
  }
  free (v.data);
  v.data = NULL;

  cap = -1;
  ret = vector_init (&v, cap, 1);
  if (!ret) {
    fail ("vector_init(%d) should have failed.\n", cap);
  }
  free (v.data);
}

void test_vector_push_back (void) {
  printf ("test_vector_push_back()\n");
  vector *v = vector_new (100, sizeof (char**));

  const char *test_string1 = "abc";
  int ret = vector_push_back (v, &test_string1);
  if (ret || v->size != 1) {
    fail ("vector_push_back() failed\n");
  }

  const char *test_string2 = "de";
  ret = vector_push_back (v, &test_string2);
  if (ret || v->size != 2) {
    fail("vector_push_back() failed\n");
  }

  if (*((char**) vector_get(v, 0)) != test_string1) {
    fail("vector_push_back() failed\n");
  } else if (*((char**) vector_get(v,1)) != test_string2) {
    fail("vector_push_back() failed\n");
  }

  vector_free (v);
}
