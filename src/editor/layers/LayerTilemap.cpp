#include "LayerTilemap.h"

#include <utility>

#include "engine/windows/WindowManager.h"
#include "engine/Globals.h"
#include "engine/TileGrid.h"
#include "engine/Tileset.h"
#include "windows/EditorImportTilesetWindow.h"
#include "EditorState.h"


LayerTilemap::LayerTilemap(EditorState *editor, std::string name) : Layer(editor, std::move(name), LayerType_Tilemap) {
    m_tileset_lock = false;
    m_palette_widget_scroll = 0;
    m_tilegrid = new TileGrid(editor->GridWidth(), editor->GridHeight());
    m_tileset = nullptr;
    m_palette_index = 1;
}

LayerTilemap::~LayerTilemap() {
    delete m_tileset;
    delete m_tilegrid;
}

void LayerTilemap::UpdateIfSelected() {
    Layer::UpdateIfSelected();
    if(IsMouseUsed() || IsMouseButtonUp(MOUSE_BUTTON_LEFT)) return;
    m_tilegrid->SetTile(Editor()->GetHoveredTileX(), Editor()->GetHoveredTileY(), m_palette_index);
    //TRACE("painting on %i %i with %i\n", Editor()->GetHoveredTileX(), Editor()->GetHoveredTileY(), m_palette_index);
    UseMouse();
}

void LayerTilemap::Draw() {
    Layer::Draw();
    if(m_tileset == nullptr || !m_tileset->Usable()) return;
    m_tilegrid->MDraw(m_tileset, {0.f, 0.f, Editor()->GetTerrainWidth(), Editor()->GetTerrainHeight()});
}

void LayerTilemap::HandleFileDrag(std::string file_name) {
    if(m_tileset_lock) return;      //TODO : display error ?
    Editor()->GetWindowManager()->AddWindow(new EditorImportTilesetWindow(Editor(), this, file_name));
}

void LayerTilemap::SetTileset(Tileset *tileset, bool lock) {
    if(m_tileset_lock) return;
    delete m_tileset;
    m_tileset = tileset;
    m_tileset_lock = lock;
    m_palette_widget_scroll = 0;
}


void LayerTilemap::SetPaletteIndex(int index) {
    if(index < 0) index = 0;
    //TODO : more checks ?
    m_palette_index = index;
}


void LayerTilemap::SetPaletteScroll(int scroll) {
    if(scroll < 0) scroll = 0;
    m_palette_widget_scroll = scroll;
}
