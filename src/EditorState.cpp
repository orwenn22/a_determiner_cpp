#include "EditorState.h"

#include "terrain/TilemapTerrain.h"
#include "utils/TiledBackground.h"
#include "GlobalResources.h"

EditorState::EditorState() {
    m_bg = new TiledBackground(&Res::menubg_grayscale);
    m_bg->SetColor({20, 20, 20, 255});
    m_terrain = nullptr;
}

EditorState::~EditorState() {
    delete m_terrain;
}

void EditorState::Update(float dt) {


    m_bg->Update(dt);
}

void EditorState::Draw() {
    m_bg->Draw();
}
