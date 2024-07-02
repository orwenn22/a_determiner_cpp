#ifndef A_DETERMINER_EDITORPALETTEWIDGET_H
#define A_DETERMINER_EDITORPALETTEWIDGET_H

#include "engine/widgets/Widget.h"

class EditorState;

class EditorPaletteWidget : public Widget {
public:
    EditorPaletteWidget(EditorState *editor, int x, int y, int w, int h);

    void Update() override;
    void Draw() override;

private:
    EditorState *m_editor;
    int m_zoom;
};


#endif //A_DETERMINER_EDITORPALETTEWIDGET_H
