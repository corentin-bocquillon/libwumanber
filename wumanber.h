#ifndef _WUMANBER_H
#define _WUMANBER_H

#include <stddef.h>
#include "wumanber_private.h"

typedef struct wumanber wumanber;

struct wumanber_matches {
  struct wumanber_match *matches;
  size_t size;
};

/**
 * Pre process the patterns 
 **/
int wumanber_init(struct wumanber *wm, char **patterns, size_t npat,
		  unsigned short hbits, size_t table_size);

/**
 * Search for patterns in text
 **/
struct wumanber_matches* wumanber_scan(struct wumanber *wm, const char *text);

void wumanber_matches_free(struct wumanber_matches*);

#endif	/* _WUMANBER_H */
