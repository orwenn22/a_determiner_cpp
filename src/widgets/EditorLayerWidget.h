#ifndef UI_FRAMEWORK_EDITORLAYERWIDGET_H
#define UI_FRAMEWORK_EDITORLAYERWIDGET_H

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


#endif //UI_FRAMEWORK_EDITORLAYERWIDGET_H
