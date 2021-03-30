#ifndef _WUMANBER_H
#define _WUMANBER_H

#include <stddef.h>
#include <stdint.h>
#include "wumanber_private.h"

typedef struct wumanber wumanber;

struct wumanber_matches {
  struct wumanber_match *matches;
  size_t size;
};

struct wumamber_patterns {
  uint8_t ** patterns;
  size_t * len_patterns;
  size_t n_pat;
};

/**
 * Pre process the patterns 
 **/
int wumanber_init(struct wumanber *wm,  struct wumamber_patterns * patterns,
		  unsigned short hbits, size_t table_size);

/**
 * Search for patterns in text
 **/
struct wumanber_matches* wumanber_scan(struct wumanber *wm, const uint8_t *text, size_t n_char);

void wumanber_matches_free(struct wumanber_matches*);

#endif	/* _WUMANBER_H */
