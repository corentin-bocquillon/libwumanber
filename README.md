# libwumanber

An implementation in C of the wu-manber algorithm described
[here](https://www.cs.arizona.edu/sites/default/files/TR94-17.pdf).

This library only works with patterns longer than 2.

## How to compile

`$ make`

## How to install

To install in /usr/local (the default) type:

`# make install`

To install to another directory, e.g. /usr type:

`# make PREFIX=/usr install`

To put the installed files in another directory, for example to create
a binary archive of the library, type:

`$ make DESTDIR=/path/to/directory install`

And you can combine `DESTDIR` with `PREFIX`:

`$ make PREFIX=/usr DESTDIR=/path/to/directory install`

## Run test suite

`$ make check`

## How to use

```c
#include <wumanber.h>

int main(void) {
  int hbits = 4;
  int table_size = 32768;

  struct wumamber_patterns pat;
  pat.n_pat = 3;
  uint8_t *patterns[pat.n_pat] = {
    "jour",
    "bon",
    "llo",
  };
  size_t len_patterns[pat.n_pat] = {
    4,
    3,
    3
  };
  pat.patterns = patterns;
  pat.len_patterns = len_patterns;

  const char *text = "hello, bonjour, hola... aujourd’hui";

  struct wumanber wm;
  int ret = wumanber_init(&wm, &pat, hbits, table_size);
  if (ret) {
    return 1;
  }

  struct wumanber_matches *m = wumanber_scan(&wm, text, strlen(text));
  for (int i = 0; i < m->size; ++i) {
    struct wumanber_match *match = m->matches + i;
    printf("(match \n");
    printf("  (pattern \"%s\")\n", match->pattern);
    printf("  (start \"%lu\")\n", match->start);
    printf(")\n");
  }

  wumanber_matches_free(m);
  wumanber_free(&wm);
}
```
