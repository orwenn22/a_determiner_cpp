#ifndef UI_FRAMEWORK_FILEOP_H
#define UI_FRAMEWORK_FILEOP_H

#include <cstdio>
#include <cstdint>

void WriteU32(uint32_t val, FILE *f);
uint32_t ReadU32(FILE *f);

void WriteF32(float val, FILE *f);
float ReadF32(FILE *f);

#endif //UI_FRAMEWORK_FILEOP_H
