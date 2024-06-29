#include "LayerSpawnRegions.h"

#include "utils/FileOp.h"
#include "engine/util/Trace.h"
#include "EditorState.h"

LayerSpawnRegions::LayerSpawnRegions(EditorState *editor) : Layer(editor, "Spawn regions", LayerType_SpawnRegions) {
    m_selected = false;
    m_spawn_regions.emplace_back(editor, 0, 0, Editor()->GetTerrainWidth()/2.f, Editor()->GetTerrainHeight(), 0);
    m_spawn_regions.emplace_back(editor, Editor()->GetTerrainWidth()/2.f, 0, Editor()->GetTerrainWidth()/2.f, Editor()->GetTerrainHeight(), 1);
}

LayerSpawnRegions::~LayerSpawnRegions() {

}

void LayerSpawnRegions::UpdateIfSelected() {
    Layer::UpdateIfSelected();
    for(EditorSpawnRegion &r : m_spawn_regions) r.Update();
}

void LayerSpawnRegions::Update() {
    Layer::Update();
    m_selected = (Editor()->GetCurrentLayer() == this);
}

void LayerSpawnRegions::PreDraw() {
    if(!m_selected) for(EditorSpawnRegion &r : m_spawn_regions) r.Draw();
}

void LayerSpawnRegions::Draw() {
    if(m_selected) for(EditorSpawnRegion &r : m_spawn_regions) r.Draw();
}


void LayerSpawnRegions::AddSpawnRegion(EditorSpawnRegion region) {
    m_spawn_regions.push_back(region);
}

void LayerSpawnRegions::Save(FILE *out_file) {
    Layer::Save(out_file);

    fputs("reg", out_file);             //signature
    WriteU32(m_spawn_regions.size(), out_file);         //Region count
    for(EditorSpawnRegion &region : m_spawn_regions) {
        region.Save(out_file);
    }
    fputs("ger", out_file);             //2nd signature
}

LayerSpawnRegions *LayerSpawnRegions::Load(EditorState *editor, FILE *in_file) {
    if(in_file == nullptr) return nullptr;

    char sig[4] = { 0 };
    fgets(sig, 4, in_file);
    if(std::string(sig) != "reg") {
        TRACE("No 'reg' signature\n");
        return nullptr;
    }

    int region_count = (int) ReadU32(in_file);
    if(region_count <= 0) {
        TRACE("Invalid region count (%i) \n", region_count);
        return nullptr;
    }

    LayerSpawnRegions *r = new LayerSpawnRegions(editor);
    r->m_spawn_regions.clear();

    for(int i = 0; i < region_count; ++i) {
        float x = ReadF32(in_file);
        float y = ReadF32(in_file);
        float w = ReadF32(in_file);
        float h = ReadF32(in_file);
        r->AddSpawnRegion(EditorSpawnRegion(editor, x, y, w, h, i));
    }

    sig[0] = 0;
    fgets(sig, 4, in_file);
    if(std::string(sig) != "ger") {
        TRACE("No 'ger' signature\n");
        delete r;
        return nullptr;
    }

    return r;
}
