#include <stdio.h>
#include <string.h>
#include "../wumanber.h"
#include "test.c"

void test_wumanber_init(void);
void test_wumanber_search(void);

#define HBITS 4
#define TABLE_SIZE 32768

#define NPAT 2
/* #define NPAT 3 */

uint8_t *patterns[NPAT] = {
  "jour",
  "hui",
  /* "h", */
};



const char *text = "hello, bonjour, hola... aujourd’hui";

int main(void) {
  test_wumanber_init();
  test_wumanber_search();

  printf("wumanber_test success\n");
  return 0;
}

void test_wumanber_init(void) {
  uint8_t *patterns_[3] = {
    "jour",
    "bon",
    "llo"
  };
  size_t len_patterns[3] = {
    4,
    3,
    3
  };

  struct wumanber wm;
  struct wumamber_patterns pat;

  pat.n_pat = 3;
  pat.patterns = patterns_;
  pat.len_patterns = len_patterns;
  int ret = wumanber_init(&wm, &pat, HBITS, TABLE_SIZE);
  if (ret) {
    fail("wumanber_init() failed\n");
  }

  if (wm.hbits != HBITS
      || wm.table_size != TABLE_SIZE) {
    fail("wumanber_init() did not set the right values for struct members\n");
  }

  if (strcmp(*((char**) vector_get(&wm.other_patterns, 0)), patterns_[0])) {
      fail("wumanber_init() failed\n");
  } else if (strcmp(*((char**) vector_get(&wm.other_patterns, 1)), patterns_[1])) {
      fail("wumanber_init() failed\n");
  } else if (strcmp(*((char**) vector_get(&wm.other_patterns, 2)), patterns_[2])) {
      fail("wumanber_init() failed\n");
  }

  wumanber_free(&wm);
}

void test_wumanber_search (void) {
  uint8_t *patterns_[3] = {
    "jour",
    "bon",
    "llo"
  };
  size_t len_patterns[3] = {
    4,
    3,
    3
  };

  struct wumanber wm;
  struct wumamber_patterns pat;

  pat.n_pat = 3;
  pat.patterns = patterns_;
  pat.len_patterns = len_patterns;
  int ret = wumanber_init(&wm, &pat, HBITS, TABLE_SIZE);
  if (ret) {
    fail("wumanber_init() failed\n");
  }

  struct wumanber_matches *matches = wumanber_scan(&wm, text, strlen(text));

  printf("%lu matches\n", matches->size);

  size_t i;
  for (i = 0; i < matches->size; ++i) {
    struct wumanber_match *match = matches->matches + i;
    printf("(match \n");
    printf("  (pattern \"%s\")\n", match->pattern);
    printf("  (start \"%lu\")\n", match->start);
    printf(")\n");
  }

  wumanber_matches_free(matches);
  wumanber_free(&wm);
}
