#include <iostream>
#include "Classes.h"
#include <cstdlib>

using namespace std;

void initialDeal (Player playerGroup[], int playerCount);
int playersAlive(Player playerGroup[]);
void processTurn(Player playerGroup[], Player &activePlayer);
void processActive(Player playerGroup[], Player &activePlayer, StackAction &activeAction);
void processBlock(Player playerGroup[], Player blockingPlayer, StackAction &blockableAction);
void processChallenges( Player playerGroup[], Player &challengedPlayer, StackAction &challengedAction);

int main()
{
   srand(1338); //random seed

    int players = 2; //stub.  query in future

    Player playerPosition [MAX_ALLOWABLE_PLAYERS]{};
    initialDeal(playerPosition, players);

    Player * activePlayer = playerPosition + (rand() % players);

    while (players > 1)
    {
        if (activePlayer->isAlive())
        {
            processTurn(playerPosition, *activePlayer);
        }

        activePlayer = playerPosition + ((activePlayer->getID() + 1) % players); // Permute to next player
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

void processTurn(Player playerGroup[], Player &activePlayer)
{
    system("cls");

    StackAction activeAction{};
    processActive(playerGroup, activePlayer, activeAction);

    if (activeAction.getStatus() == VALID && activeAction.isBlockable())
    {
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
    int idStr = activePlayer.getID();
    int iskStr = activePlayer.hasIsk();
    string handStr = activePlayer.listHand();

    std::cout << "Player " << idStr << " is the active player\n\n"
              << "You have the following unexposed card(s) in your hand: \n" << handStr << ".\n"
              << "You have " << iskStr << " ISK.\n\n"
              << "Which action would you like to take? [1-7]\n\n"
              << "[1] Take income\n"
              << "[2] Take foreign aid\n"
              << "[3] Stage a coup\n"
              << "[4] Tax\n"
              << "[5] Assassinate\n"
              << "[6] Exchange\n"
              << "[7] Steal\n\n"
              << "Your choice: ";

    std::cin >> menuSelection;//getSelection(menuSelection, 7);

    activeAction = StackAction(playerGroup, activePlayer, static_cast<Action>(menuSelection));

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

