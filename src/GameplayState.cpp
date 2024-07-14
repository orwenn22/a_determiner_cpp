#include "GameplayState.h"

#include <raylib.h>

#include "editor/EditorState.h"
#include "engine/Globals.h"
#include "engine/metrics/Graphics.h"
#include "engine/metrics/MetricsCamera.h"
#include "engine/object/ObjectManager.h"
#include "engine/state/StateManager.h"
#include "engine/tooltip/Tooltip.h"
#include "engine/util/Trace.h"
#include "engine/widgets/Label.h"
#include "engine/widgets/WidgetManager.h"
#include "engine/widgets/Widget.h"
#include "engine/windows/WindowManager.h"
#include "menus/PauseMenu.h"
#include "menus/PostGameMenu.h"
#include "objects/actors/Player.h"
#include "objects/collectibles/Constructors.h"
#include "terrain/BitmapTerrain.h"
#include "widgets/PlayerIndicator.h"
#include "windows/CollectibleSpawnWindow.h"
#include "GlobalResources.h"
#include "Teams.h"


//GameplayState *GameplayState::singleton = nullptr;


GameplayState::GameplayState() {
    m_camera = new MetricsCamera(0, 0, 16);
    m_cam_follow_mouse = false;
    m_cam_mouse_offset = {0.f, 0.f};

    //m_terrain = Terrain::construct("res/wowmap.png", {32.f, 22.f});
    m_terrain = nullptr;
    m_object_manager = new ObjectManager;

    m_current_player = -1;
    m_players_per_team = 3;
    m_team_count = 2;
    m_start_regions.push_back({0.f, 0.f, 16.f, 22.f});
    m_start_regions.push_back({16.f, 0.f, 16.f, 22.f});

    m_overlay = new WidgetManager;
    m_hotbar_text = new Label(0, 95, 20, "Hotbar text");
    m_hotbar_text->SetAlignment(WidgetAlignment_MiddleBottom);
    m_hotbar_text->SetOutline(true);
    m_overlay->AddWidget(m_hotbar_text);
    m_overlay->AddWidget(new PlayerIndicator(this));
    m_action_widgets = new WidgetManager;
    m_show_action_widgets = false;

    m_windows = new WindowManager;
    m_spawned_object = nullptr;
    m_preview_spawned_object = false;

    m_tooltip = new Tooltip;

    m_editor = nullptr;
}

GameplayState::~GameplayState() {
    if(m_editor != nullptr) {
        delete m_editor;
        m_editor = nullptr;
    }
    delete m_tooltip;

    delete m_spawned_object;
    m_spawned_object = nullptr;

    delete m_windows;
    delete m_overlay;
    delete m_action_widgets;
    delete m_object_manager;
    delete m_terrain;
    delete m_camera;
}



void GameplayState::InitTerrain(Terrain *terrain) {
    if(terrain == nullptr) return;
    if(m_terrain != nullptr) {
        TRACE("Trying to initialise a new terrain, but there is already one.\n");
        delete m_terrain;
    }

    m_terrain = terrain;
}

void GameplayState::InitSpawnRegion(int team, Rectangle rec) {
    TRACE("team %i {%f %f %f %f}\n", team, rec.x, rec.y, rec.width, rec.height);
    if(team < 0) return;
    while(m_start_regions.size() < team+1) m_start_regions.push_back({0.f, 0.f, 10.f, 10.f});
    m_start_regions[team] = rec;
    m_team_count = (int) m_start_regions.size();
}



void GameplayState::Update(float dt) {
    m_tooltip->ClearElements();

    if(IsKeyPressed(KEY_F1)) m_windows->AddWindow(new CollectibleSpawnWindow(10, 10, this));
    if(IsKeyPressed(KEY_ESCAPE)) Manager()->SetState(new PauseMenu(this), false);
    if(IsKeyDown(KEY_UP)) m_camera->origin_y += (int)100*dt;

    int mouse_x = GetMouseX();
    int mouse_y = GetMouseY();

    m_windows->Update();

    m_overlay->Update();
    if(m_show_action_widgets) m_action_widgets->Update();

    HandleDragCamera((float)mouse_x, (float)mouse_y);

    Vector2 mouse_meters = m_camera->ConvertAbsoluteToMeters(mouse_x, mouse_y);
    UpdateSpawnedObject(mouse_meters);
    //m_terrain->DestroyCircle(mouse_meters, 1.f);

    m_object_manager->Update(dt);

    if(PlacingPlayers()) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !IsMouseUsed()) {
            PlacePlayer(mouse_meters, m_players.size() % m_team_count);
        }
    }

    UpdateHotbarText();
}

void GameplayState::Draw() {
    Metrics::SetGraphicsCam(m_camera);
    Metrics::DrawGrid();

    //Spawn regions
    if(PlacingPlayers()) {
        for(int i = 0; i < m_start_regions.size(); ++i) {
            Metrics::DrawRectangle(m_start_regions[i], {s_team_colors[i].r, s_team_colors[i].g, s_team_colors[i].b, 100});
        }
    }

    if(m_terrain != nullptr) m_terrain->Draw();
    m_object_manager->Draw();

    //Green indicator
    Player *p = GetCurrentPlayer();
    if(p != nullptr) {
        Vector2 pos = p->GetPosition();
        pos.y -= 1.2f;
        Metrics::DrawSpriteRot(Res::green_marker_sprite, pos, {.7f, .7f}, 0.f, WHITE);
    }

    if(m_preview_spawned_object && m_spawned_object != nullptr) m_spawned_object->Draw();

    if(m_show_action_widgets) m_action_widgets->Draw();
    m_overlay->Draw();

    m_windows->Draw();

    m_tooltip->Draw(GetMouseX()+10, GetMouseY()+10);
}


Player *GameplayState::GetCurrentPlayer() {
    if(m_current_player < 0 || m_current_player >= m_players.size()) return nullptr;
    return m_players[m_current_player];
}

int GameplayState::GetPlayerCount() {
    return (int) m_players.size();
}
Player *GameplayState::GetPlayer(int index) {
    if(index < 0 || index >= m_players.size()) return nullptr;
    return m_players[index];
}

void GameplayState::PlacePlayer(Vector2 pos, int team, bool check_start_pos) {
    Player *p = new Player(pos, team, this, 10);

    if(m_terrain->CheckCollisionRec(p->GetRectangle(), true)) {
        delete p;
        return;
    }

    if(check_start_pos && !CheckCollisionRecs(p->GetRectangle(), m_start_regions[team])) {
        delete p;
        return;
    }

    m_players.push_back(p);
    m_object_manager->AddObject(p);
    TRACE("Player spawned\n");

    if(m_players.size() >= m_players_per_team*m_team_count) {
        NextPlayerTurn();
    }
}

void GameplayState::NextPlayerTurn() {
    ++m_current_player;
    m_current_player %= (int) m_players.size();

    // We don't want to give the turn to a dead player
    while(m_players[m_current_player] == nullptr) {         //TODO : use "do {} while();" and remove previous lines ?
        ++m_current_player;
        m_current_player %= (int) m_players.size();
    }

    TRACE("player %i's turn\n", m_current_player);
    ShowActionWidgets();

    //TODO : spawn random items here
    if(rand()%10000 < 2000) {
        int item_count = rand()%5 + 1;      //[1; 5]
        for(int i = 0; i < item_count; ++i) SpawnRandomItem();
    }
}

/**
 * Kill a player.
 * Player should be removed from the game only using this method, and nothing else.
 * @param p the player we want to kill
 */
void GameplayState::KillPlayer(Player *p) {
    int p_index = -1;
    for(int i = 0; i < m_players.size(); ++i) {
        if(m_players[i] == p) {
            p_index = i;
            break;
        }
    }

    if(p_index == -1) return;

    TRACE("killing player %i\n", p_index);
    m_players[p_index] = nullptr;
    m_object_manager->DestroyObject(p);

    CheckForVictory();

    if(p_index == m_current_player) NextPlayerTurn();   // This is in the case the current player died :(
}

void GameplayState::ShowActionWidgets() {
    m_action_widgets->Clear();
    m_show_action_widgets = true;
    int marge = 10;

    Player *current_player = GetCurrentPlayer();
    if(current_player == nullptr) return;

    auto widgets = current_player->GetActionWidgets();
    if(widgets.empty()) return;

    int widgets_width = 0;      // Get width of all widget combined

    for(Widget *w : widgets) widgets_width += w->Width();
    widgets_width += marge * (int)(widgets.size()-1);        // Add marge

    // Display all the widgets
    int x_pos = -widgets_width/2 + widgets[0]->Width()/2;
    for(int i = 0; i < widgets.size(); ++i) {
        widgets[i]->SetAlignment(WidgetAlignment_MiddleBottom);
        widgets[i]->SetPosition(x_pos, marge);
        m_action_widgets->AddWidget(widgets[i]);
        if(i != widgets.size()-1) x_pos += (widgets[i]->Width() + widgets[i+1]->Width())/2 + marge;
    }
}

void GameplayState::HideActionWidgets() {
    m_show_action_widgets = false;
    m_action_widgets->Clear();
}

void GameplayState::ForceUpdateWidgets() {
    m_overlay->Update();
    if(m_show_action_widgets) m_action_widgets->Update();
}


void GameplayState::SpawnRandomItem() {
    float x = ((float)(rand()%100000) / 100000.f) * m_terrain->Width();
    float y = ((float)(rand()%100000) / 100000.f) * m_terrain->Height();
    size_t item_index = rand()%s_item_count;

    TRACE("item index %zu at %f %f\n", item_index, x, y);
    Collectible *item = s_item_constructors[item_index]({x, y});

    if(m_terrain->CheckCollisionRec(item->GetRectangle(), true)) {
        //Make the item go up util we are no longer clipping with the terrain
        int safe_count = 0;
        do {
            item->m_position.y -= .05f;
            ++safe_count;
        } while(m_terrain->CheckCollisionRec(item->GetRectangle(), true) && safe_count < 250);

        if(safe_count == 250) {
            TRACE("item clipped too much with terrain, cancelling\n");
            delete item;
            return;
        }
    }
    else {
        //Make the item go down util we are clipping with the terrain
        int safe_count = 0;
        do {
            item->m_position.y += .05f;
            ++safe_count;
        } while(!m_terrain->CheckCollisionRec(item->GetRectangle(), true) && safe_count < 250);

        if(safe_count == 250) {
            TRACE("item clipped too much with terrain, cancelling\n");
            delete item;
            return;
        }
    }

    if(!m_object_manager->GetCollisions(item, 0).empty()) {
        TRACE("item spawned on other object, cancelling\n");
        delete item;
        return;
    }

    item->m_position.y -= .2f;
    m_object_manager->AddObject(item);
}

void GameplayState::SetSpawnedObject(EntityObject *spawned_object) {
    delete m_spawned_object;
    m_spawned_object = spawned_object;
}


////////////////////////////////
//// PRIVATE

void GameplayState::HandleDragCamera(float mouse_x, float mouse_y) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && !IsMouseUsed()) {
        m_cam_follow_mouse = true;
        m_cam_mouse_offset = {m_camera->origin_x - mouse_x, m_camera->origin_y - mouse_y};
    }
    if(IsMouseButtonUp(MOUSE_BUTTON_RIGHT)) {
        m_cam_follow_mouse = false;
    }

    if(m_cam_follow_mouse) {
        m_camera->origin_x = m_cam_mouse_offset.x + mouse_x;
        m_camera->origin_y = m_cam_mouse_offset.y + mouse_y;
    }
    else {
        if(IsMouseUsed()) return;

        // Handle zoom
        float mouse_wheel = GetMouseWheelMove();
        if(mouse_wheel > 0) m_camera->SetPixelsPerMeter((float)m_camera->PixelsPerMeter() * (1.f+mouse_wheel));
        else if(mouse_wheel < 0) m_camera->SetPixelsPerMeter((float)m_camera->PixelsPerMeter() / (1.f-mouse_wheel));
    }
}

void GameplayState::UpdateHotbarText() {
    if(PlacingPlayers()) {
        int team_index = (int)m_players.size()%m_team_count;     //Find which team is currently placing
        m_hotbar_text->SetLabel(TextFormat("Place %s player", s_team_names[team_index]));
        m_hotbar_text->SetColor(s_team_colors[team_index]);
        return;
    }

    Player *p = GetCurrentPlayer();
    if(p == nullptr) {
        m_hotbar_text->SetLabel("Error : player is null :^(");
        return;
    }

    m_hotbar_text->SetLabel(TextFormat("Energy : %i", p->GetEnergy()));
    m_hotbar_text->SetColor(s_team_colors[p->GetTeam()]);
}


void GameplayState::CheckForVictory() {
    std::vector<int> remaining_players;
    for(int i = 0; i < m_team_count; ++i) remaining_players.push_back(0);
    for(Player *p : m_players) {
        if(p == nullptr) continue;
        ++remaining_players[p->GetTeam()];
    }

    int winning_team = -1;
    for(int i = 0; i < m_team_count; ++i) {
        TRACE("Checking if there are player remaining in team %s\n", s_team_names[i]);
        if(remaining_players[i] != 0) {
            TRACE("YES\n");
            if(winning_team != -1) return;
            winning_team = i;
        }
    }
    if(winning_team == -1) return;

    TRACE("team %s won\n", s_team_names[winning_team]);
    if(m_editor != nullptr) {
        Manager()->SetState(m_editor, true);
        m_editor = nullptr;     //Do this so the editor don't get deallocated
    }
    else {
        Manager()->SetState(new PostGameMenu(winning_team), true);
    }
}


void GameplayState::UpdateSpawnedObject(Vector2 mouse_pos_meter) {
    if(m_spawned_object == nullptr || IsMouseUsed()) {
        m_preview_spawned_object = false;
        return;
    }

    m_preview_spawned_object = true;
    m_spawned_object->m_position = mouse_pos_meter;

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        m_object_manager->AddObject(m_spawned_object);
        m_spawned_object = nullptr;
        m_preview_spawned_object = false;
    }

    UseMouse();
}