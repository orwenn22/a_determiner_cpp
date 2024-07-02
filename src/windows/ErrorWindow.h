#ifndef A_DETERMINER_ERRORWINDOW_H
#define A_DETERMINER_ERRORWINDOW_H

#include "engine/windows/Window.h"

class ErrorWindow : public Window {
public:
    ErrorWindow(int x, int y, std::string message);
    ~ErrorWindow() override;
};


#endif //A_DETERMINER_ERRORWINDOW_H
