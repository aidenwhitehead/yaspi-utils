#define _POSIX_C_SOURCE 2
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "include/buffers.h"
#include "include/booleans.h"
#include "include/memory.h"
#include "include/files.h"

void head (char *file, int lines);

int main (int argc, char *argv[])
{
  int i, lines, opt;

  lines = 10;
  while ((opt = getopt(argc, argv, "n:")) != -1) {
    switch (opt) {
    case 'n':
      lines = atoi(optarg);
      break;
    case '?':
      exit(EXIT_FAILURE);
    }
  }

  if ((argc - optind) == 0)
    head("-", lines);
  else
    for (i = optind; i < argc; i++) {
      head(argv[i], lines);
    }
  
  return 0;
}

void head (char *file, int lines)
{
  char *buffer;
  int i, amount, newlines;
  FILE *fp;

  buffer = xmalloc(BUF_SIZE * sizeof(char));
  
  fp = open_file(file);
  if (fp == NULL)
    goto error;
  
  while ((amount = fread(buffer, sizeof(char), BUF_SIZE, fp)) > 0) {
    newlines = 0;
    for (i = 0; newlines < lines && i < amount; i++) {
      if (buffer[i] == '\n')
	newlines++;
      putchar(buffer[i]);
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
  return;

 error:
  fprintf(stderr, "head: %s: %s\n", file, strerror(errno));
  free(buffer);
  exit(EXIT_FAILURE);
}
