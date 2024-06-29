#include "FileOp.h"


static_assert(sizeof(float) == 4, "float type must be 4 bytes");

#ifndef __BYTE_ORDER__
#error __BYTE_ORDER__ not defined
#endif

void WriteU32(uint32_t val, FILE *f) {
    fputc((int)((val >> 24) & 0xff), f);
    fputc((int)((val >> 16) & 0xff), f);
    fputc((int)((val >>  8) & 0xff), f);
    fputc((int)( val        & 0xff), f);
}

uint32_t ReadU32(FILE *f) {
    uint32_t r = 0;
    r |= (fgetc(f) & 0xff) << 24;
    r |= (fgetc(f) & 0xff) << 16;
    r |= (fgetc(f) & 0xff) <<  8;
    r |= (fgetc(f) & 0xff);
    return r;
}

void WriteF32(float val, FILE *f) {
    union { float val; uint8_t data[4]; } u{};
    u.val = val;
    //uint8_t *data = (uint8_t *) (&val);
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    for(int i = 0; i < 4; ++i) {
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    for(int i = 3; i >= 0; --i) {
#else
#error Unknown endian ???? wat
#endif
        fputc(u.data[i], f);
    }
}

float ReadF32(FILE *f) {
    union { float val; uint8_t data[4]; } u{};
    u.val = 0.f;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    for(int i = 0; i < 4; ++i) {
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    for(int i = 3; i >= 0; --i) {
#else
#error Unknown endian ???? wat
#endif
        int c = fgetc(f);
        if(c == EOF) return 0.f;
        u.data[i] = (uint8_t) c;
    }
    return u.val;
}
