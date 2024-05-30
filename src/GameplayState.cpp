#include "GameplayState.h"

#include <random>
#include <raylib.h>
#include <stdio.h>

#include "engine/Globals.h"
#include "engine/metrics/Graphics.h"
#include "engine/metrics/MetricsCamera.h"
#include "engine/object/ObjectManager.h"
#include "engine/util/Trace.h"
#include "engine/widgets/Label.h"
#include "engine/widgets/WidgetManager.h"
#include "engine/widgets/Widget.h"
#include "objects/actors/Player.h"
#include "objects/collectibles/Portalgun.h"
#include "objects/collectibles/Trowel.h"
#include "Teams.h"
#include "Terrain.h"


//GameplayState *GameplayState::singleton = nullptr;


GameplayState::GameplayState() {
    m_camera = new MetricsCamera(0, 0, 16);
    m_cam_follow_mouse = false;
    m_cam_mouse_offset = {0.f, 0.f};

    m_terrain = Terrain::construct("res/wowmap.png", {32.f, 22.f});
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
    m_action_widgets = new WidgetManager;
    m_show_action_widgets = false;
}

GameplayState::~GameplayState() {
    delete m_overlay;
    delete m_action_widgets;
    delete m_object_manager;
    delete m_terrain;
    delete m_camera;
}

void GameplayState::Update(float dt) {
    if(IsKeyDown(KEY_UP)) m_camera->origin_y += (int)100*dt;

    int mouse_x = GetMouseX();
    int mouse_y = GetMouseY();

    //TODO : update widgets here
    m_overlay->Update();
    if(m_show_action_widgets) m_action_widgets->Update();

    HandleDragCamera((float)mouse_x, (float)mouse_y);

    Vector2 mouse_meters = m_camera->ConvertAbsoluteToMeters(mouse_x, mouse_y);
    if(IsKeyPressed(KEY_T)) m_object_manager->AddObject(new Trowel(mouse_meters));
    if(IsKeyPressed(KEY_P)) m_object_manager->AddObject(new Portalgun(mouse_meters));
    if(IsKeyPressed(KEY_R)) SpawnRandomItem();
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

    if(PlacingPlayers()) {
        for(int i = 0; i < m_start_regions.size(); ++i) {
            Metrics::DrawRectangle(m_start_regions[i], {s_team_colors[i].r, s_team_colors[i].g, s_team_colors[i].b, 100});
        }
    }
    m_terrain->Draw();
    m_object_manager->Draw();

    if(m_show_action_widgets) m_action_widgets->Draw();
    m_overlay->Draw();
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

    //TODO : check victory here

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


void GameplayState::SpawnRandomItem() {
    static const CollectibleConstructor item_constructors[] = {
            Trowel::construct,
            Portalgun::construct
    };
    static const size_t item_count = sizeof(item_constructors) / sizeof(CollectibleConstructor);

    float x = ((float)(rand()%100000) / 100000.f) * m_terrain->Width();
    float y = ((float)(rand()%100000) / 100000.f) * m_terrain->Height();
    size_t item_index = rand()%item_count;

    TRACE("item index %zu at %f %f\n", item_index, x, y);
    Collectible *item = item_constructors[item_index]({x, y});

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


Player *GameplayState::GetCurrentPlayer() {
    if(m_current_player < 0 || m_current_player >= m_players.size()) return nullptr;
    return m_players[m_current_player];
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
        if(mouse_wheel > 0) m_camera->SetPixelsPerMeter(m_camera->PixelsPerMeter() * 2);
        else if(mouse_wheel < 0) m_camera->SetPixelsPerMeter(m_camera->PixelsPerMeter() / 2);
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
