#ifndef UI_FRAMEWORK_GRAPHICS_H
#define UI_FRAMEWORK_GRAPHICS_H

#include "MetricsCamera.h"

//Put in a namespace in order to avoid collisions with raylib
namespace Metrics {

    void SetGraphicsCam(MetricsCamera *cam);
    MetricsCamera *GetGraphicsCam();

    void DrawRectangle(float x, float y, float w, float h, Color c, bool fill = true);
    void DrawRectangle(Rectangle rec, Color c, bool fill = true);

    void DrawLine(Vector2 start, Vector2 end, Color c);

    void DrawGrid();

    void DrawCircle(Vector2 center, float radius, Color c);

    void DrawSpriteScale(Texture sprite, Rectangle rec, Color c);
    void DrawSpriteScaleEx(Texture sprite, Rectangle source, Rectangle dest, Color c);
    void DrawSpriteRot(Texture sprite, Vector2 pos, Vector2 size, float rotation, Color c);
    void DrawSpriteRotEx(Texture sprite, Rectangle source, Vector2 pos, Vector2 size, float rotation, Color c);

};  //namespace Metrics

#endif //UI_FRAMEWORK_GRAPHICS_H
