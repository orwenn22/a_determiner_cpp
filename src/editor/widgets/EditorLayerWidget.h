#ifndef A_DETERMINER_EDITORLAYERWIDGET_H
#define A_DETERMINER_EDITORLAYERWIDGET_H

#include "engine/widgets/Widget.h"

class EditorState;

//TODO : implement scrolling
class EditorLayerWidget : public Widget {
public:
    EditorLayerWidget(EditorState *editor, int x, int y, int w, int h);
    ~EditorLayerWidget() override;

    void Update() override;
    void Draw() override;

private:
    EditorState *m_editor;
};


#endif //A_DETERMINER_EDITORLAYERWIDGET_H
