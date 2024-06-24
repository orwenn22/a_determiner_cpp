#ifndef UI_FRAMEWORK_EDITORPALETTEWIDGET_H
#define UI_FRAMEWORK_EDITORPALETTEWIDGET_H

#include "engine/widgets/Widget.h"

class EditorState;

class EditorPaletteWidget : public Widget {
public:
    EditorPaletteWidget(EditorState *editor, int x, int y, int w, int h);

    void Update() override;
    void Draw() override;

private:
    EditorState *m_editor;

    int m_first_tile_scroll;
    int m_zoom;
};


#endif //UI_FRAMEWORK_EDITORPALETTEWIDGET_H
