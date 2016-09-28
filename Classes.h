
#include <iostream>
#include <string>
#include "cardAttributes.h"
#include "gameParameters.h"
#include "usefulshit.h"





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
    bool hasRole(Role queriedRole)
    {return role == queriedRole ? true : false;}

    std::string getName()
    {
        return name;
    }

    bool canPerform(Action queriedAction)
    {return mainAction == queriedAction ? true : false;}

    bool canBlock (Action queriedAction)
    {return blockAction == queriedAction ? true : false;}

    bool isExposed()
    {
        return exposed ? true : false;
    }

    void expose()
    {
        exposed = true;
    }

    Card():role(NullRole), name(""), mainAction(NullAction), blockAction(NullAction), exposed(true) {} //default constructor

    Card(Role card):role(card), name(""), mainAction(NullAction), blockAction(NullAction), exposed(false)
    {
        switch (card)
        {
            case DUKE:
                name = "the Duke";
                mainAction = TAX;
                blockAction = FOREIGN_AID;
                break;
            case ASSASSIN:
                name = "the Assassin";
                mainAction = ASSASSINATE;
                blockAction = NullAction;
                break;
            case AMBASSADOR:
                name = "the Ambassador";
                mainAction = EXCHANGE;
                blockAction = STEAL;
                break;
            case CAPTAIN:
                name = "the Captain";
                mainAction = STEAL;
                blockAction = STEAL;
                break;
            case CONTESSA:
                name = "the Contessa";
                mainAction = NullAction;
                blockAction = ASSASSINATE;
                break;
            default:
                name = "the NullCard";
                mainAction = NullAction;
                blockAction = NullAction;
        }

    }

};


class Player
{
private:
    int playerID;
    int isk;
    bool stillAlive;
    Card cardInHand [2];
public:
    int getID() const
    {
        return this->playerID;
    }

    void giveIsk(int quantity)
    {
        isk += quantity;
    }

    void takeIsk(int quantity)
    {
        isk -= quantity;
    }

    int hasIsk() const
    {
        return isk;
    }

    void giveNewCard(Role card)
    {
        if (cardInHand[0].hasRole(NullRole))
        {
            cardInHand[0] = Card(card);
        }
        else if (cardInHand[1].hasRole(NullRole))
        {
            cardInHand[1] = Card(card);
        }
    }

    bool hasCardFor(Action claimedAction)
    {
        return (cardInHand[0].canPerform(claimedAction) || cardInHand[1].canPerform(claimedAction));
    }

    std::string listHand()
    {
        std::string handString = "";

        if (!cardInHand[0].isExposed())
            handString += cardInHand[0].getName();

        if (!cardInHand[0].isExposed() && !cardInHand[1].isExposed())
            handString +=  " and ";

        if (!cardInHand[1].isExposed())
            handString +=  cardInHand[1].getName();

        return handString;
    }

    bool willBlock()
    {
        int menuSelection = -1;


        std::cout << "Player " << playerID << ", will you block?\n"
                  << "[1] Let it slide"
                  << "[2] Block! [Claim $theRoleStub]\n\n"
                  << "Your choice: ";

        getSelection(menuSelection, 2);

        return (menuSelection == 2);
    }

    bool willChallenge()
    {
        int menuSelection = -1;

        if (isAlive())
        {

            std::cout << "Player " << playerID << ", will you challenge?\n"
                      << "[1] Let it slide.\n"
                      << "[2] Issue Challenge\n\n"
                      << "Your choice: ";

            getSelection(menuSelection, 2);
        }

        return (menuSelection == 2);
    }

    void sacrifice()
    {
        std::cout << "Stub: Player sacrifices a card." << std::endl;
    }
    void exchange()
    {
        std::cout << "Stub: Player exchanges card(s)" << std::endl;
    }

    void killPlayer()
    {
        stillAlive = false;
    }

    bool isAlive()
    {
        return stillAlive;
    }

    Player(): playerID(-1), isk(0), stillAlive(false){}

    Player(int playerNumber): playerID(playerNumber), isk(2), stillAlive(true){}
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
    Action getDeclaredAction()
    {
        return declaredAction;
    }
    ActionStatus getStatus()
    {
        return status;
    }

    bool isBlockable()
    {
        return blockable;
    }

    Action getBlockFor()
    {
        Action blockAction = NullAction;

        switch (declaredAction)
        {
        case FOREIGN_AID:
            blockAction = BLOCK_FOREIGN_AID;
            break;
        case ASSASSINATE:
            blockAction = BLOCK_ASSASSINATE;
            break;
        case STEAL:
            blockAction = BLOCK_STEAL;
            break;
        default:
            break;
        }

        return blockAction;
    }

    bool isChallengable()
    {
        return challengable;
    }

    bool isTargettable()
    {
        return targettable;
    }

    void perform() //unfuck this later
    {
        switch(declaredAction)
        {
            case INCOME:
                caster->giveIsk(1);
                break;
            case FOREIGN_AID:
                caster->giveIsk(2);
                break;
            case COUP:
                target->sacrifice();
                break;
            case TAX:
                caster->giveIsk(3);
                break;
            case ASSASSINATE:
                caster->takeIsk(3);
                target->sacrifice();
                break;
            case EXCHANGE:
                caster->exchange();
                break;
            case STEAL:
                if (target->hasIsk() > 1)
                {
                    caster->giveIsk(2);
                    target->takeIsk(2);
                }
                else
                {
                    caster->giveIsk(1);
                    target->takeIsk(1);
                }
                break;
            default:
                break; // Throw error
        }
    }

    void setStatus(ActionStatus newStatus)
    {
        status = newStatus;
    }

    void setTarget(Player playerGroup[])
    {
        int targetID = -1;
        std::cout << "Targetting which player? ";
        //add check for valid player selection
        getSelection(targetID, MAX_ALLOWABLE_PLAYERS);
        target = playerGroup + targetID;
    }

    Player getTarget()
    {
        return *target;
    }

    StackAction(): declaredAction(NullAction), status(NullStatus), blockable(false), challengable(false), targettable(false), caster(nullptr), target(nullptr){}

    StackAction(Player playerGroup[], Player & castingPlayer, Action action): declaredAction(action), status(VALID), blockable(false), challengable(false), targettable(false), caster(&castingPlayer), target(nullptr)
    {
        switch(declaredAction)
        {
            case INCOME:
                blockable = false;
                challengable = false;
                targettable = false;
                break;
            case FOREIGN_AID:
                blockable = true;
                challengable = false;
                targettable = false;
                break;
            case COUP:
                blockable = false;
                challengable = false;
                targettable = true;
                break;
            case TAX:
                blockable = false;
                challengable = true;
                targettable = false;
                break;
            case ASSASSINATE:
                blockable = true;
                challengable = true;
                targettable = true;
                break;
            case EXCHANGE:
                blockable = false;
                challengable = true;
                targettable = false;
                break;
            case STEAL:
                blockable = true;
                challengable = true;
                targettable = true;
                break;
            case BLOCK_FOREIGN_AID:
                blockable = false;
                challengable = true;
                targettable = false;
                break;
            case BLOCK_ASSASSINATE:
                blockable = false;
                challengable = true;
                targettable = false;
                break;
            case BLOCK_STEAL:
                blockable = false;
                challengable = true;
                targettable = false;
                break;
            default:
                blockable = false;
                challengable = false;
                targettable = false;
                break; // Throw exception
        }

        if (targettable)
        {
            setTarget(playerGroup);
        }

    }

};


