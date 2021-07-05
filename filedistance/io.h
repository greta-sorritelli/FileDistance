
#ifndef FILEDISTANCE_IO_H
#define FILEDISTANCE_IO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "distance.h"
#include "apply.h"
#include "search.h"

char *readFile(char *path, long *sizeToReturn);

long readBinaryFile(char *buffer, FILE *file, long size);

long fileSize(FILE *file);

void getPath(char *absolutePath, char *file, char *name);

#endif //FILEDISTANCE_IO_H