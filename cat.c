#define _POSIX_C_SOURCE 2
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "buffers.h"
#include "booleans.h"
#include "memory.h"
#include "files.h"

#define OPT_U (1 << 0)

void concatenate (char *file, int options);

int main (int argc, char *argv[])
{
  int i, options, opt;
  
  options = 0;
  while ((opt = getopt(argc, argv, "u")) != -1) {
    switch (opt) {
    case 'u':
      options |= OPT_U;
      break;
    case '?':
      exit(EXIT_FAILURE);
    }
  }

  if ((argc - optind) == 0)
    concatenate("-", options);
  else
    for (i = optind; i < argc; i++) {
      concatenate(argv[i], options);
    }
  
  return 0;
}

void concatenate (char *file, int options)
{
  char *buffer;
  int amount, fd;
  FILE *fp;

  buffer = xmalloc(BUF_SIZE * sizeof(char));

  if (options == 0) { /* BUFFERED */ 
    fp = open_file(file);
    if (fp == NULL)
      goto error;
    
    while ((amount = fread(buffer, sizeof(char), BUF_SIZE, fp)) > 0)
      fwrite(buffer, sizeof(char), amount, stdout);
    
    if (ferror(fp) != 0) {
      if (fp != stdin)
	fclose(fp);
      goto error;
    }
    
    if (fp != stdin)
      fclose(fp);
    free(buffer);
    return;
  } else if (options == OPT_U) { /* UNBUFFERED */ 
    fd = (strcmp("-", file) == 0) ?		\
      STDIN_FILENO: open(file, O_RDONLY);
    if (fd < 0)
      goto error;

    while ((amount = read(fd, buffer, BUF_SIZE)) > 0)
      write(STDOUT_FILENO, buffer, amount);

    if (amount < 0) {
      if (fd != STDIN_FILENO)
	close(fd);
      goto error;
    }

    if (fd != STDIN_FILENO)
      close(fd);
    free(buffer);
    return;
  }
  
 error:
  fprintf(stderr, "cat: %s: %s\n", file, strerror(errno));
  free(buffer);
  exit(EXIT_FAILURE);
}
