#include "EditorPaletteWidget.h"

#include <raylib.h>

#include "editor/layers/Layer.h"
#include "editor/layers/LayerTilemap.h"
#include "editor/EditorState.h"
#include "engine/util/Trace.h"
#include "engine/Globals.h"
#include "engine/Tileset.h"


EditorPaletteWidget::EditorPaletteWidget(EditorState *editor, int x, int y, int w, int h) : Widget(x, y, w, h) {
    m_editor = editor;
    m_zoom = 2;
}

void EditorPaletteWidget::Update() {
    //Widget::Update();
    if(!IsMouseHovering() || IsMouseUsed()) return;
    UseMouse();

    Layer *current_layer_unknown = m_editor->GetCurrentLayer();
    if(current_layer_unknown->Type() != LayerType_Tilemap) return;
    LayerTilemap *current_layer = (LayerTilemap *) current_layer_unknown;

    Tileset *tileset = ((LayerTilemap *)current_layer_unknown)->GetTileset();
    if(tileset == nullptr || tileset->GetTileWidth() == 0) return;

    //Calculate nuber of tiles in a row
    int tiles_per_rows = Width()/(tileset->GetTileWidth()*m_zoom);

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int mx = GetMouseX() - AbsoluteX();
        int my = GetMouseY() - AbsoluteY();

        int tile_x = mx/(tileset->GetTileWidth()*m_zoom);
        int tile_y = my/(tileset->GetTileHeight()*m_zoom);
        int tile_index = current_layer->GetPaletteScroll() + (tile_x + tile_y*tiles_per_rows);
        m_editor->SetPaletteIndex(tile_index);
        TRACE("clicked on %i\n", tile_index);
    }
    else {
        //Zooming
        if(IsKeyDown(KEY_LEFT_CONTROL)) {
            m_zoom += (int)GetMouseWheelMove();
            if(m_zoom < 1) m_zoom = 1;
        }
        //Scrolling
        else {
            current_layer->SetPaletteScroll(current_layer->GetPaletteScroll() + (tiles_per_rows * -(int)GetMouseWheelMove()));
        }
    }
}

void EditorPaletteWidget::Draw() {
    //Widget::Draw();
    int x = AbsoluteX();
    int y = AbsoluteY();

    //Outline
    DrawRectangleLines(x, y, Width(), Height(), WHITE);

    //Try to get the selected layer
    Layer *current_layer = m_editor->GetCurrentLayer();
    if(current_layer == nullptr || current_layer->Type() != LayerType_Tilemap) {
        DrawText("Not a tile layer :(", x+2, y+2, 10, RED);
        return;
    }

    //Try to get the tileset
    Tileset *tileset = ((LayerTilemap *)current_layer)->GetTileset();
    if(tileset == nullptr || !tileset->Usable()) {
        DrawText("Error while drawing tileset :(", x+2, y+2, 10, RED);
        return;
    }

    //Prepare to draw the tiles on the palette
    int tile_width = tileset->GetTileWidth()*m_zoom;
    int tile_height = tileset->GetTileHeight()*m_zoom;
    int painter_x = 1;
    int painter_y = 1;
    int tile_index = ((LayerTilemap *)current_layer)->GetPaletteScroll();
    int selected_tile = m_editor->GetPaletteIndex();

    while(painter_y < Height()-tile_height) {       //Rows
        while(painter_x < Width()-tile_width) {     //Columns
            Rectangle dest = {(float)(x+painter_x), (float)(y+painter_y), (float)tile_width, (float)tile_height};
            tileset->Draw(tile_index, dest);
            DrawText(TextFormat("%i", tile_index), (int)dest.x+1, (int)dest.y+1, 10, WHITE);
            if(tile_index == selected_tile) DrawRectangleLinesEx(dest, 1, GREEN);
            ++tile_index;
            painter_x += tile_width;
        }

        //Go to next row
        painter_x = 1;
        painter_y += tile_height;
    }
}
