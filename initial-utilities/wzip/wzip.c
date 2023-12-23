#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }

    int current, last = -1;
    unsigned int count = 0;
    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            printf("wzip: cannot open file\n");
            exit(1);
        }

        while ((current = fgetc(fp)) != EOF) {
            if (current != last && last != -1) {
                fwrite(&count, sizeof count, 1, stdout);
                fwrite(&last, 1, 1, stdout);
                count = 1;
            } else {
                count++;
            }

            last = current;
        }

        fclose(fp);
    }

    if (last != -1) {
        fwrite(&count, sizeof count, 1, stdout);
        fwrite(&last, 1, 1, stdout);
    }
}
