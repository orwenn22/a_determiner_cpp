#include "EditorPaletteWidget.h"

#include <raylib.h>

#include "engine/util/Trace.h"
#include "engine/Globals.h"
#include "engine/TileGrid.h"
#include "engine/Tileset.h"
#include "terrain/TilemapTerrain.h"
#include "EditorState.h"
#include "GlobalResources.h"


EditorPaletteWidget::EditorPaletteWidget(EditorState *editor, int x, int y, int w, int h) : Widget(x, y, w, h) {
    m_editor = editor;
    m_first_tile_scroll = 0;
    m_zoom = 2;
}

void EditorPaletteWidget::Update() {
    //Widget::Update();
    if(!IsMouseHovering() || IsMouseUsed()) return;
    UseMouse();

    Tileset *tileset = (m_editor->IsEditingCollisions()) ? Res::collisions_tileset : m_editor->GetTerrain()->GetTileset();
    if(tileset == nullptr || tileset->GetTileWidth() == 0) return;
    int tiles_per_rows = Width()/(tileset->GetTileWidth()*m_zoom);


    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int mx = GetMouseX() - AbsoluteX();
        int my = GetMouseY() - AbsoluteY();

        int tile_x = mx/(tileset->GetTileWidth()*m_zoom);
        int tile_y = my/(tileset->GetTileHeight()*m_zoom);
        int tile_index = m_first_tile_scroll + (tile_x + tile_y*tiles_per_rows);
        m_editor->SetPaletteIndex(tile_index);
        TRACE("clicked on %i\n", tile_index);
    }
    else {
        if(IsKeyDown(KEY_LEFT_CONTROL)) {
            m_zoom += (int)GetMouseWheelMove();
            if(m_zoom < 1) m_zoom = 1;
        }
        else {
            m_first_tile_scroll += tiles_per_rows * -(int) GetMouseWheelMove();
            if (m_first_tile_scroll < 0) m_first_tile_scroll = 0;
        }
    }
}

void EditorPaletteWidget::Draw() {
    //Widget::Draw();
    int x = AbsoluteX();
    int y = AbsoluteY();

    DrawRectangleLines(x, y, Width(), Height(), WHITE);

    Tileset *tileset = (m_editor->IsEditingCollisions()) ? Res::collisions_tileset : m_editor->GetTerrain()->GetTileset();
    if(tileset == nullptr || !tileset->Usable()) {
        DrawText("Error while drawing tileset :(", x+2, y+2, 10, RED);
        return;
    }

    int tile_width = tileset->GetTileWidth()*m_zoom;
    int tile_height = tileset->GetTileHeight()*m_zoom;
    int painter_x = 1;
    int painter_y = 1;
    int tile_index = m_first_tile_scroll;
    while(painter_y < Height()-tile_height) {
        while(painter_x < Width()-tile_width) {
            Rectangle dest = {(float)(x+painter_x), (float)(y+painter_y), (float)tile_width, (float)tile_height};
            tileset->Draw(tile_index, dest);
            DrawText(TextFormat("%i", tile_index), (int)dest.x, (int)dest.y, 10, WHITE);
            ++tile_index;
            painter_x += tile_width;
        }
        painter_x = 1;
        painter_y += tile_height;
    }
}
