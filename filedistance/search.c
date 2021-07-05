
#include "io.h"

/**
 * Cerca i file nella directory e calcola le dimensioni dal file in input,
 * se non viene inserito un limite ritorna il file con la distanza minima,
 * se viene inserito vengono ritornati tutti i file con distanza entro il limite
 * @param inputfile     file in input da confrontare
 * @param dir           directory contenente i file da confrontare con il primo
 * @param limit         limite di distanza di edit
 */
void search(char *inputfile, char *dir, int limit) {
    int size = 0;
    DIST_PATH *dist_path = NULL;
    visitInDepth(NULL, dir, &size, NULL);
    dist_path = malloc(size * sizeof(DIST_PATH));
    for (int i = 0; i < size; i++) {
        dist_path[i].path = malloc(1024 * sizeof(char));
    }
    size = 0;
    visitInDepth(inputfile, dir, &size, dist_path);
    if (limit >= 0)
        searchAll(dist_path, limit, size);
    else
        minimumDistance(dist_path, size);
    freePaths(dist_path, size);
}

/**
 * Ritorna tutti i file entro il limite di distanza
 * @param distancePath      struttura contenente i path e le relative distanze dal file in input
 * @param limit             limite di distanza di edit
 * @param size              dimensione della struttura contenente i path
 */
void searchAll(DIST_PATH *distancePath, int limit, int size) {
    DIST_PATH *tempStruct = NULL;
    register int i, k;
    int count = 0;
    for (i = 0; i < size; i++) {
        if (distancePath[i].distance <= limit) {
            count++;
        }
    }
    tempStruct = malloc(count * sizeof(DIST_PATH));
    for (i = 0; i < count; i++) {
        tempStruct[i].path = malloc(1024 * sizeof(char));
    }
    i = 0, k = 0;

    while (k < count) {
        if (distancePath[i].distance <= limit) {
            tempStruct[k] = distancePath[i];
            k++;
        }
        i++;
    }
    printf("SEARCH ALL WITHIN THE LIMIT %d\n", limit);
    insertionSort(tempStruct, count);
    if (tempStruct)
        freePaths(tempStruct, count);
}

/**
 * Ritorna solo i file con distanza minima dal file di input
 *
 */
void minimumDistance(DIST_PATH *distancePath, int size) {
    unsigned int min = INT_MAX;
    register int i;
    for (i = 0; i < size; i++) {
        if (distancePath[i].distance < min)
            min = distancePath[i].distance;
    }
    printf("SEARCH MINIMUM DISTANCE:\n");
    for (i = 0; i < size; i++) {
        if (distancePath[i].distance == min)
            printf("%s\n", distancePath[i].path);
    }
}

/**
 * Libera la memoria allocata per le strutture
 *
 */
void freePaths(DIST_PATH *distPath, int size) {
    for (register int i = 0; i < size; i++) {
        if (distPath[i].path)
            distPath[i].path = NULL;
    }
    free(distPath), distPath = NULL;
}

/**
 * Ritorna i file con distanza entro il limite in ordine crescente, secondo
 * l'algoritmo di ordinamento di Insertion Sort:
 * L’algoritmo ha bisogno di due indici, uno che punta all’elemento da ordinare e l’altro a quello immediatamente precedente.
 * Il passo successivo consiste nel confrontare i due elementi, se l’elemento puntato dal secondo indice
 * è maggiore del secondo gli elementi allora vengono scambiati.
 *
 */
void insertionSort(DIST_PATH *distance, int totDistances) {
    for (size_t i = 1; i < totDistances; ++i) {
        DIST_PATH tmp = distance[i];
        size_t j = i;
        while (j > 0 && tmp.distance < distance[j - 1].distance) {
            distance[j] = distance[j - 1];
            --j;
        }
        distance[j] = tmp;
    }
    printAll(distance, totDistances);
}

/**
 * Stampa tutti i file ritornati da Search All e ordinati con Insertion Sort
 *
 */
void printAll(DIST_PATH *pPath, int tot) {
    for (int i = 0; i < tot; i++)
        printf("Distance: %d Path: %s\n", pPath[i].distance, pPath[i].path);
}

/**
 * Visita in profondità la directory e le sottodirectory per calcolare le distanze tra i file presenti
 * e il file dato in input
 * @param inputfile     file in input
 * @param name          nome della directory
 * @param size          numero di file trovati
 * @param dist_path     struttura dei paths
 */
void visitInDepth(char *inputfile, char *name, int *size, DIST_PATH *dist_path) {
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    if (!(dir = opendir(name)))
        return;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            //1024 dim max per path in windows
            char path[1024];
            //	continua se l'entry corrente è la directory corrente (.) o la directory sopra a questa(..)
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            visitInDepth(NULL, path, size, dist_path);
        } else {
            if (dist_path != NULL) {
                long sizeInput;
                char *contentInput = NULL;
                if ((contentInput = readFile(inputfile, &sizeInput)) != NULL) {
                    char absolutePath[1024];
                    getPath(absolutePath, name, entry->d_name);
                    strcpy(dist_path[(*size)].path, absolutePath);
                    long sizeFile2 = 0;
                    char *file2 = readFile(dist_path[*size].path, &sizeFile2);
                    dist_path[(*size)++].distance = onlyDistance(contentInput, sizeInput, file2, sizeFile2);
                    freeFiles(contentInput, file2);
                }
            } else {
                (*size)++;
            }
        }
    }
    closedir(dir);
}

/**
 * Libera la memoria occupata dai contenuti dei file
 *
 */
void freeFiles(char *file1, char *file2) {
    free(file1), file1 = NULL;
    free(file2), file2 = NULL;
}