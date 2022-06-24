#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <map>
#include <vector>
using namespace std;
class Card;
class Move;
enum mana_type :  int;


class Player
{
   private:
   int id_;
   string username_;

   vector<Card*> hand_;
   vector<Card*> library_;
   vector<Card*> inPlay_;
   int life_;

   bool drewFromEmptyLibrary_;
   int landsPlayableThisTurn_;
   int landsPlayedThisTurn_;
   bool canAffordManaCost(map<mana_type, int> manaCost);


   public:
   Player(int playerId, string playerUsername);
   string username();
   int id();
   int life();
   void addCardToLibrary (Card* c);
   void playMove(Move* move, vector<Player*>players);
   int didDrawFromEmptyLibrary();
   void drawCard ();
   void resetLandsPlayedThisTurn();
   void untapPermanents();
   void decrementLife(int life);
   vector<Card*> inPlay();
   vector<Card*> hand();
   vector<Card*> library();
   int landsPlayableThisTurn()  { return landsPlayableThisTurn_; }
   int landsPlayedThisTurn()  { return landsPlayedThisTurn_; }
   bool canAffordAndTarget(Card* card);
}; 

#endif