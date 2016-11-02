#include "classes.h"
#include "stdlib.h"
#include <iostream>
#include "gameParameters.h"
#include "input_sanitisation.h"


bool Card::hasRole(Role queriedRole)
{
    return role == queriedRole ? true : false;
}

std::string Card::getName()
{
    return name;
}

bool Card::canPerform(Action queriedAction)
{
    return (mainAction == queriedAction || blockAction == queriedAction) ? true : false;
}

/*bool Card::canBlock (Action queriedAction) ////DEPRECATED
{
    return blockAction == queriedAction ? true : false;
}*/

bool Card::isExposed()
{
    return exposed ? true : false;
}

void Card::expose()
{
    exposed = true;
}

Card::Card():role(NullRole), name(""), mainAction(NullAction), blockAction(NullAction), exposed(true) {} //default constructor

Card::Card(Role card):role(card), name(""), mainAction(NullAction), blockAction(NullAction), exposed(false)
{
    switch (card)
    {
        case DUKE:
            name = "the Duke";
            mainAction = TAX;
            blockAction = BLOCK_FOREIGN_AID;
            break;
        case ASSASSIN:
            name = "the Assassin";
            mainAction = ASSASSINATE;
            blockAction = NullAction;
            break;
        case AMBASSADOR:
            name = "the Ambassador";
            mainAction = EXCHANGE;
            blockAction = BLOCK_STEAL_AMBASSADOR;
            break;
        case CAPTAIN:
            name = "the Captain";
            mainAction = STEAL;
            blockAction = BLOCK_STEAL_CAPTAIN;
            break;
        case CONTESSA:
            name = "the Contessa";
            mainAction = NullAction;
            blockAction = BLOCK_ASSASSINATE;
            break;
        default:
            name = "the NullCard";
            mainAction = NullAction;
            blockAction = NullAction;
    }

}




int Player::getID() const
{
    return playerID;
}

void Player::giveIsk(int quantity)
{
    isk += quantity;
}

void Player::takeIsk(int quantity)
{
    isk -= quantity;
}

int Player::hasIsk() const
{
    return isk;
}

void Player::giveNewCard(Role card)
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

bool Player::hasCardFor(Action claimedAction)
{
    return (cardInHand[0].canPerform(claimedAction) || cardInHand[1].canPerform(claimedAction));
}

std::string Player::listHand()
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

Role Player::willBlockWith(Action declaredAction) //FEATURE: RETURN TYPE ROLE, RETURNS BLOCKER, OR NULLROLE, WHICH IS TREATED AS FALSE
{
    int menuSelection = -1;
    Role claimedRole = NullRole;


    std::cout << "Player " << playerID << ", will you block?\n"
              << "[1] Let it slide\n";

    switch (declaredAction)
    {
    case FOREIGN_AID:
        std::cout << "[2] Block! [Claim the Duke]\n\n"
                  << "Your choice: ";
        claimedRole = (getSelection(1,2) == 2) ? DUKE : NullRole;

        break;
    case ASSASSINATE:
        std::cout << "[2] Block! [Claim the Contessa]\n\n"
                  << "Your choice: ";
        claimedRole = (getSelection(1,2) == 2) ? CONTESSA : NullRole;
        break;
    case STEAL:
        std::cout << "[2] Block! [Claim The Captain]\n"
                  << "[3] Block! [Claim the Ambassador]\n\n";
        menuSelection = getSelection(1,3);
        if (menuSelection == 2){claimedRole = CAPTAIN;}
        else if (menuSelection == 3){claimedRole = AMBASSADOR;}
        else {claimedRole=NullRole;}
        break;
    default:
        std::cout << "Invalid case in Player::willBlockWith() - Report to developer.";
        break;
    }

    return claimedRole;
}

bool Player::willChallenge()
{
    int menuSelection = -1;

    if (isAlive())
    {

        std::cout << "Player " << playerID << ", will you challenge?\n"
                  << "[1] Let it slide.\n"
                  << "[2] Issue Challenge\n\n"
                  << "Your choice: ";

        menuSelection = getSelection(1, 2);
    }

    return (menuSelection == 2);
}

void Player::sacrifice()
{
    std::cout << "Stub: Player " << getID() << " sacrifices a card." << std::endl;
}
void Player::exchange()
{
    std::cout << "Stub: Player  " << getID() << " exchanges card(s)" << std::endl;
}

void Player::killPlayer()
{
    stillAlive = false;
}

bool Player::isAlive()
{
    return stillAlive;
}

Player::Player(): playerID(-1), isk(0), stillAlive(false){}

Player::Player(int playerNumber): playerID(playerNumber), isk(2), stillAlive(true){}



Action StackAction::getDeclaredAction()
{
    return declaredAction;
}
ActionStatus StackAction::getStatus()
{
    return status;
}

bool StackAction::isBlockable()
{
    return blockable;
}

bool StackAction::isChallengable()
{
    return challengable;
}

bool StackAction::isTargettable()
{
    return targettable;
}

void StackAction::perform() //unfuck this later
{
    switch(declaredAction)
    {
        case INCOME:
            caster->giveIsk(1);
            std::cout << "Player "<< caster->getID() << " receives 1 ISK.\n\n";
            system("pause");
            break;
        case FOREIGN_AID:
            caster->giveIsk(2);
            std::cout << "Player "<< caster->getID() << " receives 2 ISK.\n\n";
            system("pause");
            break;
        case COUP:
            std::cout << "Player "<< caster->getID() << " stages a Coup against Player " << target->getID() << ".\n\n";
            system("pause");
            target->sacrifice();
            system("pause");
            break;
        case TAX:
            caster->giveIsk(3);
            std::cout << "Player "<< caster->getID() << " claims the Duke, and receives 3 ISK.\n\n";
            system("pause");
            break;
        case ASSASSINATE:
            std::cout << "Player "<< caster->getID() << " claims the Assassin, and stages an assassination against Player " << target->getID() << ".\n\n";
            system("pause");
            target->sacrifice();
            break;
        case EXCHANGE:
            std::cout << "Player "<< caster->getID() << " claims the Ambassador, and exchanges card(s).\n\n";
            system("pause");
            caster->exchange();
            break;
        case STEAL:
            std::cout << "Player "<< caster->getID() << " claims the Captain, and steals from Player " << target->getID() << ".\n\n";
            system("pause");
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
        case NullAction:
            std::cout << "ERROR: You have attempted to perform a NullAction.  Inform developer." << std::endl;
            break;
        default:
            std::cout << "Cunt's fucked.  Inform developer." << std::endl;
            break;
    }
}

void StackAction::setStatus(ActionStatus newStatus)
{
    if (newStatus == CHALLENGED && getDeclaredAction() == ASSASSINATE)
    {
        caster->giveIsk(ASSASSINATE_COST);
    }
    status = newStatus;
}

void StackAction::queryTarget(Player playerGroup[])
{
    int targetID = -1;
    std::cout << "Targetting which player? ";
    //add check for valid player selection
    targetID = getSelection(0, MAX_ALLOWABLE_PLAYERS - 1);
    while (targetID == caster->getID())
    {
        targetID = getSelection(0, MAX_ALLOWABLE_PLAYERS - 1);
    }
    target = playerGroup + targetID;
}

void StackAction::setTarget(Player targetPlayer)
{
    target = &targetPlayer;
}

Player StackAction::getCaster()
{
    return *caster;
}

Player StackAction::getTarget()
{
    return (targettable ? *target : *caster); //Because returning *target for ForeignAid causes SegFault
}

StackAction::~StackAction()
{

}

StackAction::StackAction(): declaredAction(NullAction), status(NullStatus), blockable(false), challengable(false), targettable(false), caster(nullptr), target(nullptr)
{

}

StackAction::StackAction(Player playerGroup[], Player & castingPlayer, Action action): declaredAction(action), status(VALID), blockable(false), challengable(false), targettable(false), caster(&castingPlayer), target(nullptr)
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
            if (caster->hasIsk() >= COUP_COST)
            {
                blockable = false;
                challengable = false;
                targettable = true;
            }
            else
            {
                declaredAction = NullAction;
            }
            break;
        case TAX:
            blockable = false;
            challengable = true;
            targettable = false;
            break;
        case ASSASSINATE:
            if (caster->hasIsk() >= ASSASSINATE_COST)
            {
                blockable = true;
                challengable = true;
                targettable = true;
                castingPlayer.takeIsk(ASSASSINATE_COST);
            }
            else
            {
                declaredAction = NullAction;
            }
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
        case BLOCK_STEAL_CAPTAIN:
            blockable = false;
            challengable = true;
            targettable = false;
            break;
        case BLOCK_STEAL_AMBASSADOR:
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
        queryTarget(playerGroup);
    }

}

StackAction &StackAction::operator=(const StackAction other)
{
    declaredAction = other.declaredAction;
    status = other.status;
    blockable = other.blockable;
    challengable = other.challengable;
    targettable = other.targettable;
    caster = other.caster;
    target = other.target;
    return *this;
}

StackAction::StackAction(StackAction &other):declaredAction(NullAction),status(NullStatus),blockable(false),challengable(false),targettable(false),caster(nullptr),target(nullptr)
{
    declaredAction = other.declaredAction;
    status = other.status;
    blockable = other.blockable;
    challengable = other.challengable;
    targettable = other.targettable;
    caster = other.caster;
    target = other.target;
}
