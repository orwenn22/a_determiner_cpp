#ifndef A_DETERMINER_EDITORPALETTEWINDOW_H
#define A_DETERMINER_EDITORPALETTEWINDOW_H

#include "engine/windows/Window.h"

class EditorState;

class EditorPaletteWindow : public Window {
public:
    EditorPaletteWindow(EditorState *editor, int x, int y, int w, int h);
    ~EditorPaletteWindow() override;
};


#endif //A_DETERMINER_EDITORPALETTEWINDOW_H
