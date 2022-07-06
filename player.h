#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <map>
#include <vector>
using namespace std;

class Card;
class Move;
enum mana_type :  int;

class Player {
   private:
   int id_;
   string username_;

   vector<Card*> hand_;
   vector<Card*> library_;
   vector<Card*> graveyard_;
   vector<Card*> inPlay_;
   int life_;

   bool drewFromEmptyLibrary_;
   int landsPlayableThisTurn_;
   int landsPlayedThisTurn_;
   Move* currentAttack_;

   public:
   Player(int playerId, string playerUsername);
   bool canAffordManaCost(map<mana_type, int> manaCost);
   bool canAffordAndTarget(Card* card);
   bool didDrawFromEmptyLibrary();
   int id() { return id_; }
   int landsPlayableThisTurn()  { return landsPlayableThisTurn_; }
   int landsPlayedThisTurn()  { return landsPlayedThisTurn_; }
   int life() { return life_; }
   void discardDown();
   void doUnblockedAttack(vector<Player*>players);

   Move* currentAttack();
   string username();
   vector<Card*> inPlay();
   vector<Card*> hand();
   vector<Card*> graveyard();
   vector<Card*> creatures();
   vector<Card*> lands();
   vector<Card*> library();
   void addCardToLibrary (Card* c);
   void bury(Card *permanent);
   void decrementLife(int life);
   void drawCard ();
   void payMana(map<mana_type, int> manaCost);
   void playMove(Move* move, vector<Player*>players, int turn);
   void resetLandsPlayedThisTurn();
   void setCurrentAttack(Move *move);
   void untapPermanents();
}; 


#endif