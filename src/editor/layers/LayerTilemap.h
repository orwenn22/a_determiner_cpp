#ifndef UI_FRAMEWORK_LAYERTILEMAP_H
#define UI_FRAMEWORK_LAYERTILEMAP_H

#include "Layer.h"

class TileGrid;
class Tileset;

class LayerTilemap : public Layer {
public:
    LayerTilemap(EditorState *editor, std::string name);
    ~LayerTilemap() override;

    void ResizeGrid(int grid_w, int grid_h);

    //void Update() override;
    void UpdateIfSelected() override;
    void Draw() override;
    void HandleFileDrag(std::string file_name) override;

    //The layer will have ownership ot the tileset object passed here. It should be allocated on the heap.
    void SetTileset(Tileset *tileset, bool lock = false);
    inline bool IsTilesetLocked() { return m_tileset_lock; }
    inline Tileset *GetTileset() { return m_tileset; }

    inline int GetPaletteIndex() { return m_palette_index; }
    void SetPaletteIndex(int index);

    //For the palette widget
    inline int GetPaletteScroll() { return m_palette_widget_scroll; }
    void SetPaletteScroll(int scroll);

private:
    TileGrid *m_tilegrid;
    Tileset *m_tileset;

    int m_palette_index;
    int m_palette_widget_scroll;
    bool m_tileset_lock;
};


#endif //UI_FRAMEWORK_LAYERTILEMAP_H
