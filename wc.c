#define _POSIX_C_SOURCE 2
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "include/buffers.h"
#include "include/booleans.h"
#include "include/memory.h"
#include "include/files.h"


/* Because this only supports ASCII, bytes = chars. */
#define OPT_C (1 << 0) /* Write bytes */
#define OPT_L (1 << 1) /* Write newlines */
#define OPT_M (1 << 2) /* Write chars */
#define OPT_W (1 << 3) /* Write words */

struct wc_count {
  int newlines;
  int words;
  int bytes;
  int chars;
};

struct wc_count count_get (char *file);
void count_print (struct wc_count count, char *name, int options);
struct wc_count count_sum (struct wc_count a, struct wc_count b);

int main (int argc, char *argv[])
{
  int i, options, opt;
  struct wc_count count_cur, count_total;

  options = 0;
  while ((opt = getopt(argc, argv, "clmw")) != -1) {
    switch (opt) {
    case 'c':
      options |= OPT_C;
      options &= (~OPT_M); /* Mask away '-m', if present. */
      break;
    case 'l':
      options |= OPT_L;
      break;
    case 'm':
      options |= OPT_M;
      options &= (~OPT_C); /* Mask away '-c', if present. */
      break;
    case 'w':
      options |= OPT_W;
      break;
    case '?':
      exit(EXIT_FAILURE);
    }
  }

  /* Set default to be equivalent to -clw */
  if (options == 0)
    options |= (OPT_C | OPT_L | OPT_W);

  if ((argc - optind) == 0) {
    count_cur = count_get("-");
    count_print(count_cur, "", options);
  } else {
    for (i = optind; i < argc; i++) {
      count_cur = count_get(argv[i]);
      count_print(count_cur, argv[i], options);
      
      if (i == optind)
	count_total = count_cur;
      else
	count_total = count_sum(count_total, count_cur);
    }
    
    if ((argc - optind) > 1)
      count_print(count_total, "total", options);
  }    
      
  return 0;
}

struct wc_count count_get (char *file)
{
  char *buffer;
  FILE *fp;
  int i, amount, in_word;
  struct wc_count count = {0};

  buffer = xmalloc(BUF_SIZE * sizeof(char));
  
  fp = open_file(file);
  if (fp == NULL)
    goto error;
    
  in_word = FALSE;
  while ((amount = fread(buffer, sizeof(char), BUF_SIZE, fp)) > 0) {
    for (i = 0; i < amount; i++) {
      /* NEWLINES */
      if (buffer[i] == '\n')
	count.newlines++;
      /* WORDS */
      if (isspace((unsigned char) buffer[i])) {
	in_word = FALSE;
      } else if (!in_word) {
	in_word = TRUE;
	count.words++;
      }
      /* BYTES */
      count.bytes++;
      /* CHARACTERS */
      count.chars++;
    }
  }
    
  if (ferror(fp) != 0) {
    if (fp != stdin)
      fclose(fp);
    goto error;
  }

  if (fp != stdin)
    fclose(fp);
  free(buffer);
  return count;
  
 error:
  fprintf(stderr, "wc: %s: %s\n", file, strerror(errno));
  free(buffer);
  exit(EXIT_FAILURE);
}

void count_print (struct wc_count count, char *name, int options)
{
  if (options & OPT_L)
    printf("%d ", count.newlines);
  if (options & OPT_W)
    printf("%d ", count.words);
  if (options & OPT_C)
    printf("%d ", count.bytes);
  if (options & OPT_M)
    printf("%d ", count.chars);
  
  printf("%s\n", name);
}

struct wc_count count_sum (struct wc_count a, struct wc_count b)
{
  struct wc_count sum;
  
  sum.newlines = a.newlines + b.newlines;
  sum.words = a.words + b.words;
  sum.bytes = a.bytes + b.bytes;
  sum.chars = a.chars + b.chars;

  return sum;
}
