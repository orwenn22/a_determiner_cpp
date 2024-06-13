#ifndef UI_FRAMEWORK_MAPSELECTMENU_H
#define UI_FRAMEWORK_MAPSELECTMENU_H

#include <string>
#include <vector>

#include "engine/state/State.h"


class Label;
class MapLoader;
class TiledBackground;
class WidgetManager;

class MapSelectMenu : public State {
public:
    MapSelectMenu();
    ~MapSelectMenu() override;

    void Update(float dt) override;
    void Draw() override;


private:
    void UpdateMapNames();
    void SetPage(int page);
    void LoadMap(int index);
    void SetError(std::string error_message);


    TiledBackground *m_bg;
    WidgetManager *m_permanent_widgets;
    WidgetManager *m_temporary_widgets;

    std::vector<std::string> m_map_names;
    int m_page;     //current page index
    int m_page_count;
    std::vector<MapLoader *> m_loaders;

    //We need to keep a reference to these in order to update them
    Label *m_current_page_label;
    Label *m_page_count_label;
    Label *m_cwd_label;
    Label *m_error_label;
};


#endif //UI_FRAMEWORK_MAPSELECTMENU_H
