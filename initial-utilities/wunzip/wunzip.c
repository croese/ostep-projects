#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("wunzip: file1 [file2 ...]\n");
        exit(1);
    }

    size_t nread;
    int count;
    char c;
    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "rb");
        if (fp == NULL) {
            printf("wunzip: cannot open file\n");
            exit(1);
        }

        while (nread = fread(&count, sizeof count, 1, fp), nread != 0) {
            nread = fread(&c, 1, 1, fp);
            if (nread == 0) {
                fclose(fp);
                printf("wunzip: corrupted file %s\n", argv[i]);
                exit(1);
            }

            for (int i = 0; i < count; i++) {
                printf("%c", c);
            }
        }

        if (ferror(fp) != 0) {
            fclose(fp);
            printf("wunzip: error reading file %s\n", argv[i]);
            exit(1);
        }

        fclose(fp);
    }
}
