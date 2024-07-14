#include "LayerTilemap.h"

#include <utility>

#include "editor/windows/EditorImportTilesetWindow.h"
#include "editor/EditorState.h"
#include "engine/windows/WindowManager.h"
#include "engine/util/Trace.h"
#include "engine/Globals.h"
#include "engine/TileGrid.h"
#include "engine/Tileset.h"
#include "utils/FileOp.h"
#include "../EditorLevel.h"
#include "GlobalResources.h"


LayerTilemap::LayerTilemap(EditorLevel *level, std::string name) : Layer(level, std::move(name), LayerType_Tilemap) {
    m_tileset_lock = false;
    m_palette_widget_scroll = 0;
    m_tilegrid = new TileGrid(level->GridWidth(), level->GridHeight());
    m_tileset = nullptr;
    m_palette_index = 1;
    m_painting = false;
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


void LayerTilemap::Update(EditorState *editor) {
    if(editor->GetCurrentLayer() != this) m_painting = false;
}

void LayerTilemap::UpdateIfSelected(EditorState *editor) {
    Layer::UpdateIfSelected(editor);

    if(m_painting) {
        m_tilegrid->SetTile(editor->GetHoveredTileX(), editor->GetHoveredTileY(), m_palette_index);
        //TRACE("painting on %i %i with %i\n", Editor()->GetHoveredTileX(), Editor()->GetHoveredTileY(), m_palette_index);
    }

    if(IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
        m_painting = false;
        return;
    }

    if(IsMouseUsed()) return;

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_painting = true;
    UseMouse();
}

void LayerTilemap::Draw(EditorState *editor) {
    Layer::Draw(editor);
    if(m_tileset == nullptr || !m_tileset->Usable()) return;
    m_tilegrid->MDraw(m_tileset, {0.f, 0.f, Level()->GetTerrainWidth(), Level()->GetTerrainHeight()});
}

void LayerTilemap::HandleFileDrag(EditorState *editor, std::string file_name) {
    if(m_tileset_lock) return;      //TODO : display error ?
    editor->GetWindowManager()->AddWindow(new EditorImportTilesetWindow(editor, this, file_name));
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


bool LayerTilemap::Savable() {
    return (m_tileset != nullptr && m_tileset->Usable());
}

//NOTE : this does not save the size of the grid because it is stored in the EditorLevel
bool LayerTilemap::Save(FILE *out_file) {
    if(m_tileset == nullptr || !m_tileset->Usable()) {
        TRACE("Can't save unusable tileset :(\n");
        return false;
    }

    //Save layer header
    (void) Layer::Save(out_file);

    //First signature
    fputs("til", out_file);

    //Save tile width and tile height
    WriteU32(m_tileset->GetTileWidth(), out_file);
    WriteU32(m_tileset->GetTileHeight(), out_file);

    //Save lock
    WriteU32((uint32_t)m_tileset_lock, out_file);

    //If the tileset is unlocked (aka not the collision layer) then we save the texture
    if(!m_tileset_lock) WriteTexture(*(m_tileset->GetTexture()), out_file);

    //Save tilemat data
    for(int y = 0; y < m_tilegrid->GridHeight(); ++y) {
        for(int x = 0; x < m_tilegrid->GridWidth(); ++x) {
            fputc(m_tilegrid->GetTile(x, y), out_file);
        }
    }

    //End signature
    fputs("lit", out_file);
    return true;
}

//This should be called by Layer::Load
LayerTilemap *LayerTilemap::Load(EditorLevel *level, FILE *in_file) {
    if(in_file == nullptr) return nullptr;
    LayerTilemap *r = nullptr;

    //Check first signature
    if(!CheckSignature("til", 3, in_file)) {
        TRACE("No signature 'til'\n");
        return nullptr;
    }

    //Read size
    int tile_width = (int) ReadU32(in_file);
    int tile_height = (int) ReadU32(in_file);
    TRACE("Tile layer have size (%i * %i)\n", tile_width, tile_height);
    if(tile_width <= 0 || tile_height <= 0) {
        TRACE("Invalid size\n");
        return nullptr;
    }

    //Read lock
    int lock = (int) ReadU32(in_file);
    TRACE("tileset lock : %i\n", lock);

    r = new LayerTilemap(level, "temp lmao");       //The name is set when we return to Layer::Load
    if(lock) {
        //In this case we don't need to load the bitmap, we just use the collision tileset
        r->SetTileset(new Tileset(Res::collisions_sprite, 16, 16));
    }
    else {
        //Load bitmap
        Texture tileset_texture = ReadTexture(in_file);
        if(tileset_texture.id <= 0) {
            TRACE("Failed to load texture\n");
            delete r;
            return nullptr;
        }

        //Make TextureRef from bitmap
        TextureRef tileset_textureref = TextureRef::construct(tileset_texture);
        if(!tileset_textureref.IsValid()) {
            TRACE("Failed to make TextureRef, this should NEVER get reached !!!! :(\n");
            //UnloadTexture(tileset_texture);
            delete r;
            return nullptr;
        }
        r->SetTileset(new Tileset(tileset_textureref, tile_width, tile_height));
    }

    //Load tilemap
    for(int y = 0; y < level->GridHeight(); ++y) {
        for(int x = 0; x < level->GridWidth(); ++x) {
            int tile = getc(in_file);
            if(tile == EOF) {
                TRACE("Reached EOF while parsing tilemap\n");
                delete r;
                return nullptr;
            }
            r->m_tilegrid->SetTile(x, y, tile);
        }
    }

    //Check end signature
    if(!CheckSignature("lit", 3, in_file)) {
        TRACE("No signature 'lit' (offset 0x%lx)\n", ftell(in_file));
        delete r;
        return nullptr;
    }

    return r;
}


unsigned char LayerTilemap::GetTile(int x, int y) {
    return m_tilegrid->GetTile(x, y);
}
