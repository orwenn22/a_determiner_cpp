#include "StateManager.h"

#include "State.h"

StateManager::StateManager(State *state) {
    m_state = state;
}

StateManager::~StateManager() {
    delete m_state;
    m_state = nullptr;

    ClearTrashcan();
}

void StateManager::Update(float dt) {
    if(m_state != nullptr) m_state->Update(dt);
    ClearTrashcan();
}

void StateManager::Draw() {
    if(m_state != nullptr) m_state->Draw();
}

void StateManager::ClearTrashcan() {
    if(m_trashcan.empty()) return;

    for(State *s : m_trashcan) {
        delete s;
    }
    m_trashcan.clear();
}
