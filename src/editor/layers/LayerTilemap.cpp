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
    fputs("til", out_file);             //signature
    for(int y = 0; y < m_tilegrid->GridHeight(); ++y) {
        for(int x = 0; x < m_tilegrid->GridWidth(); ++x) {
            fputc(m_tilegrid->GetTile(x, y), out_file);
        }
    }
    fputs("lit", out_file);             //2nd signature
}


//TODO : make the two functions below independant of LayerTilemap ? Maybe we should have a WriteTexture and ReadTexture in FileOp.h ?

void LayerTilemap::SaveTileset(FILE *out_file) {
    if(out_file == nullptr || m_tileset == nullptr || !m_tileset->Usable()) return;
    Image tileset_image = LoadImageFromTexture(*(m_tileset->GetTexture()));
    fputs("img", out_file);                             //signature
    WriteU32(tileset_image.width, out_file);                //image width
    WriteU32(tileset_image.height, out_file);               //image height
    WriteU32(m_tileset->GetTileWidth(), out_file);          //tile width
    WriteU32(m_tileset->GetTileHeight(), out_file);         //tile height
    for(int y = 0; y < tileset_image.height; ++y) {
        for(int x = 0; x < tileset_image.width; ++x) {
            Color c = GetImageColor(tileset_image, x, y);
            fputc(c.r, out_file);
            fputc(c.g, out_file);
            fputc(c.b, out_file);
            fputc(c.a, out_file);
        }
    }
    fputs("gmi", out_file);                             //2nd signature
    UnloadImage(tileset_image);
}

bool LayerTilemap::LoadTileset(FILE *in_file) {
    if(in_file == nullptr) return false;

    char sig[4] = { 0 };
    fgets(sig, 4, in_file);
    if(std::string(sig) != "img") {
        TRACE("No 'img' signature");
        return false;
    }

    int tileset_width = (int) ReadU32(in_file);
    int tileset_height = (int) ReadU32(in_file);
    int tile_width = (int) ReadU32(in_file);
    int tile_height = (int) ReadU32(in_file);
    if(tileset_width <= 0 || tileset_height <= 0 || tile_width <= 0 || tile_height <= 0 || tile_width > tileset_width || tile_height > tileset_height) {
        TRACE("Invalid size parameter\n");
        return false;
    }

    Image tileset_image = GenImageColor(tileset_width, tileset_height, BLANK);
    for(int y = 0; y < tileset_height; ++y) {
        for(int x = 0; x < tileset_width; ++x) {
            Color c;
            c.r = fgetc(in_file);
            c.g = fgetc(in_file);
            c.b = fgetc(in_file);
            c.a = fgetc(in_file);
            ImageDrawPixel(&tileset_image, x, y, c);
        }
    }

    fgets(sig, 4, in_file);
    if(std::string(sig) != "gmi") {
        TRACE("No 'gmi' signature");
        UnloadImage(tileset_image);
        return false;
    }

    Texture tileset_texture = LoadTextureFromImage(tileset_image);
    Tileset *new_tileset = new Tileset(&tileset_texture, tile_width, tile_height, true);
    SetTileset(new_tileset);

    UnloadImage(tileset_image);
    return true;
}
