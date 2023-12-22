#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_matches(FILE *fp, const char *term) {
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;

    while ((linelen = getline(&line, &linecap, fp)) > 0) {
        if (strstr(line, term) != NULL) {
            fwrite(line, linelen, 1, stdout);
        }
    }

    if (line != NULL) {
        free(line);
    }
}

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }

    char *searchterm = argv[1];

    if (argc == 2) {
        print_matches(stdin, searchterm);
    } else {
        for (int i = 2; i < argc; i++) {
            FILE *fp = fopen(argv[i], "r");
            if (fp == NULL) {
                printf("wgrep: cannot open file\n");
                exit(1);
            }

            print_matches(fp, searchterm);

            fclose(fp);
        }
    }
}
