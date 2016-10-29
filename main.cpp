#include <iostream>
#include "Classes.hpp"
#include <cstdlib>

using namespace std;

void initialDeal (Player playerGroup[], int playerCount);
int playersAlive(Player playerGroup[]);
void processTurn(Player playerGroup[], int players, Player &activePlayer);
void processActive(Player playerGroup[], Player &activePlayer, StackAction &activeAction);
void processBlock(Player playerGroup[], Player blockingPlayer, StackAction &blockableAction);
void processChallenges( Player playerGroup[], Player &challengedPlayer, StackAction &challengedAction);
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

        processBlock(playerGroup, activeAction.getTarget(), activeAction);
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
        processChallenges(playerGroup, activePlayer, activeAction);
    }
}
void processBlock(Player playerGroup[], Player blockingPlayer, StackAction &blockableAction)
{
    if (blockingPlayer.willBlock())
    {
        StackAction blockAction(playerGroup, blockingPlayer, blockableAction.getBlockFor());

        if(blockableAction.isChallengable())
        {
            processChallenges(playerGroup, blockingPlayer, blockAction);
        }

    }

}

void processChallenges( Player playerGroup[], Player &challengedPlayer, StackAction &challengedAction)
{
    Player * challenger = playerGroup;

    for (int i = 0; i < MAX_ALLOWABLE_PLAYERS; i++)
    {
        challenger = playerGroup + i;

        if (challenger->getID() == challengedPlayer.getID()) // Cannot challenge your own action WILL OVERFLOW ON MATCH FOR SIXTH PLAYER
        {
            i++;
            challenger++;
        }

        if (challenger->willChallenge())
        {
            if (challengedPlayer.hasCardFor(challengedAction.getDeclaredAction()))
            {
                challenger->sacrifice();
            }
            else
            {
                    challengedPlayer.sacrifice();
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
        if((i != activeAction.getCaster().getID()) && playerGroup[i].willBlock())
        {
            blockerFound = true;
            activeAction.setTarget(playerGroup[i]);
        }
    }
}
