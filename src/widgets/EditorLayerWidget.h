#ifndef UI_FRAMEWORK_EDITORLAYERWIDGET_H
#define UI_FRAMEWORK_EDITORLAYERWIDGET_H

#include "engine/widgets/Widget.h"

class EditorState;

//Quick and dirty widget to select the layer in the editor. Right now everything is hardcoded, which is not good,
//so maybe in the future it would be nice to allow the editor and the tilemap terrain to have a dynamic number of
//layers.
class EditorLayerWidget : public Widget {
public:
    EditorLayerWidget(EditorState *editor, int x, int y);
    ~EditorLayerWidget() override;

    void Update() override;
    void Draw() override;

private:
    EditorState *m_editor;
};


#endif //UI_FRAMEWORK_EDITORLAYERWIDGET_H
