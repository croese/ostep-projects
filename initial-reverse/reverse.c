#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

struct node {
    char *str;
    struct node *next;
};

void free_list(struct node *list) {
    struct node *current = list, *temp;
    while (current != NULL) {
        temp = current;
        current = current->next;

        if (temp->str != NULL) {
            free(temp->str);
        }

        free(temp);
    }
}

int main(int argc, char **argv) {
    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    struct stat insb, outsb;

    if (argc == 3) {
        if (strcmp(argv[1], argv[2]) == 0) {
            fprintf(stderr, "reverse: input and output file must differ\n");
            exit(1);
        }

        if (stat(argv[1], &insb) == -1) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(1);
        }

        if (stat(argv[2], &outsb) == -1) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
            exit(1);
        }

        if (insb.st_ino == outsb.st_ino) {
            fprintf(stderr, "reverse: input and output file must differ\n");
            exit(1);
        }
    }

    FILE *infp = stdin, *outfp = stdout;

    if (argc > 1) {
        infp = fopen(argv[1], "r");
        if (infp == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
    }

    if (argc == 3) {
        outfp = fopen(argv[2], "w");
        if (outfp == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
            fclose(infp);
            exit(1);
        }
    }

    struct node *list = NULL, *current = NULL;

    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while ((linelen = getline(&line, &linecap, infp)) > 0) {

        current = malloc(sizeof(struct node));
        if (current == NULL) {
            free(line);
            free_list(list);
            fprintf(stderr, "reverse: malloc failed\n");
            exit(1);
        }

        current->next = NULL;

        current->str = strndup(line, linelen - 1);
        if (current->str == NULL) {
            free(line);
            free_list(list);
            fprintf(stderr, "reverse: malloc failed\n");
            exit(1);
        }

        if (list == NULL) {
            list = current;
        } else {
            current->next = list;
            list = current;
        }
    }

    for (current = list; current != NULL; current = current->next) {
        fprintf(outfp, "%s\n", current->str);
    }

    if (line != NULL) {
        free(line);
    }

    free_list(list);
}
