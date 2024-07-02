#ifndef A_DETERMINER_EDITORLAYERWINDOW_H
#define A_DETERMINER_EDITORLAYERWINDOW_H

#include "engine/windows/Window.h"

class EditorState;

class EditorLayerWindow : public Window {
public:
    EditorLayerWindow(EditorState *editor, int x, int y);
    ~EditorLayerWindow() override;
};


#endif //A_DETERMINER_EDITORLAYERWINDOW_H
