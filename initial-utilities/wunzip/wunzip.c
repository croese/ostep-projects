#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  if (argc == 1) {
    printf("wunzip: file1 [file2 ...]\n");
    exit(1);
  }

  FILE* fp = NULL;
  int count = 0;
  char c = 0;

  for (int i = 1; i < argc; i++) {
    fp = fopen(argv[i], "r");
    if (!fp) {
      printf("wunzip: cannot open file\n");
      exit(1);
    }

    while (fread(&count, sizeof(int), 1, fp) == 1) {
      fread(&c, sizeof(char), 1, fp);
      for (int n = 0; n < count; n++) {
        putchar(c);
      }
    }

    fclose(fp);
  }

  return 0;
}