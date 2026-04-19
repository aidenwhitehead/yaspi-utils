#ifndef FILES_H
#define FILES_H

FILE *open_file(const char *path) {
  return strcmp(path, "-") == 0 ?		\
    stdin : fopen(path, "r");
}

#endif
