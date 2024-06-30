#include <string>
#include "FileOp.h"
#include "engine/util/Trace.h"


static_assert(sizeof(float) == 4, "float type must be 4 bytes");

#ifndef __BYTE_ORDER__
#error __BYTE_ORDER__ not defined
#endif

void WriteU32(uint32_t val, FILE *f) {
    fputc((int)( val        & 0xff), f);
    fputc((int)((val >>  8) & 0xff), f);
    fputc((int)((val >> 16) & 0xff), f);
    fputc((int)((val >> 24) & 0xff), f);
}

uint32_t ReadU32(FILE *f) {
    uint32_t r = 0;
    r |= (fgetc(f) & 0xff);
    r |= (fgetc(f) & 0xff) <<  8;
    r |= (fgetc(f) & 0xff) << 16;
    r |= (fgetc(f) & 0xff) << 24;
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

void WriteTexture(Texture texture, FILE *f) {
    if(texture.id <= 0) return;

    Image tileset_image = LoadImageFromTexture(texture);
    fputs("img", f);                             //signature
    WriteU32(tileset_image.width, f);                //image width
    WriteU32(tileset_image.height, f);               //image height
    for(int y = 0; y < tileset_image.height; ++y) {
        for(int x = 0; x < tileset_image.width; ++x) {
            Color c = GetImageColor(tileset_image, x, y);
            fputc(c.r, f);
            fputc(c.g, f);
            fputc(c.b, f);
            fputc(c.a, f);
        }
    }
    fputs("gmi", f);                             //2nd signature
    UnloadImage(tileset_image);
}

Texture ReadTexture(FILE *f) {
    char sig[4] = { 0 };
    fgets(sig, 4, f);
    if(std::string(sig) != "img") {
        TRACE("No 'img' signature");
        return {.id = 0};
    }

    int tileset_width = (int) ReadU32(f);
    int tileset_height = (int) ReadU32(f);
    if(tileset_width <= 0 || tileset_height <= 0) {
        TRACE("Invalid size parameter\n");
        return {.id = 0};
    }

    Image tileset_image = GenImageColor(tileset_width, tileset_height, BLANK);
    for(int y = 0; y < tileset_height; ++y) {
        for(int x = 0; x < tileset_width; ++x) {
            Color c;
            c.r = fgetc(f);
            c.g = fgetc(f);
            c.b = fgetc(f);
            c.a = fgetc(f);
            ImageDrawPixel(&tileset_image, x, y, c);
        }
    }

    sig[0] = 0;
    fgets(sig, 4, f);
    if(std::string(sig) != "gmi") {
        TRACE("No 'gmi' signature");
        UnloadImage(tileset_image);
        return {.id = 0};
    }

    Texture tileset_texture = LoadTextureFromImage(tileset_image);
    UnloadImage(tileset_image);
    return tileset_texture;
}


bool CheckSignature(const char *signature, int length, FILE *f) {
    for(int i = 0; i < length; ++i) {
        if(fgetc(f) != signature[i]) return false;
    }
    return true;
}
