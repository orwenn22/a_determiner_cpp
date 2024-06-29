#include "LayerTilemap.h"

#include <utility>

#include "editor/windows/EditorImportTilesetWindow.h"
#include "engine/windows/WindowManager.h"
#include "engine/util/Trace.h"
#include "engine/Globals.h"
#include "engine/TileGrid.h"
#include "engine/Tileset.h"
#include "utils/FileOp.h"
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


void LayerTilemap::ResizeGrid(int grid_w, int grid_h) {
    if(grid_w < 0 || grid_h < 0) return;
    TileGrid *new_tilegrid = new TileGrid(grid_w, grid_h);
    for(int y = 0; y < m_tilegrid->GridHeight(); ++y) {
        for(int x = 0; x < m_tilegrid->GridWidth(); ++x) {
            new_tilegrid->SetTile(x, y, m_tilegrid->GetTile(x, y));
        }
    }
    delete m_tilegrid;
    m_tilegrid = new_tilegrid;
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


void LayerTilemap::Save(FILE *out_file) {
    Layer::Save(out_file);

    if(!m_tileset->Usable()) {
        TRACE("Can't save unusable tileset :(\n");
        return;
    }

    fputs("til", out_file);             //signature
    WriteU32(m_tileset->GetTileWidth(), out_file);
    WriteU32(m_tileset->GetTileHeight(), out_file);
    WriteU32((uint32_t)m_tileset_lock, out_file);
    if(m_tileset_lock) WriteTexture(*(m_tileset->GetTexture()), out_file);
    for(int y = 0; y < m_tilegrid->GridHeight(); ++y) {
        for(int x = 0; x < m_tilegrid->GridWidth(); ++x) {
            fputc(m_tilegrid->GetTile(x, y), out_file);
        }
    }
    fputs("lit", out_file);             //2nd signature
}

LayerTilemap *LayerTilemap::Load(EditorState *editor, FILE *in_file) {
    if(in_file == nullptr) return nullptr;
    LayerTilemap *r = nullptr;

    char sig[4] = { 0 };
    fgets(sig, 4, in_file);
    if(std::string(sig) != "til") {
        TRACE("No signature 'til'\n");
        return nullptr;
    }

    int tile_width = (int) ReadU32(in_file);
    int tile_height = (int) ReadU32(in_file);
    if(tile_width <= 0 || tile_height <= 0) {
        TRACE("Invalid tile size (%i * %i)", tile_width, tile_height);
        return nullptr;
    }

    int lock = (int) ReadU32(in_file);
    if(lock) {
        r = new LayerTilemap()
    }

    return nullptr;
}
