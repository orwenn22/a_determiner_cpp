#ifndef A_DETERMINER_EDITORLEVEL_H
#define A_DETERMINER_EDITORLEVEL_H

#include <raylib.h>
#include <string>
#include <vector>

class EditorState;
class Layer;

class EditorLevel {
public:
    EditorLevel(int grid_w, int grid_h, Vector2 size_m);
    ~EditorLevel();

    void EditorUpdate(EditorState *editor);
    void EditorDraw(EditorState *editor);

    void ResizeGrid(int grid_w, int grid_h);
    void ResizeTerrain(Vector2 size_m);
    void Resize(int grid_w, int grid_h, Vector2 size_m);

    float GetTileWidthM();
    float GetTileHeightM();

    inline int GridWidth() { return m_grid_width; }
    inline int GridHeight() { return m_grid_height; }
    inline float GetTerrainWidth() { return m_size_m.x; }
    inline float GetTerrainHeight() { return m_size_m.y; }

    inline int GetLayerCount() { return (int) m_layers.size(); }
    Layer *GetLayer(int index);
    Layer *GetLayer(std::string name);
    void AddLayer(Layer *l);

    bool Save(std::string file_name);
    static EditorLevel *Load(std::string file_name);


private:
    std::vector<Layer *> m_layers;      //index 0 is top layer
    int m_grid_width, m_grid_height;
    Vector2 m_size_m;
};


#endif //A_DETERMINER_EDITORLEVEL_H
