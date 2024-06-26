#ifndef UI_FRAMEWORK_EDITORSTATE_H
#define UI_FRAMEWORK_EDITORSTATE_H

#include "engine/state/State.h"
#include "engine/util/Math.h"

#include <raylib.h>
#include <string>
#include <vector>

class EditorSpawnRegion;
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


    void CreateNew(int w, int h, int tile_w, int tile_h, Vector2 size_m);
    void Save(std::string file_name);

    int GetPaletteIndex();
    void SetPaletteIndex(int index);

    float GetTileWidthM();
    float GetTileHeightM();

    inline int GetCurrentLayer() { return m_current_layer; }
    inline void SetCurrentlayer(int l) { m_current_layer = l; }
    inline TilemapTerrain *GetTerrain() { return m_terrain; }

private:
    void UpdateEditTerrain();

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

    TilemapTerrain *m_terrain;
    std::vector<EditorSpawnRegion> m_spawn_regions;

    int m_current_layer;            //0 = teams, 1 = collisions, 2 = tilemap
    unsigned char m_tilemap_palette_index;
    unsigned char m_collision_palette_index;

    bool m_preview_hovered_tile;
    Vector2i m_hovered_tile;
};


#endif //UI_FRAMEWORK_EDITORSTATE_H
