
#ifndef FILEDISTANCE_DISTANCE_H
#define FILEDISTANCE_DISTANCE_H

typedef struct INSTRUCTIONS {
    char *operazione;
    unsigned int n;
    char b;
} INSTRUCTIONS;

void distance(char *file1, char *file2, char *output);

unsigned int onlyDistance(const char *file1, long dim1, const char *file2, long dim2);

unsigned int LevenshteinDistance(char *file1, long dim1, char *file2, long dim2, char *output);

void initializeMatrix(int **matrice, const char *file1, long dim1, const char *file2, long dim2);

void instructions(int **matrice, const char *file1, long dim1, char *file2, long dim2, INSTRUCTIONS *istruzioni);

unsigned int initialInstructions(int **matrice, const char *file1, long *dim1, char *file2, long *dim2, INSTRUCTIONS *istruzioni);

void finalInstructions(const char *file1, long *dim1, char *file2, long *dim2, INSTRUCTIONS *istruzioni, unsigned int cont);

void printInstructions(struct INSTRUCTIONS *istruzioni, unsigned int distance, char *output);

void createInstruction(struct INSTRUCTIONS *istruzioni, char *operazione, unsigned int n, char b);

void freeInstructions(INSTRUCTIONS *istruzioni, unsigned int size);

void freeMatrix(int **matrice, long dim1);

#endif //FILEDISTANCE_DISTANCE_H