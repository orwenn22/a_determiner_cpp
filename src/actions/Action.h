#ifndef A_DETERMINER_ACTION_H
#define A_DETERMINER_ACTION_H

#include <raylib.h>

class Player;

class Action {
public:
    Action();
    virtual ~Action();

    /**
     * Called when the button corresponding to the action is clicked.
     * If we need to do something else than change the players' current action when clicking (for example
     *  instantly consuming an item ?), then this should be overwritten by the subclass.
     * @param player the player performing the action
     * @param action_index the index of the action in the player's actions
     */
    virtual void OnClick(Player *player, int action_index);

    /**
     * Called when the turn of the current player is skipped by clicking on the "skip" button
     * Can return True if this action require custom handling for skipping turn
     * @param player player owing the action
     * @return true if we don't want to pass the action to the next player yet and have custom skipping logic
     */
    virtual bool OnSkip(Player *player);

    /**
     * Called every frame on the player's update if the action is selected
     * @param player the player performing the action
     * @param dt deltatime in seconds
     */
    virtual void OnUpdate(Player *player, float dt);

    /**
     * Called every frame on the player's draw if the action is selected
     * @param player the player performing the action
     */
    virtual void OnDraw(Player *player);

    const char *GetName() { return m_action_name; }
    inline bool IsItem() { return m_is_item; }
    inline int Cost() { return m_action_cost; }
    Texture *GetIcon() { return m_icon; }

protected:
    const char *m_action_name;
    int m_action_cost;
    bool m_is_item;
    Texture *m_icon;
};


#endif //A_DETERMINER_ACTION_H
