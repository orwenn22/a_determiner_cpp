#include "EditorLevel.h"

#include "layers/LayerSpawnRegions.h"
#include "layers/LayerTilemap.h"
#include "engine/Tileset.h"
#include "engine/util/Trace.h"
#include "EditorState.h"
#include "GlobalResources.h"

EditorLevel::EditorLevel(int grid_w, int grid_h, Vector2 size_m) {
    //for(Layer *l : m_layers) delete l;
    m_layers.clear();

    m_grid_width = grid_w;
    m_grid_height = grid_h;
    m_size_m = size_m;

    //This is done inside the editor's CreateNew(), but let's keep this here just in case
    // m_layers.push_back(new LayerSpawnRegions(this));
    // LayerTilemap *collisions = new LayerTilemap(this, "Collisions");
    // collisions->SetTileset(Res::collisions_tileset->WeakCopy(), true);
    // m_layers.push_back(collisions);
    // m_layers.push_back(new LayerTilemap(this, "Tilemap"));
}

EditorLevel::~EditorLevel() {
    for(Layer *l : m_layers) delete l;
    m_layers.clear();
}


void EditorLevel::EditorUpdate(EditorState *editor) {
    if(editor == nullptr) return;
    m_layers[editor->GetCurrentLayerIndex()]->UpdateIfSelected(editor);
    for(size_t i = 0; i < m_layers.size(); ++i) m_layers[i]->Update(editor);
}

void EditorLevel::EditorDraw(EditorState *editor) {
    if(editor == nullptr) return;
    for(auto it = m_layers.rbegin(); it != m_layers.rend(); it++) (*it)->PreDraw(editor);
    for(auto it = m_layers.rbegin(); it != m_layers.rend(); it++) (*it)->Draw(editor);
    //The outline is drawn by the EditorState
}


float EditorLevel::GetTileWidthM() {
    return m_size_m.x/(float)m_grid_width;
}

float EditorLevel::GetTileHeightM() {
    return m_size_m.y/(float)m_grid_height;
}


void EditorLevel::ResizeGrid(int grid_w, int grid_h) {
    if(grid_w <= 0 || grid_h <= 0) return;        //TODO : error message ?
    m_grid_width = grid_w;
    m_grid_height = grid_h;
    for(Layer *l : m_layers) {
        if(l->Type() != LayerType_Tilemap) continue;
        ((LayerTilemap *)l)->ResizeGrid(grid_w, grid_h);
    }
}

void EditorLevel::ResizeTerrain(Vector2 size_m) {
    if(size_m.x <= 0 || size_m.y <= 0) return;
    m_size_m = size_m;
}

void EditorLevel::Resize(int grid_w, int grid_h, Vector2 size_m) {
    ResizeGrid(grid_w, grid_h);
    ResizeTerrain(size_m);
}

void EditorLevel::Save(std::string file_name) {
    //TODO : save size info
    //TODO : save spawn regions
    //TODO : save tileset bitmap
    //TODO : save terrain tilemap
    //TODO : save terrain collision mask
}


Layer *EditorLevel::GetLayer(int index) {
    if(index < 0 || index >= GetLayerCount()) return nullptr;
    return m_layers[index];
}

void EditorLevel::AddLayer(Layer *l) {
    if(l == nullptr) return;
    if(l->Level() == this) m_layers.push_back(l);
    else if(l->Level() == nullptr) {
        TRACE("Adding a layer with a level set to nullptr\n");      //TODO : Maybe we shouldn't add it ? Or maybe we sould set the m_level member of the layer to be 'this' ?
        m_layers.push_back(l);
    }
    else {
        TRACE("The layer is in another level\n");
    }
}
