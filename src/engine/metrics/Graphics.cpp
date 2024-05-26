#include "Graphics.h"

//#include <cmath>  //for sqrt()

namespace Metrics {

static MetricsCamera *s_graphics_cam = nullptr;

void SetGraphicsCam(MetricsCamera *cam) { s_graphics_cam = cam; }
MetricsCamera *GetGraphicsCam() { return s_graphics_cam; }

void DrawRectangle(float x, float y, float w, float h, Color c, bool fill) {
    if(s_graphics_cam == nullptr) return;

    Vector2 origin = s_graphics_cam->ConvertMetersToAbsolute({x, y});
    int pixel_x = (int)origin.x;
    int pixel_y = (int)origin.y;
    int pixel_width = s_graphics_cam->MetersToPixels(w);
    int pixel_height = s_graphics_cam->MetersToPixels(h);

    if(fill) {
        ::DrawRectangle(pixel_x, pixel_y, pixel_width, pixel_height, c);
    }
    else {
        ::DrawRectangleLines(pixel_x, pixel_y, pixel_width, pixel_height, c);
    }
}

void DrawRectangle(Rectangle rec, Color c, bool fill) {
    DrawRectangle(rec.x, rec.y, rec.width, rec.height, c, fill);
}

void DrawLine(Vector2 start, Vector2 end, Color c) {
    if(s_graphics_cam == nullptr) return;

    ::DrawLineV(s_graphics_cam->ConvertMetersToAbsolute(start),
                s_graphics_cam->ConvertMetersToAbsolute(end),
                c);
}

void DrawGrid() {
    if(s_graphics_cam == nullptr) return;

    for(int y = -10; y < 11; ++y) {
        DrawLine({-10.f, (float)y}, {10.f, (float)y}, BLUE);
    }

    for(int x = -10; x < 11; ++x) {
        DrawLine({(float)x, -10.f}, {(float)x, 10.f}, BLUE);
    }
}

void DrawCircle(Vector2 center, float radius, Color c) {
    if(s_graphics_cam == nullptr) return;

    ::DrawCircleV(s_graphics_cam->ConvertMetersToAbsolute(center),
                  (float) s_graphics_cam->MetersToPixels(radius),
                  c);
}

/**
 * Draw a sprite using a rectangle
 * @param sprite the sprite we want to draw
 * @param rec the target rectangle (meters)
 * @param c color applied to texture
 */
void DrawSpriteScale(Texture sprite, Rectangle rec, Color c) {
    if(s_graphics_cam == nullptr) return;

    Vector2 origin = s_graphics_cam->ConvertMetersToAbsolute({rec.x, rec.y});
    float width = (float)s_graphics_cam->MetersToPixels(rec.width);
    float height = (float)s_graphics_cam->MetersToPixels(rec.height);

    if(origin.x+width < 0 || origin.y+height < 0 || origin.x >= (float)GetScreenWidth() || origin.y >= (float)GetScreenHeight()) {
        return;
    }

    ::DrawTexturePro(sprite, {0.f, 0.f, (float)sprite.width, (float)sprite.height},
                     {origin.x, origin.y, width, height},
                     {0.f, 0.f}, 0.f,
                     c);
}

/**
 * Draw a sprite using a position and rotation
 * @param sprite the sprite we want to draw
 * @param pos position of the center of the sprite (in meters)
 * @param size size of the sprite (int meters)
 * @param rotation angle
 * @param c color applied to texture
 */
void DrawSpriteRot(Texture sprite, Vector2 pos, Vector2 size, float rotation, Color c) {
    Vector2 pixel_pos = s_graphics_cam->ConvertMetersToAbsolute(pos);
    float width = (float)s_graphics_cam->MetersToPixels(size.x);
    float height = (float)s_graphics_cam->MetersToPixels(size.y);

    //float half_diagonal = sqrt(width*width + height*height) / 2.f;
    //TODO : check if sprite is on screen here ?

   ::DrawTexturePro(sprite, {0.f, 0.f, (float)sprite.width, (float)sprite.height},
                    {pixel_pos.x, pixel_pos.y, width, height},
                    {width/2.f, height/2.f}, rotation,
                    c);
}

/**
 * Draw a region of a sprite using a position and rotation
 * @param sprite the sprite we want to draw
 * @param source the region of the sprite we want to draw
 * @param pos position of the center of the sprite (in meters)
 * @param size size of the sprite (int meters)
 * @param rotation angle
 * @param c color applied to texture
 */
void DrawSpriteRotEx(Texture sprite, Rectangle source, Vector2 pos, Vector2 size, float rotation, Color c) {
    Vector2 pixel_pos = s_graphics_cam->ConvertMetersToAbsolute(pos);
    float width = (float)s_graphics_cam->MetersToPixels(size.x);
    float height = (float)s_graphics_cam->MetersToPixels(size.y);

    //float half_diagonal = sqrt(width*width + height*height) / 2.f;
    //TODO : check if sprite is on screen here ?

    ::DrawTexturePro(sprite, source,
                     {pixel_pos.x, pixel_pos.y, width, height},
                     {width/2.f, height/2.f}, rotation,
                     c);
}

};  //namespace Metrics
