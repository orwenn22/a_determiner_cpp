#include "EditorState.h"

#include "editor/layers/Layer.h"
#include "editor/layers/LayerSpawnRegions.h"
#include "editor/layers/LayerTilemap.h"
#include "editor/windows/EditorLayerWindow.h"
#include "editor/windows/EditorPaletteWindow.h"
#include "editor/windows/NewLevelWindow.h"
#include "editor/windows/ResizeLevelWindow.h"
#include "editor/EditorSpawnRegion.h"
#include "engine/metrics/Graphics.h"
#include "engine/metrics/MetricsCamera.h"
#include "engine/util/Paths.h"
#include "engine/util/Trace.h"
#include "engine/widgets/Button.h"
#include "engine/widgets/Label.h"
#include "engine/widgets/WidgetManager.h"
#include "engine/windows/WindowManager.h"
#include "engine/Globals.h"
#include "engine/Tileset.h"
#include "engine/TileGrid.h"
#include "terrain/TilemapTerrain.h"
#include "utils/TiledBackground.h"
#include "GlobalResources.h"


EditorState::EditorState() {
    m_preview_hovered_tile = false;
    m_hovered_tile = {0, 0};

    m_current_layer = Layer_Tilemap;

    m_bg = new TiledBackground(&Res::menubg_grayscale);
    m_bg->SetColor({20, 20, 20, 255});

    m_level_loaded = false;
    m_size_m = {0.f, 0.f};
    m_grid_width = 0;
    m_grid_height = 0;

    m_camera = new MetricsCamera(10, 10, 16);

    m_window_manager = new WindowManager;

    m_widgets = new WidgetManager;

    Label *welcome1 = new Label(0, -8, 10, "Click the button bellow to create a new level,");
    Label *welcome2 = new Label(0, 8, 10, "or drag and drop a file to edit an existiong one.");
    Button *new_level_button = new Button(0, 25, 100, 20, "New level", [this]() {
        m_window_manager->AddWindow(new NewLevelWindow(this));
    });

    welcome1->SetAlignment(WidgetAlignment_Center);
    welcome2->SetAlignment(WidgetAlignment_Center);
    new_level_button->SetAlignment(WidgetAlignment_Center);
    new_level_button->CenterLabel();

    m_widgets->AddWidget(welcome1);
    m_widgets->AddWidget(welcome2);
    m_widgets->AddWidget(new_level_button);
}

EditorState::~EditorState() {
    for(Layer *l : m_layers) delete l;
    m_layers.clear();
    delete m_camera;
    delete m_widgets;
}

void EditorState::Update(float dt) {
    m_bg->Update(dt);

    HandleFilesDragAndDrop();
    if(IsKeyPressed(KEY_TAB)) m_current_layer = (m_current_layer+1)%(int)(m_layers.size());

    if(IsKeyDown(KEY_LEFT_CONTROL)) {
        if (IsKeyPressed(KEY_N)) {
            //TODO : check if windows is already opened ?
            m_window_manager->AddWindow(new NewLevelWindow(this));
        }
        else if(IsKeyPressed(KEY_R)) {
            m_window_manager->AddWindow(new ResizeLevelWindow(this));
        }
    }

    m_window_manager->Update();
    m_widgets->Update();

    if(m_level_loaded) {
        HandleDragCamera((float)GetMouseX(), (float)GetMouseY());
        m_layers[m_current_layer]->UpdateIfSelected();
        UpdateHoveredTilePreview();
        for(size_t i = 0; i < m_layers.size(); ++i) m_layers[i]->Update();
    }
    else {
        m_camera->SetCameraTopLeft({0.f, 0.f});
    }
}

void EditorState::Draw() {
    Metrics::SetGraphicsCam(m_camera);
    m_bg->Draw();
    if(m_level_loaded) {
        for(auto it = m_layers.rbegin(); it != m_layers.rend(); it++) (*it)->PreDraw();
        for(auto it = m_layers.rbegin(); it != m_layers.rend(); it++) (*it)->Draw();
        DrawHoveredTilePreview();
        Metrics::DrawRectangle(0, 0, GetTerrainWidth(), GetTerrainHeight(), RED, false);
    }
    m_widgets->Draw();
    m_window_manager->Draw();
}


void EditorState::CreateNew(int grid_w, int grid_h, Vector2 size_m) {
    if(grid_w <= 0 || grid_h <= 0 || size_m.x <= 0 || size_m.y <= 0) return;
    m_window_manager->Clear();
    m_widgets->Clear();
    for(Layer *l : m_layers) delete l;
    m_layers.clear();

    m_grid_width = grid_w;
    m_grid_height = grid_h;
    m_size_m = size_m;

    m_layers.push_back(new LayerSpawnRegions(this));
    LayerTilemap *collisions = new LayerTilemap(this, "Collisions");
    collisions->SetTileset(Res::collisions_tileset->WeakCopy(), true);
    m_layers.push_back(collisions);
    m_layers.push_back(new LayerTilemap(this, "Tilemap"));

    m_camera->origin_x = 10.f;
    m_camera->origin_y = 10.f;
    m_current_layer = 2;

    m_window_manager->AddWindow(new EditorLayerWindow(this, 15, 15));
    m_window_manager->AddWindow(new EditorPaletteWindow(this, 30, 30, 250, 250));

    m_level_loaded = true;
}


void EditorState::ResizeGrid(int grid_w, int grid_h) {
    if(!m_level_loaded ||grid_w <= 0 || grid_h <= 0) return;        //TODO : error message ?
    m_grid_width = grid_w;
    m_grid_height = grid_h;
    for(Layer *l : m_layers) {
        if(l->Type() != LayerType_Tilemap) continue;
        ((LayerTilemap *)l)->ResizeGrid(grid_w, grid_h);
    }
}

void EditorState::ResizeTerrain(Vector2 size_m) {
    if(!m_level_loaded) return;
    if(size_m.x <= 0 || size_m.y <= 0) return;
    m_size_m = size_m;
}

void EditorState::Resize(int grid_w, int grid_h, Vector2 size_m) {
    if(!m_level_loaded) return;
    ResizeGrid(grid_w, grid_h);
    ResizeTerrain(size_m);
}


void EditorState::Save(std::string file_name) {
    if(!m_level_loaded) return;

    //TODO : save size info
    //TODO : save spawn regions
    //TODO : save tileset bitmap
    //TODO : save terrain tilemap
    //TODO : save terrain collision mask
}


int EditorState::GetPaletteIndex() {
    Layer *l = GetCurrentLayer();
    if(l == nullptr || l->Type() != LayerType_Tilemap) return 0;
    return ((LayerTilemap *)l)->GetPaletteIndex();
}

void EditorState::SetPaletteIndex(int index) {
    Layer *l = GetCurrentLayer();
    if(l == nullptr || l->Type() != LayerType_Tilemap) return;
    if(index < 0) index = 0;
    ((LayerTilemap *)l)->SetPaletteIndex(index);
}


float EditorState::GetTileWidthM() {
    if(!m_level_loaded) return 0;
    return m_size_m.x/(float)m_grid_width;
}

float EditorState::GetTileHeightM() {
    if(!m_level_loaded) return 0;
    return m_size_m.y/(float)m_grid_height;
}


Layer *EditorState::GetLayer(int index) {
    if(index < 0 || index >= m_layers.size()) return nullptr;
    return m_layers[index];
}


/////////////////////////////////////
//// PRIVATE

void EditorState::UpdateHoveredTilePreview() {
    if(!m_level_loaded) return;

    Vector2 mouse_pos = m_camera->ConvertAbsoluteToMeters(GetMouseX(), GetMouseY());
    m_hovered_tile = {
            (int)(mouse_pos.x / GetTileWidthM()) - (mouse_pos.x < 0),
            (int)(mouse_pos.y / GetTileHeightM()) - (mouse_pos.y < 0)
    };

    m_preview_hovered_tile = !IsMouseUsed();
}


void EditorState::DrawHoveredTilePreview() {
    if(!m_preview_hovered_tile) return;
    Rectangle dest = {
            GetTileWidthM()*(float)m_hovered_tile.x,
            GetTileHeightM()*(float)m_hovered_tile.y,
            GetTileWidthM(),
            GetTileHeightM()
    };
    Metrics::DrawRectangle(dest , GREEN, false);

}


void EditorState::HandleDragCamera(float mouse_x, float mouse_y) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && !IsMouseUsed()) {
        m_cam_follow_mouse = true;
        m_cam_mouse_offset = {m_camera->origin_x - mouse_x, m_camera->origin_y - mouse_y};
    }
    if(IsMouseButtonUp(MOUSE_BUTTON_RIGHT)) {
        m_cam_follow_mouse = false;
    }

    if(m_cam_follow_mouse) {
        m_camera->origin_x = m_cam_mouse_offset.x + mouse_x;
        m_camera->origin_y = m_cam_mouse_offset.y + mouse_y;
    }
    else {
        if(IsMouseUsed()) return;

        // Handle zoom
        float mouse_wheel = GetMouseWheelMove();
        if(mouse_wheel > 0) m_camera->SetPixelsPerMeter((float)m_camera->PixelsPerMeter() * (1.f+mouse_wheel));
        else if(mouse_wheel < 0) m_camera->SetPixelsPerMeter((float)m_camera->PixelsPerMeter() / (1.f-mouse_wheel));
    }
}

void EditorState::HandleFilesDragAndDrop() {
    if(!IsFileDropped()) return;

    FilePathList dropped_files = LoadDroppedFiles();
    for(int i = 0; i < dropped_files.count; ++i) HandleFileDragAndDrop(dropped_files.paths[i]);
    UnloadDroppedFiles(dropped_files);
}

void EditorState::HandleFileDragAndDrop(std::string file_path) {
    std::string extension = GetExtensionFromPath(file_path);

    //if(file_is_level(file_path)) {
    //    load(file_path)
    //}
    //else {
        if(GetCurrentLayer() != nullptr) GetCurrentLayer()->HandleFileDrag(file_path);
    //}
}
