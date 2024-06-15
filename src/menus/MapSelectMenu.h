#ifndef UI_FRAMEWORK_MAPSELECTMENU_H
#define UI_FRAMEWORK_MAPSELECTMENU_H

#include <string>
#include <vector>

#include "engine/state/State.h"

//TODO : drag and drop to load map.
//       This should detect if the dragged file is a map or an image. If it is an image, it should put it in ext/res,
//       if it is a map, it should put it in ext/maps. If it is a map, we should reload the menu and go to the last page

class Label;
class MapLoader;
class TiledBackground;
class WidgetManager;


struct MapInfo {
    std::string map_path;
    bool is_external;

    MapInfo(std::string _map_path, bool _is_external) : map_path(_map_path), is_external(_is_external) {}
};


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

    void HandleFilesDragAndDrop();
    static bool HandleFileDragAndDrop(std::string dragged_path);


    TiledBackground *m_bg;
    WidgetManager *m_permanent_widgets;
    WidgetManager *m_temporary_widgets;

    std::vector<MapInfo> m_map_names;
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
