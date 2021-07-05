
#ifndef FILEDISTANCE_SEARCH_H
#define FILEDISTANCE_SEARCH_H

#include <dirent.h>

typedef struct DISTANCE_PATH {
    unsigned int distance;
    char *path;
} DIST_PATH;

void search(char *inputfile, char *dir, int limit);

void searchAll(DIST_PATH *distancePath, int limit, int size);

void minimumDistance(DIST_PATH *distancePath, int size);

void visitInDepth(char *contentInput, char *name, int *size, DIST_PATH *dist_path);

void insertionSort(DIST_PATH *distance, int totDistances);

void printAll(DIST_PATH *pPath, int tot);

void freeFiles(char *file1, char *file2);

void freePaths(DIST_PATH *paths, int size);

#endif //FILEDISTANCE_SEARCH_H