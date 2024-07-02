#ifndef A_DETERMINER_FILEOP_H
#define A_DETERMINER_FILEOP_H

#include <cstdio>
#include <cstdint>
#include <raylib.h>

void WriteU32(uint32_t val, FILE *f);
uint32_t ReadU32(FILE *f);

void WriteF32(float val, FILE *f);
float ReadF32(FILE *f);

void WriteTexture(Texture texture, FILE *f);
Texture ReadTexture(FILE *f);

bool CheckSignature(const char *signature, int length, FILE *f);

#endif //A_DETERMINER_FILEOP_H
