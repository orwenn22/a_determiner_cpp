#include <raylib.h>

#include "engine/metrics/Graphics.h"
#include "engine/metrics/MetricsCamera.h"

void DrawDebugMenu(float dt) {
    MetricsCamera *cam = Metrics::GetGraphicsCam();

    int mouse_x = GetMouseX();
    int mouse_y = GetMouseY();

    DrawFPS(10, 10);
    DrawText(TextFormat("DT : %f", dt), 10, 30, 20, RED);

    if(cam == nullptr) {
        DrawText("CAM : null", 10, 50, 20, WHITE);
        DrawText(TextFormat("MOUSE POS : px : %i %i  |  m : [NO CAM]", mouse_x, mouse_y), 10, 70, 20, WHITE);
    }
    else {
        Vector2 mouse_world_pos = cam->ConvertAbsoluteToMeters(mouse_x, mouse_y);

        DrawText(TextFormat("CAM : [origin_x : %f , origin_y : %f , ppm : %i]", cam->origin_x, cam->origin_y, cam->PixelsPerMeter()),
                 10, 50, 20, WHITE);
        DrawText(TextFormat("MOUSE POS : px : %i %i  |  m : %f %f", mouse_x, mouse_y, mouse_world_pos.x, mouse_world_pos.y),
                 10, 70, 20, WHITE);
    }
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(512, 512, "À déterminer");
    SetTargetFPS(240);

    MetricsCamera metrics_camera(0, 0, 20);
    Metrics::SetGraphicsCam(&metrics_camera);

    while(!WindowShouldClose()) {
        // UPDATE
        float dt = GetFrameTime();
        if(IsKeyDown(KEY_UP)) metrics_camera.origin_y += (int)100*dt;

        //DRAW
        BeginDrawing();
        ClearBackground(BLACK);
        Metrics::DrawGrid();
        DrawDebugMenu(dt);
        EndDrawing();

    }

    CloseWindow();
    return 0;
}