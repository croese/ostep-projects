#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 512

void print_file(const char *fname) {
    FILE *fp = fopen(fname, "r");
    if (fp == NULL) {
        printf("wcat: cannot open file\n");
        exit(1);
    }

    char buffer[BUF_SIZE];

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }

    if (ferror(fp)) {
        perror("wcat");
    }

    fclose(fp);
}

int main(int argc, char **argv) {
    (void)argv;

    if (argc < 2) {
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        print_file(argv[i]);
    }
}
