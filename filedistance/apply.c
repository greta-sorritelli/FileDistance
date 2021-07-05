
#include "io.h"

/**
 * Applica le istruzioni dal filem su input e salva il risultato su output.
 * @param input      file da modificare
 * @param filem      file contenente le modifiche
 * @param output     file di output
 */
void apply(char *input, char *filem, char *output) {
    INSTRUCTIONS *instr = NULL;

    FILE *f1 = NULL;
    if ((f1 = fopen(input, "rb")) == NULL) {
        printf("Error opening file!\n");
        exit(-1);
    }
    FILE *f2 = NULL;
    if ((f2 = fopen(output, "wb")) == NULL) {
        printf("Error opening file!\n");
        exit(-1);
    }
    FILE *fm = NULL;
    if ((fm = fopen(filem, "rb")) == NULL) {
        printf("Error opening file!\n");
        exit(-1);
    }

    long sizeFm = fileSize(fm);
    instr = malloc((sizeFm / 9) * sizeof(INSTRUCTIONS));
    for (int i = 0; i < (sizeFm / 9); i++)
        instr[i].operazione = malloc(4 * sizeof(char));
    long sizeInstructions = fileToStructTraduction(instr, fm);

    applicationMod(f1, f2, &instr[0], sizeInstructions);
    freeStruct(instr, sizeFm);
    fclose(f1), fclose(f2), fclose(fm);
}

/**
 * Converte in struttura le istruzioni lette dal file binario.
 * @return il numero di istruzioni
 */
long fileToStructTraduction(INSTRUCTIONS *inst, FILE *f) {
    int i = 0;
    char type;
    unsigned int pos = 0;
    char byte;
    while (fread(&type, sizeof(char), 1, f) == 1) {
        fseek(f, 2, SEEK_CUR);
        fread(&pos, (sizeof(unsigned int)), 1, f);
        fread(&byte, (sizeof(char)), 1, f);
        switch (type) {
            case 'S':
                createInstruction(&inst[i++], "SET", pos, byte);
                fseek(f, 1, SEEK_CUR);
                break;
            case 'A':
                createInstruction(&inst[i++], "ADD", pos, byte);
                fseek(f, 1, SEEK_CUR);
                break;
            case 'D':
                createInstruction(&inst[i++], "DEL", pos, '\0');
                fseek(f, 1, SEEK_CUR);
                break;
        }
        type = '\0';
    }
    return i;
}

/**
 * Applica le modifiche al file f2, confrontando il file f1 con le istruzioni presenti nella struttura.
 *
 */
void applicationMod(FILE *f1, FILE *f2, INSTRUCTIONS *inst, long sizeInst) {
    char ch;
    unsigned int count1 = 0, i = 0;
    rewind(f1);
    rewind(f2);
    while (i <= sizeInst - 1) {
        fread(&ch, sizeof(char), 1, f1);
        if (inst->n - 1 == count1 || inst->n == 0) {
            switch (inst->operazione[0]) {
                case 'S':
                    fwrite(&inst->b, sizeof(char), 1, f2);
                    count1++, i++, inst++;
                    break;
                case 'A':
                    fwrite(&inst->b, sizeof(char), 1, f2);
                    fseek(f1, -1, SEEK_CUR);
                    count1++, i++, inst++;
                    break;
                case 'D':
                    i++, inst++;
                    break;
            }
        } else {
            fwrite(&ch, sizeof(char), 1, f2);
            count1++;
        }
    }
    while (fread(&ch, sizeof(char), 1, f1) == 1)
        fwrite(&ch, sizeof(char), 1, f2);
}

/**
 * Serve a liberare l'aria di memoria allocata per la struttura delle istruzioni
 */
void freeStruct(INSTRUCTIONS *inst, long sizeFm) {
    for (int i = 0; i < (sizeFm / 9); i++) {
        free(inst[i].operazione);
        inst[i].operazione = NULL;
    }
    free(inst), inst = NULL;
}