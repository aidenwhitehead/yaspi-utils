#define _POSIX_C_SOURCE 2
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "buffers.h"
#include "booleans.h"
#include "memory.h"
#include "files.h"

#define BOTTOM 0
#define TOP 1

void tail (char *file, int num_lines, int from);
void free_lines (char **lines, int num_lines);

int main (int argc, char *argv[])
{
  int i, lines, opt, from;

  lines = 10;
  from = BOTTOM;
  while ((opt = getopt(argc, argv, "n:")) != -1) {
    switch (opt) {
    case 'n':
      if (*optarg == '+')
	from = TOP;
      lines = abs(atoi(optarg));
      break;
    case '?':
      exit(EXIT_FAILURE);
    }
  }

  if ((argc - optind) == 0)
    tail("-", lines, from);
  else
    for (i = optind; i < argc; i++) {
      tail(argv[i], lines, from);
    }
  
  return 0;
}

void tail (char *file, int num_lines, int from)
{
  char *buffer, **lines;
  int i, amount, newlines, line_len, idx;
  FILE *fp;

  lines = xcalloc(num_lines, sizeof(char *));
  buffer = xmalloc(BUF_SIZE * sizeof(char));
  
  fp = open_file(file);
  if (fp == NULL)
    goto error;

  if (from == BOTTOM) { /* FROM THE BOTTOM */
    while ((amount = fread(buffer, sizeof(char), BUF_SIZE, fp)) > 0) {
      newlines = 0;
      line_len = 0;
      for (i = 0; i < amount; i++) {
	if (buffer[i] == '\n') {
	  newlines++;
	  
	  idx = newlines % num_lines;
	  free(lines[idx]);

	  lines[idx] = xmalloc(line_len * sizeof(char) + 1);
	  memcpy(lines[idx], &buffer[i - line_len],	\
		 line_len * sizeof(char));
	  lines[idx][line_len] = '\0';

	  line_len = 0;
	} else {
	  line_len++;
	}
      }
    }
    
    idx = (newlines + 1) % num_lines;
    for (i = idx; i < num_lines + idx; i++)
      printf("%s\n", lines[i % num_lines]);
  } else if (from == TOP) { /* FROM THE TOP */
    while ((amount = fread(buffer, sizeof(char), BUF_SIZE, fp)) > 0) {
      newlines = 0;
      for (i = 0; i < amount; i++) {	
	if (newlines + 1 >= num_lines) /* Also print line before nl */
	  putchar(buffer[i]);
	if (buffer[i] == '\n')
	  newlines++;
      }
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
  free_lines(lines, num_lines);
  return;

 error:
  fprintf(stderr, "tail: %s: %s\n", file, strerror(errno));
  free(buffer);
  free_lines(lines, num_lines);
  exit(EXIT_FAILURE);
}

void free_lines (char **lines, int num_lines)
{
  int i;

  for (i = 0; i < num_lines; i++)
    free(lines[i]);
  free(lines);
}
