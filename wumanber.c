#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "wumanber.h"

int wumanber_init(struct wumanber *wm, char **patterns, size_t npat,
		  unsigned short hbits, size_t table_size) {
  wm->m = 0;
  wm->table_size = table_size;
  wm->hbits = hbits;

  int ret = wumanber_alloc_mem(wm, npat);
  if (ret) {
    return ret;
  }

  size_t i;
  for (i = 0; i < npat; ++i) {
    size_t pat_len = strlen(patterns[i]);
    if (pat_len < BLOCK_SIZE) {
      wumanber_free_mem(wm);
      return -1;
    }

    if (!wm->m || pat_len < wm->m) {
      wm->m = pat_len;
    }

    ret = vector_push_back(&wm->other_patterns, patterns + i);
    if (ret) {
      wumanber_free_mem(wm);
      return ret;
    }
  }
  wm->k = wm->other_patterns.size;

  /* fill default value for SHIFT table */
  for (i = 0; i < wm->table_size; ++i) {
    wm->shift_table[i] = wm->m - BLOCK_SIZE + 1;
  }

  /* fill HASH/PREFIX and SHIFT tables */
  size_t j;
  for (i = 0; i < wm->k; ++i) {
    for (j = wm->m; j >= BLOCK_SIZE; --j) {
      unsigned hash_value = get_wumanber_table_hash_from_text
	(wm, *((char**) vector_get(&wm->other_patterns, i)), j - 1);

      size_t shift_length = wm->m - j;

      if (wm->shift_table[hash_value] > shift_length) {
	wm->shift_table[hash_value] = shift_length;
      }
      
      if (!shift_length) {
	struct pattern_hash pattern_hash_to_add;
	pattern_hash_to_add.index = i;

	/* calculate this prefix_hash to help us skip some patterns if there are collisions in hashPrefixTable_ */
	pattern_hash_to_add.hash = get_prefix_hash_from_text(wm, *((char**) vector_get(&wm->other_patterns, i)), 0);
	ret = vector_push_back(&wm->hash_prefix_table[hash_value],
			       &pattern_hash_to_add);
	if (ret) {
	  wumanber_free_mem(wm);
	  return ret;
	}
      }
    }
  }

  return 0;
}

struct wumanber_matches* wumanber_scan(struct wumanber *wm, const char *text) {
  size_t text_length = strlen(text);
  if (text_length == 0) {
    return NULL;
  }

  struct wumanber_matches *returned_matches
    = malloc(sizeof (struct wumanber_matches));
  if (!returned_matches) {
    return NULL;
  }

  returned_matches->matches = NULL;
  struct vector *matches = vector_new(MATCHES_VECTOR_INITIAL_CAP,
				      sizeof (struct wumanber_match));
  if (!matches) {
    free(returned_matches);
    return NULL;
  }

  size_t index = wm->m - 1;
  while (index < text_length) {
    /* hash value for HASH table */
    unsigned int hash_value = get_wumanber_table_hash_from_text(wm, text, index);

    size_t shift_length = wm->shift_table[hash_value];
    if (shift_length == 0) {
      /* found a potential match, check values in HASH/PREDIX and will shift by 1 character */
      shift_length = 1;

      /* hash value to match pattern */
      unsigned int prefix_hash = get_prefix_hash_from_text(wm, text, index - wm->m + 1);
      /* for (const auto &potentialMatch : hashPrefixTable_[hash_value]) { */
      struct vector *potential_pat = &wm->hash_prefix_table[hash_value];
      size_t npotential_match = potential_pat->size;

      size_t i;
      for (i = 0; i < npotential_match; ++i) {
	struct pattern_hash *potential_match = vector_get(potential_pat, i);
	if (prefix_hash == potential_match->hash) {
	  char *pattern = *((char**) vector_get(&wm->other_patterns, potential_match->index));
	  size_t index_in_pattern = 0;
	  size_t index_in_text = index - wm->m + 1;

	  /* prefix hash matched so we try to match character by character */
	  size_t pattern_length = strlen(pattern);
	  while(index_in_pattern < pattern_length
		&& index_in_text < text_length
		&& pattern[index_in_pattern++] == text[index_in_text++]);

	  /* end of pattern reached => match found */
	  if (index_in_pattern == pattern_length) {
	    /* Match */
	    struct wumanber_match wmmatch;
	    wmmatch.pattern = pattern;
	    wmmatch.start = index - wm->m + 1;
	    int ret = vector_push_back(matches, &wmmatch);
	    if (ret) {
	      free(returned_matches);
	      vector_free(matches);
	      return NULL;
	    }
	  }
	}
      }
    }
    index += shift_length;
  }

  returned_matches->matches = matches->data;
  returned_matches->size = matches->size;

  /* Return the match */
  return returned_matches;
}

int wumanber_alloc_mem(struct wumanber *wm, size_t npat) {
  int ret = vector_init(&wm->other_patterns, npat, sizeof (char**));
  if (ret) {
    return ret;
  }

  wm->shift_table = malloc(sizeof (size_t) * wm->table_size);
  if (!wm->shift_table) {
    vector_free(&wm->other_patterns);
    return ret;
  }

  wm->hash_prefix_table = malloc(sizeof (struct vector) * wm->table_size);
  if (!wm->hash_prefix_table) {
    vector_free(&wm->other_patterns);
    free(wm->shift_table);
    return ret;
  }

  int error = -1;
  size_t i;
  for (i = 0; i < wm->table_size; ++i) {
    ret = vector_init(&wm->hash_prefix_table[i], npat, sizeof (struct pattern_hash));
    if (ret) {
      error = i;
      free(wm->shift_table);
      vector_free(&wm->other_patterns);
    }
  }

  if (error != -1) {
    for (i = 0; i < error; ++i) {
      vector_free(&wm->hash_prefix_table[i]);
    }
  }

  return 0;
}

void wumanber_free_mem(struct wumanber *wm) {
  vector_free(&wm->other_patterns);
  free(wm->shift_table);

  size_t i;
  for (i = 0; i < wm->table_size; ++i) {
    vector_free(&wm->hash_prefix_table[i]);
  }

  free(wm->hash_prefix_table);
}

unsigned int
get_wumanber_table_hash_from_text(struct wumanber *wm, const char *text, size_t cur_index) {
  unsigned int hash_value;
  hash_value = (size_t) text[cur_index];
  hash_value <<= wm->hbits;
  hash_value += (size_t) text[cur_index - 1];
  hash_value <<= wm->hbits;
  hash_value += (size_t) text[cur_index - 2];
  hash_value %= wm->table_size; /* may try this for faster mod: https://www.youtube.com/watch?v=nXaxk27zwlk&feature=youtu.be&t=56m34s */
  return hash_value;
}

unsigned int get_prefix_hash_from_text(struct wumanber *wm, const char *text,
				       size_t cur_index)  {
  unsigned int prefix_hash;
  prefix_hash = text[cur_index];
  prefix_hash <<= wm->hbits;
  prefix_hash += text[cur_index + 1];
  return prefix_hash;
}

void wumanber_matches_free(struct wumanber_matches *m) {
  free(m->matches);
  free(m);
}
