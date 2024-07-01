#ifndef UI_FRAMEWORK_ERRORWINDOW_H
#define UI_FRAMEWORK_ERRORWINDOW_H

#include "engine/windows/Window.h"

class ErrorWindow : public Window {
public:
    ErrorWindow(int x, int y, std::string message);
    ~ErrorWindow() override;
};


#endif //UI_FRAMEWORK_ERRORWINDOW_H
