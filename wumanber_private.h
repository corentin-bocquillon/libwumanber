#ifndef _WUMANBER_PRIVATE_H
#define _WUMANBER_PRIVATE_H

#include <stddef.h>
#include <limits.h>

#include "vector.h"

#define BLOCK_SIZE 3
#define MATCHES_VECTOR_INITIAL_CAP 100

struct pattern_hash {
  unsigned hash;
  size_t index;
};

struct wumanber {
  /* Size of the shortest pattern longer than two characters */
  size_t m;

  /* Number of patterns longer than 2 */
  size_t k;

  /* number of bits to shift when hashing, the paper says it use 5 */
  unsigned short hbits;

  /* Shift and hash table size */
  size_t table_size;

  size_t *shift_table;
  struct vector other_patterns, *hash_prefix_table;
};

struct wumanber_match {
  /* A pointer to the matched pattern */
  char *pattern;

  /* Start of pattern in haystack */
  size_t start;
};

/**
 * Alloc every structure that has to be dynamically allocated in a
 * struct wumanber
 **/
int wumanber_alloc_mem(struct wumanber *wm, size_t npat);

/**
 * Free every structure that has been dynamically allocated
 **/
void wumanber_free_mem(struct wumanber *wm);

/**
 *
 **/
unsigned int
get_wumanber_table_hash_from_text(struct wumanber *wm, const char *text,
				  size_t cur_index);

unsigned int get_prefix_hash_from_text(struct wumanber *wm, const char *text,
				       size_t cur_index);

int check_short_pattern(struct wumanber *wm, const char *text,
			 size_t cur_index, struct vector *matches);

#endif /* _WUMANBER_PRIVATE_H */
