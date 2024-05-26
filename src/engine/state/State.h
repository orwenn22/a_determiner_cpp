#ifndef UI_FRAMEWORK_STATE_H
#define UI_FRAMEWORK_STATE_H

class StateManager;

class State {
public:
    State();
    virtual ~State();

    virtual void Update(float dt) = 0;
    virtual void Draw() = 0;

    virtual inline const char *GetIdentifier() { return "NO IDENTIFIER"; }

private:
    friend StateManager;
    StateManager *m_manager;
};


#endif //UI_FRAMEWORK_STATE_H
