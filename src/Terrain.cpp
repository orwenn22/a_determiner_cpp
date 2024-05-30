#include "Terrain.h"

#include <cmath>
#include <malloc.h>

#include "engine/metrics/Graphics.h"
#include "engine/util/Trace.h"

Terrain *Terrain::construct(const char *filepath, Vector2 size) {
    FILE *in_file = fopen(filepath, "r");
    if(in_file == nullptr) return nullptr;
    fclose(in_file);
    return new Terrain(filepath, size);
}

Terrain::Terrain(const char *filepath, Vector2 size) {
    m_origin = {0.f, 0.f};

    m_image = LoadImage(filepath);
    m_texture = LoadTextureFromImage(m_image);
    m_size = size;

    m_collision_mask = (uint8_t *) malloc(sizeof(uint8_t) * m_image.width * m_image.height);

    Color *image_colors =LoadImageColors(m_image);
    TRACE("calculating terrain mash...\n");
    for(size_t y = 0; y < m_image.height; ++y) {
        for(size_t x = 0; x < m_image.width; ++x) {
            m_collision_mask[x + y*m_image.width] = (image_colors[x + y*m_image.width].a != 0);
        }
    }
    UnloadImageColors(image_colors);
    TRACE("Done ! :D\n");
}

Terrain::~Terrain() {
    free(m_collision_mask);
    UnloadImage(m_image);
    UnloadTexture(m_texture);
}


void Terrain::Draw() {
    Metrics::DrawSpriteScale(m_texture, {m_origin.x, m_origin.y, m_size.x, m_size.y}, WHITE);
    Metrics::DrawRectangle(m_origin.x, m_origin.y, m_size.x, m_size.y, RED, false);
}


bool Terrain::CheckCollision(Vector2 position, bool outside_solid) {
    //Compute the coordinates of the correct pixel on the image
    int pixel_x = (int) ((position.x - m_origin.x)/m_size.x * (float)m_image.width) - (position.x < m_origin.x);
    int pixel_y = (int) ((position.y - m_origin.y)/m_size.y * (float)m_image.height) - (position.y < m_origin.y);

    if(pixel_x < 0 || pixel_x >= m_image.width || pixel_y < 0 || pixel_y >= m_image.height) return outside_solid;
    return m_collision_mask[pixel_x + pixel_y*m_image.width];
}

bool Terrain::CheckCollisionRec(Rectangle rec, bool outside_solid) {
    //Top left
    int pixel_x = (int) (rec.x/m_size.x * (float)m_image.width) - (rec.x < m_origin.x);
    int pixel_y = (int) (rec.y/m_size.y * (float)m_image.height) - (rec.y < m_origin.y);

    //Bottom right
    int pixel_x2 = (int) ((rec.x + rec.width - m_origin.x)/m_size.x * (float)m_image.width) - (rec.x + rec.width < m_origin.x);
    int pixel_y2 = (int) ((rec.y + rec.height - m_origin.y)/m_size.y * (float)m_image.height) - (rec.y + rec.height < m_origin.y);

    if(outside_solid) {
        if(pixel_x < 0 || pixel_x2 >= m_image.width || pixel_y < 0 || pixel_y2 >= m_image.height) return true;
    }
    else {
        if(pixel_x < 0) pixel_x = 0;
        if(pixel_x2 >= m_image.width) pixel_x2 = m_image.width - 1;
        if(pixel_y < 0) pixel_y = 0;
        if(pixel_y2 >= m_image.height) pixel_y2 = m_image.height - 1;
    }

    //Check left and right edges
    if(!(pixel_x >= m_image.width || pixel_x2 < 0)) {
        for(size_t y = pixel_y; y <= pixel_y2; ++y) {
            if(m_collision_mask[pixel_x + y * m_image.width] || m_collision_mask[pixel_x2 + y * m_image.width]) return true;
        }
    }

    //Check top and bottom edges
    if(!(pixel_y >= m_image.height || pixel_y2 < 0)) {
        for(size_t x = pixel_x; x <= pixel_x2; ++x) {
            if(m_collision_mask[x + pixel_y * m_image.width] || m_collision_mask[x + pixel_y2 * m_image.width]) return true;
        }
    }

    //Check center (only check 1/9 of the pixels to save time)
    for(size_t y = pixel_y+1; y < pixel_y2; ++y) {
        for(size_t x = pixel_x+1; x < pixel_x2; ++x) {
            if(m_collision_mask[x + y * m_image.width]) return true;
        }
    }

    return false;
}


void Terrain::DestroyRectangle(Rectangle rec) {
    //Top left
    int pixel_x = (int) (rec.x/m_size.x * (float)m_image.width) - (rec.x < m_origin.x);
    int pixel_y = (int) (rec.y/m_size.y * (float)m_image.height) - (rec.y < m_origin.y);

    //Bottom right
    int pixel_x2 = (int) ((rec.x + rec.width - m_origin.x)/m_size.x * (float)m_image.width) - (rec.x + rec.width < m_origin.x);
    int pixel_y2 = (int) ((rec.y + rec.height - m_origin.y)/m_size.y * (float)m_image.height) - (rec.y + rec.height < m_origin.y);

    //Make sure we don't try to destroy outside
    if(pixel_x < 0) pixel_x = 0;
    if(pixel_x2 >= m_image.width) pixel_x2 = m_image.width - 1;
    if(pixel_y < 0) pixel_y = 0;
    if(pixel_y2 >= m_image.height) pixel_y2 = m_image.height - 1;

    for(size_t y = pixel_y; y <= pixel_y2; ++y) {
        for(size_t x = pixel_x; x <= pixel_x2; ++x) {
            UnsafeDestroyPixel((int)x, (int)y);
        }
    }

    UpdateSprite();
}

void Terrain::DestroyCircle(Vector2 center, float radius) {
    //Calculate the horizontal and vertical "radius" of the shape (in case a pixel of the terrain is not a square)
    int radius_width = (int)(radius / m_size.x * (float)m_image.width);
    int radius_height = (int)(radius / m_size.y * (float)m_image.height);
    DestroyElispePixel(center, radius_width, radius_height);
}


///////////////////////////////
//// PRIVATE

void Terrain::UnsafeDestroyPixel(int x, int y) {
    //TODO : make a way to make some stuff indestructible ?
    m_collision_mask[x + y * m_image.width] = 0;
    ImageDrawPixel(&m_image, x, y, {0, 0, 0, 0});
}

/**
 * Destroy an elispe/circle-shaped section of the terrain
 * @param center center (in meters) of the shape
 * @param radius_width width/2 (in pixels)
 * @param radius_height height/2 (in pixels)
 */
void Terrain::DestroyElispePixel(Vector2 center, int radius_width, int radius_height) {
    int pixel_x = (int) ((center.x - m_origin.x)/m_size.x * (float)m_image.width) - (center.x < m_origin.x);
    int pixel_y = (int) ((center.y - m_origin.y)/m_size.y * (float)m_image.height) - (center.y < m_origin.y);

    //Number of iterations we need to do in order to noit skip columns of pixels of the circle
    int step_count = (int)((float)radius_width * 3.14f);

    int previous_xpos = -99999;
    for(int i = 0; i < step_count; ++i) {
        //compute x position for the current step
        int x_pos = pixel_x + (int)(cos((double)i*PI / (double)step_count) * (double)radius_width);
        if(x_pos == previous_xpos) continue;        //don't do the same column multiple times
        previous_xpos = x_pos;

        if(x_pos < 0 || x_pos >= m_image.width) continue;       //don't try to remove outside the terrain

        int y_dist = (int)(sin((double)i*PI / (double)step_count) * (double)radius_height);         //y distance from the x axes
        for(int y = pixel_y-y_dist; y <= pixel_y+y_dist; ++y) {                 //for each pixel of the columns...
            if(y < 0 || y >= m_image.height) continue;                          //check if it is in bound....
            UnsafeDestroyPixel(x_pos, y);                                    //If so destroy it
        }
    }

    UpdateSprite();
}


void Terrain::UpdateSprite() {
    UnloadTexture(m_texture);
    m_texture = LoadTextureFromImage(m_image);
}
