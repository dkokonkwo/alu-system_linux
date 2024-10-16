#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [object_file]\n", argv[0]);
        return 1;
    }

    char command[256];
    snprintf(command, sizeof(command), "nm -p %s 2>&1", argv[1]);
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: failed to execute nm command.\n");
        return 1;
    }

    char line[256];
    int found_symbols = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strstr(line, "no symbols") != NULL) {
            fprintf(stderr, "%s: %s: no symbols\n.", argv[0], argv[1]);
            found_symbols = 1;
        } else {
            printf("%s", line);
        }
    }

    pclose(fp);
    return found_symbols ? 1 : 0;
}
