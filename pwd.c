#define _POSIX_C_SOURCE 2
#include <sys/types.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "buffers.h"
#include "booleans.h"
#include "memory.h"

#define OPT_L_UP (1 << 0)
#define OPT_P_UP (1 << 1)

bool check_valid (char *path);
void print_phys_path (void);

int main (int argc, char *argv[])
{
  char *path;
  int options, opt;
  bool valid;

  options = OPT_L_UP;
  while ((opt = getopt(argc, argv, "LP")) != -1) {
    switch (opt) {
    case 'L':
      options = OPT_L_UP;
      break;
    case 'P':
      options = OPT_P_UP;
      break;
    case '?':
      exit(EXIT_FAILURE);
    }
  }

  valid = TRUE;
  if (options == OPT_L_UP) {
    path = getenv("PWD");
    valid = check_valid(path);
    
    if (valid)
      printf("%s\n", path);
    else
      options = OPT_P_UP;
  }

  if (options == OPT_P_UP)
    print_phys_path();

  exit(EXIT_SUCCESS);
  
  return 0;
}

bool check_valid (char *path)
{
  int len;

  if (path == NULL || path[0] != '/')
    return FALSE;

  if (strstr(path, "/./") != NULL)
    return FALSE;
  if (strstr(path, "/../") != NULL)
    return FALSE;

  len = strlen(path);
  if (len >= 2 && strcmp(path + len - 2, "/.") == 0)
    return FALSE;
  if (len >= 3 && strcmp(path + len - 3, "/..") == 0)
    return FALSE;

  return TRUE;
}

void print_phys_path (void)
{
  char buffer[PATH_MAX];
  
  if (getcwd(buffer, sizeof(buffer)) == NULL) {
    fprintf(stderr, "pwd: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  printf("%s\n", buffer);
}
