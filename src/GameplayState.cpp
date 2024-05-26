#include "GameplayState.h"

#include <raylib.h>

#include "engine/Globals.h"
#include "engine/metrics/Graphics.h"
#include "engine/metrics/MetricsCamera.h"

GameplayState::GameplayState() {
    m_camera = new MetricsCamera(0, 0, 20);
    m_cam_follow_mouse = false;
    m_cam_mouse_offset = {0.f, 0.f};
}

GameplayState::~GameplayState() {
    delete m_camera;
}

void GameplayState::Update(float dt) {
    if(IsKeyDown(KEY_UP)) m_camera->origin_y += (int)100*dt;

    int mouse_x = GetMouseX();
    int mouse_y = GetMouseY();

    //TODO : update widgets here

    HandleDragCamera((float)mouse_x, (float)mouse_y);

}

void GameplayState::Draw() {
    Metrics::SetGraphicsCam(m_camera);

    Metrics::DrawGrid();
}


////////////////////////////////
//// PRIVATE

void GameplayState::HandleDragCamera(float mouse_x, float mouse_y) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && !IsMouseUsed()) {
        m_cam_follow_mouse = true;
        m_cam_mouse_offset = {m_camera->origin_x - mouse_x, m_camera->origin_y - mouse_y};
    }
    if(IsMouseButtonUp(MOUSE_BUTTON_RIGHT)) {
        m_cam_follow_mouse = false;
    }

    if(m_cam_follow_mouse) {
        m_camera->origin_x = m_cam_mouse_offset.x + mouse_x;
        m_camera->origin_y = m_cam_mouse_offset.y + mouse_y;
    }
    else {
        if(IsMouseUsed()) return;

        // Handle zoom
        float mouse_wheel = GetMouseWheelMove();
        if(mouse_wheel > 0) m_camera->SetPixelsPerMeter(m_camera->PixelsPerMeter() * 2);
        else if(mouse_wheel < 0) m_camera->SetPixelsPerMeter(m_camera->PixelsPerMeter() / 2);
    }
}
