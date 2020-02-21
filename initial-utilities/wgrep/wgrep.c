#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_matches(const char* needle, FILE* stream) {
  char* line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  while ((linelen = getline(&line, &linecap, stream)) > 0) {
    if (strnstr(line, needle, linecap)) {
      // found a match
      printf("%s", line);
    }
  }
}

int main(int argc, char** argv) {
  if (argc == 1) {
    printf("wgrep: searchterm [file ...]\n");
    exit(1);
  }

  char* needle = argv[1];
  if (argc == 2) {
    print_matches(needle, stdin);
  } else {
    FILE* fp = NULL;
    for (int i = 2; i < argc; i++) {
      fp = fopen(argv[i], "r");

      if (!fp) {
        printf("wgrep: cannot open file\n");
        exit(1);
      }

      print_matches(needle, fp);

      fclose(fp);
    }
  }

  return 0;
}