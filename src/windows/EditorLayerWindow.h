#ifndef UI_FRAMEWORK_EDITORLAYERWINDOW_H
#define UI_FRAMEWORK_EDITORLAYERWINDOW_H

#include "engine/windows/Window.h"

class EditorState;

class EditorLayerWindow : public Window {
public:
    EditorLayerWindow(EditorState *editor, int x, int y);
    ~EditorLayerWindow() override;
};


#endif //UI_FRAMEWORK_EDITORLAYERWINDOW_H
