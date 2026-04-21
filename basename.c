#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>

int main (int argc, char *argv[])
{
  char *string;
  int suf_len, str_len;
  
  if (argc < 2) {
    fprintf(stderr, "basename: missing operand\n");
    exit(EXIT_FAILURE);
  } else if (argc > 3) {
    fprintf(stderr, "basename: extra operand\n");
    exit(EXIT_FAILURE);
  }
  
  string = basename(argv[1]);
  
  if (argc == 3) {
    suf_len = strlen(argv[2]);
    str_len = strlen(string);
    if (str_len >= suf_len &&					\
	strcmp(&string[str_len - suf_len], argv[2]) == 0) {
      string[str_len - suf_len] = '\0';
    }
  }
    
  printf("%s\n", string);
  return 0;
}
