
#include "io.h"

/**
 * macro per il calcolo del minimo tra due numeri
 */
#define MIN2(a, b) ((a) <= (b)  ? (a) : (b) )
/**
 * macro per il calcolo del minimo tra tre numeri
 */
#define MIN3(a, b, c) (MIN2(MIN2(a,b),c))

/**
 * Distanza tra due file, se esiste l'output stampa le istruzioni, altrimenti solo la distanza e il tempo di calcolo.
 * @param file1     primo file in input
 * @param file2     secondo file in input
 * @param output    file di output per le istruzioni
 */
void distance(char *file1, char *file2, char *output) {
    char *contenutoFile1 = NULL, *contenutoFile2 = NULL;
    long dimIniziale1 = 0, dimIniziale2 = 0;
    contenutoFile1 = readFile(file1, &dimIniziale1);
    contenutoFile2 = readFile(file2, &dimIniziale2);
    if (output == NULL) {
        clock_t clock1 = clock();
        unsigned int dist = onlyDistance(contenutoFile1, dimIniziale1, contenutoFile2, dimIniziale2);
        clock_t clock2 = clock();
        double time = (double) (clock2 - clock1) / CLOCKS_PER_SEC;
        printf("EDIT DISTANCE: %d\nTIME: %3f\n", dist, time);
    } else {
        FILE *f = NULL;
        if ((f = fopen(output, "wb")) == NULL) {
            printf("Error opening file!\n");
            exit(-1);
        }
        fprintf(f, "");
        fclose(f);
        LevenshteinDistance(contenutoFile1, dimIniziale1, contenutoFile2, dimIniziale2, output);
    }
    free(contenutoFile1), contenutoFile1 = NULL;
    free(contenutoFile2), contenutoFile2 = NULL;
}

/**
 * Ritorna solo la distanza utilizzando l'algoritmo di Levenshtein ottimizzato.
 *
 */
unsigned int onlyDistance(const char *file1, long dim1, const char *file2, long dim2) {
    register int i, j;
    int distance;
    int *prev = malloc((dim2 + 1) * sizeof(int));
    int *curr = malloc((dim2 + 1) * sizeof(int));
    int *tmp;
    for (i = 0; i <= dim2; i++)
        prev[i] = i;
    for (i = 1; i <= dim1; i++) {
        curr[0] = i;
        for (j = 1; j <= dim2; j++) {
            if (file1[i - 1] != file2[j - 1]) {
                int k = MIN3(curr[j - 1], prev[j - 1], prev[j]);
                curr[j] = k + 1;
            } else {
                curr[j] = prev[j - 1];
            }
        }
        tmp = prev;
        prev = curr;
        curr = tmp;
        memset((void *) curr, 0, sizeof(int) * (dim2 + 1));
    }
    distance = prev[dim2];
    prev = NULL;
    free(curr), curr = NULL;
    free(prev);
    return distance;
}

/**
 * Algoritmo di Levenshtein: calcola la distanza di edit, è una misura per la differenza fra due stringhe.
 * Serve a determinare quanto due stringhe siano simili. La distanza è il numero minimo di modifiche elementari
 * che consentono di trasformare la prima stringa nella seconda. Per modifica elementare si intende:
 * la cancellazione di un carattere, la sostituzione di un carattere con un altro, o l'inserimento di un carattere.
 * @param file1         primo file di input
 * @param dim1          dimensione del primo file in input
 * @param file2         secondo file in input
 * @param dim2          dimensione del secondo file in input
 * @param output        file di output
 * @return la distanza di edit
 */
unsigned int LevenshteinDistance(char *file1, long dim1, char *file2, long dim2, char *output) {
    unsigned int distance;
    int **matrice = (int **) malloc((dim1 + 1) * sizeof(int *));
    for (register int i = 0; i <= dim1; i++)
        matrice[i] = (int *) malloc((dim2 + 1) * sizeof(int));
    initializeMatrix(matrice, file1, dim1, file2, dim2);
    distance = matrice[dim1][dim2];
    if (output != NULL) {
        struct INSTRUCTIONS *istruzioni = malloc(distance * sizeof(struct INSTRUCTIONS));
        for (register int i = 0; i < distance; i++) {
            istruzioni[i].operazione = malloc(4 * sizeof(char));
            istruzioni[i].n = 0;
            istruzioni[i].b = '\0';
        }
        instructions(matrice, file1, dim1, file2, dim2, istruzioni);
        printInstructions(istruzioni, distance, output);
        freeInstructions(istruzioni, distance);
    }
    freeMatrix(matrice, dim1);
    return distance;
}

/**
 * Inizializzazione della matrice di Levenshtein.
 * @param matrice      matrice costituita dai caratteri del primo file e del secondo file
 * @param file1        il contenuto del primo file rappresenta le righe
 * @param dim1         numero di righe della matrice
 * @param file2        il contenuto del secondo file rappresenta le colonne
 * @param dim2         numero di colonne della matrice
 */
void initializeMatrix(int **matrice, const char *file1, long dim1, const char *file2, long dim2) {
    register int i, j;

    for (i = 0; i <= dim1; i++)
        matrice[i][0] = i;

    for (j = 1; j <= dim2; j++)
        matrice[0][j] = j;

    for (i = 1; i <= dim1; i++)
        for (j = 1; j <= dim2; j++) {
            if (file1[i - 1] != file2[j - 1]) {
                int k = MIN3(matrice[i][j - 1], matrice[i - 1][j], matrice[i - 1][j - 1]);
                matrice[i][j] = k + 1;
            } else
                matrice[i][j] = matrice[i - 1][j - 1];
        }
}

/**
 * Crea le istruzioni scorrendo la matrice costruita dall'algoritmo di Levenshtein.
 *
 */
void instructions(int **matrice, const char *file1, long dim1, char *file2, long dim2, INSTRUCTIONS *istruzioni) {
    unsigned int tmp = initialInstructions(matrice, file1, &dim1, file2, &dim2, istruzioni);
    finalInstructions(file1, &dim1, file2, &dim2, istruzioni, tmp);
}

/**
 * Istruzioni elementari per trasformare una stringa in un'altra.
 * @param istruzioni        istruzioni da effettuare per trasformare file1 in file2
 * @return il numero di istruzioni generate
 */
unsigned int
initialInstructions(int **matrice, const char *file1, long *dim1, char *file2, long *dim2, INSTRUCTIONS *istruzioni) {
    unsigned int k, toReturn = 0;
    while (*dim1 > 0 && *dim2 > 0) {
        k = MIN3(matrice[*dim1 - 1][*dim2 - 1], matrice[*dim1 - 1][*dim2], matrice[*dim1][*dim2 - 1]);
        if (file1[*dim1 - 1] != file2[*dim2 - 1]) {
            if (k == matrice[*dim1 - 1][*dim2 - 1]) {
                createInstruction(&istruzioni[toReturn], "SET", *dim2, file2[*dim2 - 1]);
                toReturn++;
            } else if (k == matrice[*dim1][*dim2 - 1]) {
                createInstruction(&istruzioni[toReturn], "ADD", *dim2, file2[*dim2 - 1]);
                (*dim1)++;
                toReturn++;
            } else if (k == matrice[*dim1 - 1][*dim2]) {
                createInstruction(&istruzioni[toReturn], "DEL", *dim2 + 1, ' ');
                (*dim2)++;
                toReturn++;
            }
        }
        (*dim1)--;
        (*dim2)--;
    }
    return toReturn;
}

/**
 * Istruzioni aggiuntive per file di dimensioni diverse.
 */
void
finalInstructions(const char *file1, long *dim1, char *file2, long *dim2, INSTRUCTIONS *istruzioni, unsigned int cont) {
    while (*dim2 > 0) {
        if (file1[*dim1 - 1] != file2[*dim2 - 1] || *dim1 == 0) {
            createInstruction(&istruzioni[cont], "ADD", *dim2, file2[*dim2 - 1]);
            cont++;
        }
        (*dim2)--;
    }
    while (*dim1 > 0) {
        if (file1[*dim1 - 1] != file2[*dim2 - 1] || *dim2 == 0) {
            createInstruction(&istruzioni[cont], "DEL", *dim2, ' ');
            cont++;
        }
        (*dim1)--;
    }
}

/**
 * Associa ad ogni elemento "istruzione" il suo tipo, l'indice nel file ed il byte da impostare.
 * @param istruzioni        struttura contenente tutte le istruzioni
 * @param operazione        tipo di istruzione
 * @param n                 indice della posizione nel file
 * @param b                 nuovo byte
 */
void createInstruction(struct INSTRUCTIONS *istruzioni, char *operazione, unsigned int n, char b) {
    memcpy(istruzioni->operazione, operazione, 4);
    istruzioni->n = n;
    istruzioni->b = b;
}

/**
 * Stampa le istruzioni in binario sul file output.
 * @param istruzioni        struttura contenente tutte le istruzioni generate
 * @param distance          distanza di edit(è uguale alla somma delle istruzioni)
 * @param output            file di output per le istruzioni
 */
void printInstructions(INSTRUCTIONS *istruzioni, unsigned int distance, char *output) {
    FILE *file;
    if ((file = fopen(output, "wb+")) == NULL) {
        printf("Error!\n");
        exit(-1);
    }
    for (register int i = (int) distance - 1; i >= 0; i--) {
        char *tmp = istruzioni[i].operazione;
        for (int j = 0; j < 3; j++) {
            fwrite(&tmp[j], sizeof(char), 1, file);
        }
        unsigned int num = istruzioni[i].n;
        fwrite(&num, sizeof(unsigned int), 1, file);
        fwrite(&istruzioni[i].b, sizeof(char), 1, file);
        fwrite("\n", sizeof(char), 1, file);
    }
    fclose(file);
}

/**
 * Libera lo spazio di memoria delle INSTRUCTIONS.
 */
void freeInstructions(INSTRUCTIONS *istruzioni, unsigned int size) {
    for (int i = 0; i < size; i++) {
        free(istruzioni[i].operazione);
        istruzioni[i].operazione = NULL;
    }
    free(istruzioni), istruzioni = NULL;
}

/**
 * Libera lo spazio di memoria della matrice e dei suoi elementi.
 */
void freeMatrix(int **matrice, long dim1) {
    for (register int i = 0; i <= dim1; i++) {
        free(matrice[i]);
        matrice[i] = NULL;
    }
    free(matrice), matrice = NULL;
}