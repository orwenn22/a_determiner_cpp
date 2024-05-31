#include "StateManager.h"

#include "State.h"

StateManager::StateManager(State *state) {
    m_state = state;
    if(m_state == nullptr) return;
    //TODO : check if the state already have a manager ?
    m_state->m_manager = this;
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

void StateManager::SetState(State *state, bool dealloc_previous) {
    if(dealloc_previous && m_state != nullptr) {
        m_trashcan.push_back(m_state);
    }

    m_state = state;
    if(m_state == nullptr) return;
    //TODO : check if the state already have a manager ?
    m_state->m_manager = this;
}


///////////////////////////////////
//// PRIVATE

void StateManager::ClearTrashcan() {
    if(m_trashcan.empty()) return;

    for(State *s : m_trashcan) {
        delete s;
    }
    m_trashcan.clear();
}
