#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  if (argc == 1) {
    printf("wzip: file1 [file2 ...]\n");
    exit(1);
  }

  FILE* fp = NULL;
  char current = EOF;
  char last = current;
  int count = 0;

  for (int i = 1; i < argc; i++) {
    fp = fopen(argv[i], "r");
    if (!fp) {
      printf("wzip: cannot open file\n");
      exit(1);
    }

    while ((current = (char)fgetc(fp)) != EOF) {
      // fprintf(stderr, "read: '%c', last: '%c', count: %d\n", current, last,
      //        count);

      if (last == EOF) {
        last = current;
      }

      if (current == last) {
        count++;
        continue;
      } else {
        // fprintf(stderr, "writing %d '%c'\n", count, last);
        fwrite(&count, sizeof(int), 1, stdout);
        fwrite(&last, sizeof(char), 1, stdout);
        last = current;
        count = 1;
      }
    }

    if (ferror(fp)) {
      fclose(fp);
      printf("wzip: file read error");
      exit(1);
    }

    if (i == (argc - 1)) {
      // finished with last file
      // fprintf(stderr, "done: writing %d '%c'\n", count, last);
      fwrite(&count, sizeof(int), 1, stdout);
      fwrite(&last, sizeof(char), 1, stdout);
    }

    fclose(fp);
  }

  return 0;
}