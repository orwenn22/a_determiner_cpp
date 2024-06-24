#include "EditorState.h"

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
#include "windows/EditorLayerWindow.h"
#include "windows/NewLevelWindow.h"
#include "GlobalResources.h"


EditorState::EditorState() {
    m_preview_hovered_tile = false;
    m_hovered_tile = {0, 0};

    m_editing_collisions = false;
    m_tilemap_palette_index = 1;
    m_collision_palette_index = 1;

    m_bg = new TiledBackground(&Res::menubg_grayscale);
    m_bg->SetColor({20, 20, 20, 255});
    m_terrain = nullptr;

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
    delete m_terrain;
    delete m_camera;
    delete m_widgets;
}

void EditorState::Update(float dt) {
    HandleFilesDragAndDrop();
    if(IsKeyPressed(KEY_TAB)) m_editing_collisions ^= 1;
    if(IsKeyPressed(KEY_N)) {
        //TODO : check if windows is already opened ?
        m_window_manager->AddWindow(new NewLevelWindow(this));
    }


    m_window_manager->Update();
    m_widgets->Update();

    if(m_terrain == nullptr) m_camera->SetCameraTopLeft({0.f, 0.f});
    else HandleDragCamera((float)GetMouseX(), (float)GetMouseY());

    if(m_terrain != nullptr) UpdateEditTerrain();

    m_bg->Update(dt);
}

void EditorState::Draw() {
    Metrics::SetGraphicsCam(m_camera);
    m_bg->Draw();
    if(m_terrain) {
        m_terrain->Draw();
        if(m_editing_collisions) m_terrain->DrawCollisions();
        DrawHoveredTilePreview();
    }
    m_widgets->Draw();
    m_window_manager->Draw();
}


void EditorState::CreateNew(int w, int h, int tile_w, int tile_h, Vector2 size_m) {
    if(w <= 0 || h <= 0 || size_m.x <= 0 || size_m.y <= 0) return;
    m_window_manager->Clear();
    m_widgets->Clear();
    delete m_terrain;
    m_terrain = new TilemapTerrain(size_m, tile_w, tile_h, w, h);
    m_camera->origin_x = 10.f;
    m_camera->origin_y = 10.f;
    m_editing_collisions = false;
    m_tilemap_palette_index = 1;
    m_collision_palette_index = 1;

    m_window_manager->AddWindow(new EditorLayerWindow(this, 15, 15));
}


/////////////////////////////////////
//// PRIVATE

void EditorState::UpdateEditTerrain() {
    Vector2 mouse_pos = m_camera->ConvertAbsoluteToMeters(GetMouseX(), GetMouseY());
    m_hovered_tile = m_terrain->GetTilePosition(mouse_pos);

    if(IsMouseUsed()) {
        m_preview_hovered_tile = false;
        return;
    }

    m_preview_hovered_tile = true;
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if(m_editing_collisions) m_terrain->GetCollision()->SetTile(m_hovered_tile.x, m_hovered_tile.y, m_collision_palette_index);
        else m_terrain->GetTilemap()->SetTile(m_hovered_tile.x, m_hovered_tile.y, m_tilemap_palette_index);
    }
}


void EditorState::DrawHoveredTilePreview() {
    if(!m_preview_hovered_tile) return;
    Rectangle dest = {
            m_terrain->X() + m_terrain->TileWidth()*(float)m_hovered_tile.x,
            m_terrain->Y() + m_terrain->TileHeight()*(float)m_hovered_tile.y,
            m_terrain->TileWidth(),
            m_terrain->TileHeight()
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

    if(extension == "png") {
        Texture new_texture = LoadTexture(file_path.c_str());
        m_terrain->GetTileset()->SetTexture(&new_texture, true);
        TRACE("Imported texture %s\n", file_path.c_str());
    }
    else {
        TRACE("Unknown %s\n", file_path.c_str());
    }
}
