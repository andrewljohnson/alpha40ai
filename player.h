#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <string>
#include <vector>
using namespace std;

class Card;
class Game;
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
   
   void playMoveSelectAttackers_(Move* move, Game* game);
   void playMoveSelectDefenders_(Move* move, Game* game);
   void playMoveSelectHandCard_(Move* move, Game* game);
   Player* opponent_(vector<Player*>players);
   void setCurrentAttack_(Move *move);
   void payMana_(map<mana_type, int> manaCost);

   
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
   void playMove(Move* move, Game *game);
   void resetLandsPlayedThisTurn();
   void untapPermanents();
   void resolveMove(Move* move, Card* card, Game* game);
}; 


#endif