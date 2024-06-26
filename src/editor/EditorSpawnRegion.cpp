#include "EditorSpawnRegion.h"

#include "engine/metrics/Graphics.h"
#include "engine/util/Trace.h"
#include "engine/Globals.h"
#include "EditorState.h"
#include "Teams.h"

EditorSpawnRegion::EditorSpawnRegion(EditorState *editor, float x, float y, float w, float h, int team_index) {
    m_editor = editor;
    m_x = x;
    m_y = y;
    m_w = w;
    m_h = h;
    m_team_index = team_index;

    m_mouse_offset_x = 0;
    m_mouse_offset_y = 0;
    m_follow_mouse = false;
    m_resized = false;
}

void EditorSpawnRegion::Update() {
    Vector2 mouse_m = Metrics::GetGraphicsCam()->ConvertAbsoluteToMeters(GetMouseX(), GetMouseY());

    if(IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
        m_resized = false;
        m_follow_mouse = false;
    }

    if(m_follow_mouse) {
        if(IsKeyDown(KEY_LEFT_SHIFT)) {
            //m_x = (float)(int)(mouse_m.x - m_mouse_offset_x);
            //m_y = (float)(int)(mouse_m.y - m_mouse_offset_y);
            m_x = (float)(int)((mouse_m.x - m_mouse_offset_x) / m_editor->GetTileWidthM()) * m_editor->GetTileWidthM();
            m_y = (float)(int)((mouse_m.y - m_mouse_offset_y) / m_editor->GetTileHeightM()) * m_editor->GetTileHeightM();
        }
        else {
            m_x = mouse_m.x - m_mouse_offset_x;
            m_y = mouse_m.y - m_mouse_offset_y;
        }
    }
    else if(m_resized) {
        if(IsKeyDown(KEY_LEFT_SHIFT)) {
            m_w = (float)(int)((mouse_m.x-m_x) / m_editor->GetTileWidthM()) * m_editor->GetTileWidthM();
            m_h = (float)(int)((mouse_m.y-m_y) / m_editor->GetTileHeightM()) * m_editor->GetTileHeightM();
        }
        else {
            m_w = mouse_m.x-m_x;
            m_h = mouse_m.y-m_y;
        }
    }

    if(m_w < 1.f) m_w = 1.f;
    if(m_h < 1.f) m_h = 1.f;

    if(IsMouseUsed() || m_editor->GetCurrentLayer() != Layer_Teams) return;

    if(!CheckCollisionPointRec(mouse_m, {m_x, m_y, m_w, m_h})) return;
    UseMouse();

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        TRACE("CLICKED ON SPAWN REGION\n");
        m_mouse_offset_x = mouse_m.x - m_x;
        m_mouse_offset_y = mouse_m.y - m_y;
        if(mouse_m.x > m_x+m_w-.5f && mouse_m.y > m_y+m_h-.5f) {
            TRACE("CORNER\n");
            m_resized = true;
            m_follow_mouse = false;
        }
        else {
            TRACE("BODY\n");
            m_follow_mouse = true;
            m_resized = false;
        }
    }
}

void EditorSpawnRegion::Draw() {
    Color c = s_team_colors[m_team_index];
    c.a = 100;
    Metrics::DrawRectangle(m_x, m_y, m_w, m_h, c, true);
    if(m_follow_mouse || m_resized) {
        Metrics::DrawRectangle(m_x, m_y, m_w, m_h, GREEN, false);
    }
}
