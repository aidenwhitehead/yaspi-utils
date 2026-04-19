#define _POSIX_C_SOURCE 2
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "booleans.h"

int main (int argc, char *argv[])
{
  unsigned char octal; /* For octal escape sequence, thus a char */
  int i, j, end_newline;

  end_newline = TRUE;
  for (i = 1; i < argc && end_newline; i++) {
    for (j = 0; argv[i][j] != '\0' && end_newline; j++) {
      if (argv[i][j] == '\\') {
	j++;
	switch (argv[i][j]) {
	case '\0':
	  putchar('\\');
	  break;
	case 'a':
	  putchar('\a');
	  break;
	case 'b':
	  putchar('\b');
	  break;
	case 'c':
	  end_newline = FALSE;
	  break;
	case 'f':
	  putchar('\f');
	  break;
	case 'n':
	  putchar('\n');
	  break;
	case 'r':
	  putchar('\r');
	  break;
	case 't':
	  putchar('\t');
	  break;
	case 'v':
	  putchar('\v');
	  break;
	case '\\':
	  putchar('\\');
	  break;
	case '0':
	  j++;
	  octal = 0;
	  while (('0' <= argv[i][j] && argv[i][j] <= '7') && \
		 octal < 040) { /* Must be less than 0377 */
	    octal = (octal << 3) | (argv[i][j] - '0');
	    j++;
	  }
	  putchar(octal);
	  j--;
	  break;
	default:
	  fprintf(stderr, "echo: '\\%c': Unrecognized escape sequence.",
		  argv[i][j]);
	  exit(EXIT_FAILURE);
	}
      } else {
	  putchar(argv[i][j]);
      }
    }
    if (i < argc - 1)
      putchar(' ');
  }
  if (end_newline)
    putchar('\n');
  
  return 0;
}
