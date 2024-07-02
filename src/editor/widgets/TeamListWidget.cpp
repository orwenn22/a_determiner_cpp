#include "TeamListWidget.h"

#include <raylib.h>

#include "editor/layers/LayerSpawnRegions.h"
#include "engine/util/Trace.h"
#include "engine/Globals.h"
#include "Teams.h"


TeamListWidget::TeamListWidget(EditorState *editor, LayerSpawnRegions *spawn_regions_layer, int x, int y, int w, int h) : Widget(x, y, w, h) {
    m_editor = editor;
    m_spawn_regions_layer = spawn_regions_layer;
}

TeamListWidget::~TeamListWidget()  = default;

void TeamListWidget::Update() {
    if(IsMouseUsed() || !IsMouseHovering()) return;

    int x = AbsoluteX();
    int y = AbsoluteY();

    int mx = GetMouseX() - x;
    int my = GetMouseY() - y;

    int hoverred_entry = my/26;   // + m_scroll
    //TODO : maybe in the future we should store the team colors inside the EditorSpawnRegion object.
    //       if we do this, then clicking on an entry here should open another window allowing to change the color.

    UseMouse();
}

void TeamListWidget::Draw() {
    int x = AbsoluteX();
    int y = AbsoluteY();

    int painter_y = y;

    for(int i = 0; i < m_spawn_regions_layer->GetSpawnRegionCount(); ++i) {
        DrawRectangle(x+3, painter_y+3, 20, 20, s_team_colors[i]);
        DrawRectangleLines(x+3, painter_y+3, 20, 20, WHITE);
        DrawText(s_team_names[i], x+26, painter_y+3, 20, WHITE);

        painter_y += 26;
        if(painter_y+26 > y+Height()) break;
    }

    DrawRectangleLines(x, y, Width(), Height(), WHITE);
}
