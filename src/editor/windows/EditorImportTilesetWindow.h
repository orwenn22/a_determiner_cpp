#ifndef A_DETERMINER_EDITORIMPORTTILESETWINDOW_H
#define A_DETERMINER_EDITORIMPORTTILESETWINDOW_H

#include "engine/windows/Window.h"

class EditorState;
class LayerTilemap;

class EditorImportTilesetWindow : public Window {
public:
    EditorImportTilesetWindow(EditorState *editor, LayerTilemap *layer, std::string file_path);
    ~EditorImportTilesetWindow() override;

private:
    EditorState *m_editor;
    LayerTilemap *m_layer;
    std::string m_file_path;

    int m_tile_width_px;
    int m_tile_height_px;
};


#endif //A_DETERMINER_EDITORIMPORTTILESETWINDOW_H
