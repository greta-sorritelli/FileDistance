
#ifndef FILEDISTANCE_APPLY_H
#define FILEDISTANCE_APPLY_H

void apply(char *input, char *filem, char *output);

long fileToStructTraduction(INSTRUCTIONS *inst, FILE *f);

void applicationMod(FILE *f1, FILE *f2, INSTRUCTIONS *inst, long sizeInst);

void freeStruct(INSTRUCTIONS *inst, long sizeFm);

#endif //FILEDISTANCE_APPLY_H
