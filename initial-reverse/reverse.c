#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int same_file(int fd1, int fd2) {
  struct stat stat1, stat2;
  if (fstat(fd1, &stat1) < 0)
    return -1;
  if (fstat(fd2, &stat2) < 0)
    return -1;
  return (stat1.st_dev == stat2.st_dev) && (stat1.st_ino == stat2.st_ino);
}

typedef struct _node {
  struct _node* next;
  struct _node* prev;
  char* line;
} node_t;

void free_list(node_t* list) {
  node_t* node = list;
  node_t* next = NULL;

  while (node) {
    next = node->next;
    free(node->line);
    free(node);
    node = next;
  }
}

void reverse(FILE* input, FILE* output) {
  char* line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  node_t* list = NULL;
  node_t* current = NULL;

  while ((linelen = getline(&line, &linecap, input)) > 0) {
    line[strcspn(line, "\n")] = 0;
    if (list == NULL) {
      list = malloc(sizeof(node_t));
      list->line = malloc(linelen + 1);
      strncpy(list->line, line, linelen);
      list->next = NULL;
      list->prev = NULL;
      current = list;
    } else {
      node_t* node = malloc(sizeof(node_t));
      node->line = malloc(linelen + 1);
      strncpy(node->line, line, linelen + 1);
      current->next = node;
      node->prev = current;
      current = node;
    }
  }

  free(line);

  if (current) {
    for (node_t* node = current; node; node = node->prev) {
      fprintf(output, "%s\n", node->line);
    }

    free_list(list);
  }
}

int main(int argc, char** argv) {
  if (argc == 1) {
    // in: stdin, out: stdout
    reverse(stdin, stdout);
  } else if (argc == 2) {
    // in: argv[1], out: stdout
    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
      fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
      exit(1);
    }

    reverse(fp, stdout);

    fclose(fp);
  } else if (argc == 3) {
    // in: argv[1], out: argv[2]
    FILE* input = fopen(argv[1], "r");
    if (!input) {
      fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
      exit(1);
    }

    FILE* output = fopen(argv[2], "w");
    if (!output) {
      fclose(input);
      fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
      exit(1);
    }

    if (same_file(fileno(input), fileno(output))) {
      fprintf(stderr, "reverse: input and output file must differ\n");
      exit(1);
    }

    reverse(input, output);

    fclose(input);
    fclose(output);
  } else {
    fprintf(stderr, "usage: reverse <input> <output>\n");
    exit(1);
  }

  return 0;
}