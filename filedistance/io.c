#include "io.h"

/**
 * Legge il file in binario, indipendentemente dal formato, tramite fread()
 * @param buffer    allocazione di partenza da cui leggere i dati
 * @param file      puntatore al file
 * @param size      numero di elementi da leggere
 * @return dimensione del file
 */
long readBinaryFile(char *buffer, FILE *file, long size) {
    return (long) fread(buffer, sizeof(char), size, file);
}

/**
 * Calcola la dimensione di un file
 *
 */
long fileSize(FILE *file) {
    fseek(file, 0, SEEK_END);
    long int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

/**
 * Legge il contenuto di un file in binario
 *
 */
char *readFile(char *path, long *sizeToReturn) {
    long size;
    FILE *file = NULL;
    char *buffer = NULL;
    if((file = fopen(path, "rb")) == NULL){
        printf("Error opening file!\n");
        exit(-1);
    }
    size = fileSize(file);
    buffer = malloc((size + 1) * sizeof(char));
    *sizeToReturn = readBinaryFile(buffer, file, size);
    fclose(file);
    return buffer;
}


/**
 * Costruisce una stringa come percorso assoluto di un file
 * @param absolutePath    stringa per salvare il percorso assoluto
 * @param file            path della directory
 * @param name            nome del file
 */
void getPath(char *absolutePath, char *file, char *name) {
    unsigned int n = strlen(file) - 1;
    strcpy(absolutePath, file);
    if (file[n] != '/')
        strcat(absolutePath, "/");
    strcat(absolutePath, name);
}