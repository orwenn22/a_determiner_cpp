#ifndef A_DETERMINER_CREDITSMENU_H
#define A_DETERMINER_CREDITSMENU_H

#include "engine/state/State.h"

class TiledBackground;
class WidgetManager;

class CreditsMenu : public State {
public:
    CreditsMenu();
    ~CreditsMenu() override;

    void Update(float dt) override;
    void Draw() override;

    inline const char *GetIdentifier() override { return "CreditsMenu"; }

private:
    TiledBackground *m_bg;
    WidgetManager *m_widgets;
};


#endif //A_DETERMINER_CREDITSMENU_H
