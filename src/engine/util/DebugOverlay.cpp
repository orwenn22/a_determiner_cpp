#include "DebugOverlay.h"

#include "../metrics/Graphics.h"
#include "../metrics/MetricsCamera.h"
#include "../state/StateManager.h"
#include "../state/State.h"


void DrawDebugOverlay(float dt, StateManager *sm) {
    MetricsCamera *cam = Metrics::GetGraphicsCam();

    int painter_y = 10;

    int mouse_x = GetMouseX();
    int mouse_y = GetMouseY();

    DrawFPS(10, painter_y);
    painter_y += 20;

    DrawText(TextFormat("DT : %f", dt), 10, painter_y, 20, RED);
    painter_y += 20;

    if(cam == nullptr) {
        DrawText(TextFormat("MOUSE POS : px : %i %i  |  m : [NO CAM]", mouse_x, mouse_y), 10, painter_y, 20, WHITE);
        painter_y += 20;
    }
    else {
        Vector2 mouse_world_pos = cam->ConvertAbsoluteToMeters(mouse_x, mouse_y);

        DrawText(TextFormat("CAM : [origin_x : %f , origin_y : %f , ppm : %i]", cam->origin_x, cam->origin_y, cam->PixelsPerMeter()),
                 10, painter_y, 20, WHITE);
        painter_y += 20;

        DrawText(TextFormat("MOUSE POS : px : %i %i  |  m : %f %f", mouse_x, mouse_y, mouse_world_pos.x, mouse_world_pos.y),
                 10, painter_y, 20, WHITE);
        painter_y += 20;
    }

    if(sm != nullptr) {
        State *state = sm->GetState();
        if(state == nullptr) {
            DrawText("STATE : null", 10, painter_y, 20, WHITE);
        }
        else {
            DrawText(TextFormat("STATE : %s", state->GetIdentifier()), 10, painter_y, 20, WHITE);
        }
        painter_y += 20;
    }
}
