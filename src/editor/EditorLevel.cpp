#include "EditorLevel.h"

#include "layers/LayerSpawnRegions.h"
#include "layers/LayerTilemap.h"
#include "engine/Tileset.h"
#include "engine/util/Trace.h"
#include "utils/FileOp.h"
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


Layer *EditorLevel::GetLayer(int index) {
    if(index < 0 || index >= GetLayerCount()) return nullptr;
    return m_layers[index];
}

Layer *EditorLevel::GetLayer(std::string name) {
    for(Layer *l : m_layers) {
        if(l->Name() == name) return l;
    }
    return nullptr;
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


void EditorLevel::Save(std::string file_name) {
    //TODO : perform checks to ensure all layers are savable here

    FILE *out_file = fopen(file_name.c_str(), "w");
    if(out_file == nullptr) {
        TRACE("Failed to create/open file %s\n", file_name.c_str());
        return;     //TODO : return false ?
    }

    fputs("lev", out_file);

    //Save grid size
    WriteU32(m_grid_width, out_file);
    WriteU32(m_grid_height, out_file);

    //Save terrain size (m)
    WriteF32(m_size_m.x, out_file);
    WriteF32(m_size_m.y, out_file);

    //Save layer count
    WriteU32(m_layers.size(), out_file);

    //Save layers
    for(int i = 0; i < m_layers.size(); ++i) {
        m_layers[i]->Save(out_file);
    }

    //End signature
    fputs("vel", out_file);

    fclose(out_file);
    TRACE("Successfully saved as %s\n", file_name.c_str());
}


EditorLevel *EditorLevel::Load(std::string file_name) {
    FILE *in_file = fopen(file_name.c_str(), "r");
    if(in_file == nullptr) {
        TRACE("Could not open file %s\n", file_name.c_str());
        return nullptr;
    }

    //Check first signature
    if(!CheckSignature("lev", 3, in_file)) {
        fclose(in_file);
        TRACE("No 'lev' signature\n");
        return nullptr;
    }

    //Read grid size
    int grid_width = ReadU32(in_file);
    int grid_height = ReadU32(in_file);
    if(grid_width <= 0 || grid_height <= 0) {
        fclose(in_file);
        TRACE("Invalid grid size (%i * %i)\n", grid_width, grid_height);
        return nullptr;
    }

    //Read terrain size
    float terrain_width = ReadF32(in_file);
    float terrain_height = ReadF32(in_file);

    //Read layer count   TODO : prevent too many layers from being loaded ?
    int layer_count = ReadU32(in_file);
    if(layer_count < 0) {
        fclose(in_file);
        TRACE("Invalid layer count (%i)\n", layer_count);
        return nullptr;
    }

    EditorLevel *r = new EditorLevel(grid_width, grid_height, {terrain_width, terrain_height});

    //Load layers
    for(int i = 0; i < layer_count; ++i) {
        Layer *l = Layer::Load(r, in_file);
        if(l == nullptr) {
            TRACE("Failed to load layer %i\n", i);
            delete r;
            return nullptr;
        }
        r->AddLayer(l);
    }

    //Check end signature
    if(!CheckSignature("vel", 3, in_file)) {
        TRACE("No 'vel' signature\n");
        delete r;
        return nullptr;
    }

    return r;
}
