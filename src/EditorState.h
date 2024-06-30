#ifndef UI_FRAMEWORK_EDITORSTATE_H
#define UI_FRAMEWORK_EDITORSTATE_H

#include "engine/state/State.h"
#include "engine/util/Math.h"

#include <raylib.h>
#include <string>
#include <vector>

class EditorLevel;
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

    //These are just aliases to the m_level methods
    void ResizeGrid(int grid_w, int grid_h);
    void ResizeTerrain(Vector2 size_m);
    void Resize(int grid_w, int grid_h, Vector2 size_m);
    void Save(std::string file_name);

    int GetPaletteIndex();
    void SetPaletteIndex(int index);

    inline WindowManager *GetWindowManager() { return m_window_manager; }

    inline EditorLevel *GetLevel() { return m_level; }
    inline int GetCurrentLayerIndex() { return m_current_layer; }
    inline void SetCurrentLayerIndex(int l) { m_current_layer = l; }
    Layer *GetCurrentLayer();

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

    EditorLevel *m_level;

    int m_current_layer;

    bool m_preview_hovered_tile;
    Vector2i m_hovered_tile;
};


#endif //UI_FRAMEWORK_EDITORSTATE_H
