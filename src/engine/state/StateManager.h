#ifndef UI_FRAMEWORK_STATEMANAGER_H
#define UI_FRAMEWORK_STATEMANAGER_H

#include <vector>

class State;

class StateManager {
public:
    StateManager(State *state);
    ~StateManager();

    void Update(float dt);
    void Draw();

    void SetState(bool dealloc_previous = true);
    inline State *GetState() { return m_state; }


private:
    void ClearTrashcan();

    State *m_state;
    std::vector<State *> m_trashcan;
};


#endif //UI_FRAMEWORK_STATEMANAGER_H
