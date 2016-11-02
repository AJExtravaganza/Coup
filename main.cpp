#include <iostream>
#include "Classes.hpp"
#include <cstdlib>

using namespace std;

void initialDeal (Player playerGroup[], int playerCount);
int playersAlive(Player playerGroup[]);
void processTurn(Player playerGroup[], int players, Player &activePlayer);
void processActive(Player playerGroup[], Player &activePlayer, StackAction &activeAction);
void processBlock(Player playerGroup[], Player blockingPlayer, StackAction &blockableAction);
void processChallenges( Player playerGroup[], StackAction &challengedAction);
void queryForeignAidBlocker(Player playerGroup[], int players, StackAction &activeAction);

int main()
{
   srand(1338); //random seed

    int players = 2; //stub.  query in future

    Player playerPosition [MAX_ALLOWABLE_PLAYERS]{};
    initialDeal(playerPosition, players);

    Player * activePlayer = &playerPosition[(rand() % players)];

    while (players > 1)
    {
        if (activePlayer->isAlive())
        {
            processTurn(playerPosition, players, *activePlayer);
        }

        activePlayer = &playerPosition[((activePlayer->getID() + 1) % players)]; // Permute to next player
    }

    return 0;
}

void initialDeal(Player playerGroup[], int playerCount)
{

    for (int i = 0; i < playerCount; i++)
    {
        playerGroup[i] = Player(i);
        playerGroup[i].giveNewCard(static_cast<Role>(rand() % 5 + 1));
        playerGroup[i].giveNewCard(static_cast<Role>(rand() % 5 + 1));

    }
}

int playersAlive(Player playerGroup[])
{
    int livePlayers = 0;
    for (int i = 0; i < MAX_ALLOWABLE_PLAYERS; i++)
    {
        if (playerGroup[i].isAlive())
        {
            livePlayers++;
        }

    }

    return livePlayers;

}

void processTurn(Player playerGroup[], int currentPlayers, Player &activePlayer)
{
    //system("cls");

    StackAction activeAction;
    processActive(playerGroup, activePlayer, activeAction);

    if (activeAction.getStatus() == VALID && activeAction.isBlockable())
    {

        if (activeAction.getDeclaredAction() == FOREIGN_AID)
        {
            queryForeignAidBlocker(playerGroup, currentPlayers, activeAction);
        }
        else
        {
            processBlock(playerGroup, activeAction.getTarget(), activeAction);
        }
    }

    if (activeAction.getStatus() == VALID)
    {
        activeAction.perform();
    }

}

void processActive(Player playerGroup[], Player &activePlayer, StackAction &activeAction)
{
    int menuSelection = -1;

    std::cout << "Player " << activePlayer.getID() << " is the active player\n\n"
              << "You have the following unexposed card(s) in your hand: \n" << activePlayer.listHand() << ".\n"
              << "You have " << activePlayer.hasIsk() << " ISK.\n\n"
              << "Which action would you like to take? [1-7]\n\n"
              << "[1] Take income\n"
              << "[2] Take foreign aid\n"
              << "[3] Stage a coup\n"
              << "[4] Tax\n"
              << "[5] Assassinate\n"
              << "[6] Exchange\n"
              << "[7] Steal\n\n"
              << "Your choice: ";

    bool validEntry = false;
    do
    {
        menuSelection = (activePlayer.hasIsk() > MAX_ALLOWABLE_ISK) ? 3 : getSelection(1, 7);
        StackAction newAction(playerGroup, activePlayer, static_cast<Action>(menuSelection));
        if (newAction.getDeclaredAction() != NullAction)
        {
            activeAction = newAction;
            validEntry = true;
        }
        else
        {
            std::cout << "Insufficient Funds.  Make another selection: ";
        }
    }while (!validEntry);

    if(activeAction.isChallengable())
    {
        processChallenges(playerGroup, activeAction);
    }
}
void processBlock(Player playerGroup[], Player blockingPlayer, StackAction &blockableAction)
{
    Role blocker = blockingPlayer.willBlockWith(blockableAction.getDeclaredAction());
    StackAction blockAction;
    switch (blocker)
    {
    case DUKE:
        {StackAction test(playerGroup, blockingPlayer, BLOCK_FOREIGN_AID); //BOOKMARK
        blockAction = test;
        break;}
    case CONTESSA:
        {StackAction test(playerGroup, blockingPlayer, BLOCK_ASSASSINATE); //BOOKMARK
        blockAction = test;
        break;}
    case CAPTAIN:
        {StackAction test(playerGroup, blockingPlayer, BLOCK_STEAL_CAPTAIN); //BOOKMARK
        blockAction = test;
        break;}
    case AMBASSADOR:
        {StackAction test(playerGroup, blockingPlayer, BLOCK_STEAL_AMBASSADOR); //BOOKMARK
        blockAction = test;
        break;}
    default:
        break;
    }

    if (blocker)
    {
        if(blockAction.isChallengable()) //should always return true.
        {
            processChallenges(playerGroup, blockAction);
        }

        if (blockAction.getStatus() != CHALLENGED)
        {
            blockableAction.setStatus(BLOCKED);
        }
    }




}

void processChallenges( Player playerGroup[], StackAction &challengedAction)
{
    Player * challenger = playerGroup;

    for (int i = 0; i < MAX_ALLOWABLE_PLAYERS; i++)
    {
        challenger = playerGroup + i;

        if (challenger->getID() == challengedAction.getCaster().getID()) // Cannot challenge your own action WILL OVERFLOW ON MATCH FOR SIXTH PLAYER
        {
            //i++;
            challenger++;
        }
        else if (challenger->willChallenge())
        {
            if (challengedAction.getCaster().hasCardFor(challengedAction.getDeclaredAction()))
            {
                challenger->sacrifice();
            }
            else
            {
                    challengedAction.getCaster().sacrifice();
                    challengedAction.setStatus(CHALLENGED);
            }

        }

    }

}

void queryForeignAidBlocker(Player playerGroup[], int currentPlayers, StackAction &activeAction)
{
    bool blockerFound = false;
    for (int i = 0; (i < currentPlayers) && blockerFound == false; i++)
    {
        if((i != activeAction.getCaster().getID()) && (activeAction.getStatus() == VALID)) //BOOKMARK
        {
            processBlock(playerGroup, playerGroup[i], activeAction);
        }
    }
}
