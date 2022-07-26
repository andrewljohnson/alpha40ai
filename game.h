#ifndef GAME_H
#define GAME_H

#include <map>
#include <vector>
using namespace std;

#include "enums.h"

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

   void addCard_(Card* c, Player* p);
   Player* playerWithPriority_();
   Player* playerWithoutPriority_();
   Player* turnPlayer_();
   vector<Move*> addAttackMoves_(vector<Move*> moves);
   vector<Move*> addDefenseMoves_(vector<Move*> moves);
   void groupAttackers_(vector<Card*>::iterator begin, vector<Card*>::iterator end, vector<Card*>&readyCreatures, vector<int>& attack);
   void groupDefenders_(map<int, vector<int>>& blockAssignments, const vector<int>& defenders, vector<int>::iterator defendersIterator);
   void printAttackMoves_(vector<Move*>moves);
   void printBlockMoves_(vector<Move*>moves);
   vector<Move*> addPlayPermanentMoves_(vector<Move*> moves);
   vector<Move*> addInstantMoves_(vector<Move*> moves);
   vector<Move*> addPassMove_(vector<Move*> moves);
   void passPriority_();
   void playPassMove_(Move* move);
   void printDecks_();

   public:

   Game();
   vector<Player*> players();
   vector<Move*> stackMove();
   vector<Card*> stackCard();
   int turn();
   game_step gameStep();
   void addPlayer(Player* p);
   void makeDecks();
   void drawOpeningHands();
   void printGameStatus();
   void printValidMoves();
   void playRandomMove();
   bool isOver();
   void addToStack(Move* move, Card* card);
   void setDecks(vector<card_name> p1Deck, vector<card_name> p2Deck);
   void playMove(Move* move);
   vector<Move*> validMoves();
};

#endif