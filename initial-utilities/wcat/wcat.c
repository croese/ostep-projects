#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 512

int main(int argc, char** argv) {
  FILE* fp;
  char buffer[BUFSIZE];

  for (int i = 1; i < argc; i++) {
    fp = fopen(argv[i], "r");
    if (!fp) {
      printf("wcat: cannot open file\n");
      exit(1);
    }

    while (fgets(buffer, BUFSIZE, fp)) {
      printf("%s", buffer);
    }

    fclose(fp);
  }

  return 0;
}
