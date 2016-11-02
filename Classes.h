#include <string>
#include "cardAttributes.h"

class Card;
class Player;
class StackAction;

class Card
{
private:
    Role role;
    std::string name;
    Action mainAction;
    Action blockAction;
    bool exposed;
public:
    bool hasRole(Role queriedRole);
    std::string getName();
    bool canPerform(Action queriedAction);
    //bool canBlock (Action queriedAction);
    bool isExposed();
    void expose();

    Card();
    Card(Role card);
};


class Player
{
private:
    int playerID;
    int isk;
    bool stillAlive;
    Card cardInHand [2];
public:
    int getID() const;
    void giveIsk(int quantity);
    void takeIsk(int quantity);
    int hasIsk() const;
    void giveNewCard(Role card);
    bool hasCardFor(Action claimedAction);
    std::string listHand();
    Role willBlockWith(Action declaredAction); //FEATURE: RETURN TYPE ROLE, RETURNS BLOCKER, OR NULLROLE, WHICH IS TREATED AS FALSE
    bool willChallenge();
    void sacrifice();
    void exchange();
    void killPlayer();
    bool isAlive();

    Player();
    Player(int playerNumber);
};

class StackAction
{
private:
    Action declaredAction;
    ActionStatus status;
    bool blockable;
    bool challengable;
    bool targettable;
    Player * caster;
    Player * target;
public:
    Action getDeclaredAction();
    ActionStatus getStatus();
    bool isBlockable();
    Action getBlockFor();
    bool isChallengable();
    bool isTargettable();
    void perform();
    void setStatus(ActionStatus);
    void queryTarget(Player playerGroup[]);
    void setTarget(Player targetPlayer);
    Player getCaster();
    Player getTarget();

    StackAction();
    StackAction(Player playerGroup[], Player & castingPlayer, Action action);
    StackAction &operator=(const StackAction other); //Assignment constructor
    StackAction(StackAction &other); //Copy constructor
    ~StackAction(); //Destructor
};


