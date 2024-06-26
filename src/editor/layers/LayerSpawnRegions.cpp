#include "LayerSpawnRegions.h"

#include "EditorState.h"

LayerSpawnRegions::LayerSpawnRegions(EditorState *editor) : Layer(editor, "Spawn regions", LayerType_SpawnRegions) {
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
}

void LayerSpawnRegions::Draw() {
    Layer::Draw();
    for(EditorSpawnRegion &r : m_spawn_regions) r.Draw();
}

void LayerSpawnRegions::AddSpawnRegion(EditorSpawnRegion region) {
    m_spawn_regions.push_back(region);
}
