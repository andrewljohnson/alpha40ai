#ifndef GAME_H
#define GAME_H

#include "enums.h"
#include <map>
#include <vector>
using namespace std;
class Card;
class Move;
class Player;


class Game {

   private:
   
   // the index of the player that currently has priority to play moves
   int indexOfPlayerWithPriority_;

   // each new card created in a game has a sequential ID
   int lastCardId_;

   // the max number of cards a player can have at the end of a turn
   int maxHandSize_;

   // the current turn, which consists of a series of steps 
   int turn_;

   // the current step in a turn
   game_step gameStep_; 

   // a vector of players in the game
   vector<Player*> players_;

   vector<Move*> moves_;

   vector<Move*> stackMove_;

   vector<Card*> stackCard_;


   public:
   Game();

   vector<Player*> players();
   int turn();


   void addPlayer (Player* p);

   void addCard (Card* c, Player* p);

   void makeDecks();

   void drawOpeningHands();

   void printGameStatus();

   void printValidMoves();

   Player* playerWithPriority();

   Player* playerWithoutPriority();

   Player* turnPlayer();

   vector<Move*> addAttackMoves(vector<Move*> moves);

   vector<Move*> addDefenseMoves(vector<Move*> moves);

   void groupAttackers(vector<Card*>::iterator begin, vector<Card*>::iterator end, vector<Card*>&readyCreatures, vector<int>& attack);

   void groupDefenders(map<int, vector<int>>& blockAssignments, const vector<int>& defenders, vector<int>::iterator defendersIterator);

   void printAttackMoves(vector<Move*>moves);

   void printBlockMoves(vector<Move*>moves);

   vector<Move*> addPlayPermanentMoves(vector<Move*> moves);

   vector<Move*> addInstantMoves(vector<Move*> moves);

   vector<Move*> addPassMove(vector<Move*> moves);

   vector<Move*> validMoves();

   void passPriority();

   void playMove(Move* move);

   void playPassMove_(Move* move);

   void playRandomMove();

   bool isOver();

};

#endif