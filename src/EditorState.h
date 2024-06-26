#ifndef UI_FRAMEWORK_EDITORSTATE_H
#define UI_FRAMEWORK_EDITORSTATE_H

#include "engine/state/State.h"
#include "engine/util/Math.h"

#include <raylib.h>
#include <string>
#include <vector>

class EditorSpawnRegion;
class Layer;
class MetricsCamera;
class TilemapTerrain;
class TiledBackground;
class WidgetManager;
class WindowManager;

enum EditorLayerNames {
    Layer_Teams,
    Layer_Collisions,
    Layer_Tilemap
};

class EditorState : public State {
public:
    EditorState();
    ~EditorState() override;

    void Update(float dt) override;
    void Draw() override;


    void CreateNew(int grid_w, int grid_h, Vector2 size_m);
    void Save(std::string file_name);

    int GetPaletteIndex();
    void SetPaletteIndex(int index);

    float GetTileWidthM();
    float GetTileHeightM();

    inline WindowManager *GetWindowManager() { return m_window_manager; }

    inline int GetCurrentLayerIndex() { return m_current_layer; }
    inline void SetCurrentLayerIndex(int l) { m_current_layer = l; }
    inline Layer *GetCurrentLayer() { return m_layers[m_current_layer]; }
    inline int GetlayerCount() { return (int) m_layers.size(); }
    Layer *GetLayer(int index);

    inline int GridWidth() { return m_grid_width; }
    inline int GridHeight() { return m_grid_height; }
    inline float GetTerrainWidth() { return m_size_m.x; }
    inline float GetTerrainHeight() { return m_size_m.y; }
    inline int GetHoveredTileX() { return m_hovered_tile.x; }
    inline int GetHoveredTileY() { return m_hovered_tile.y; }

private:
    void UpdateHoveredTilePreview();

    void DrawHoveredTilePreview();

    void HandleDragCamera(float mouse_x, float mouse_y);        //Take mouse pos in pixel
    void HandleFilesDragAndDrop();
    void HandleFileDragAndDrop(std::string file_path);

    TiledBackground *m_bg;
    WidgetManager *m_widgets;
    WindowManager *m_window_manager;

    MetricsCamera *m_camera;
    bool m_cam_follow_mouse;
    Vector2 m_cam_mouse_offset;

    bool m_level_loaded;
    std::vector<Layer *> m_layers;      //index 0 is top layer
    int m_grid_width, m_grid_height;
    Vector2 m_size_m;

    int m_current_layer;

    bool m_preview_hovered_tile;
    Vector2i m_hovered_tile;
};


#endif //UI_FRAMEWORK_EDITORSTATE_H
