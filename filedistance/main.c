
#include "io.h"

int main(int argc, char **argv) {
    if (argc == 4 || argc == 5) {
        if (strcmp(argv[1], "distance") == 0) {
            if (argc == 4) {
                distance(argv[2], argv[3], NULL);
                return 0;
            }
            else {
                distance(argv[2], argv[3], argv[4]);
                return 0;
            }
        }
        if (strcmp(argv[1], "apply") == 0) {
            apply(argv[2], argv[3], argv[4]);
            return 0;
        }
        if (strcmp(argv[1], "search") == 0) {
            search(argv[2], argv[3], -1);
            return 0;
        }
        if (strcmp(argv[1], "searchall") == 0) {
            search(argv[2], argv[3], strtol(argv[4], NULL, 10));
            return 0;
        }
    } else {
        printf("Parametri errati (darne 4 o 5, ne sono: %d)\n", argc);
        return -1;
    }
}