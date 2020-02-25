#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char error_message[30] = "An error has occurred\n";
void print_error() {
  write(STDERR_FILENO, error_message, strlen(error_message));
}

void interactive();
void batch(FILE* file);

int main(int argc, char** argv) {
  if (argc == 1) {
    interactive();
  } else if (argc == 2) {
    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
      print_error();
      exit(1);
    }

    batch(fp);

    fclose(fp);
  } else {
    print_error();
    exit(1);
  }

  return 0;
}

void interactive() {
  printf("wish> ");

  char* line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  char* token = NULL;
  char* string = NULL;

  while ((linelen = getline(&line, &linecap, stdin)) > 0) {
    line[strcspn(line, "\n")] = 0;
    printf("READ: %s (%zd)\n", line, linelen);

    string = line;
    while ((token = strsep(&string, " ")) != NULL) {
      printf("TOKEN: '%s'\n", token);
    }

    printf("wish> ");
  }

  if (line) {
    free(line);
  }

  if (feof(stdin)) {
    exit(0);
  }

  if (ferror(stdin)) {
    exit(1);
  }
}

void batch(FILE* file) {}