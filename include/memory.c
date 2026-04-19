#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

void *xmalloc (size_t size)
{
  void *ptr;

  ptr = malloc(size);
  if (ptr == NULL) {
    fprintf(stderr, "Not enough memory!\n");
    exit(EXIT_FAILURE);
  }
  
  return ptr;
}

void *xcalloc (size_t nelem, size_t elsize)
{
  void *ptr;

  ptr = calloc(nelem, elsize);
  if (ptr == NULL) {
    fprintf(stderr, "Not enough memory!\n");
    exit(EXIT_FAILURE);
  }
  
  return ptr;
}
