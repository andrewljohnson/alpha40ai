#include <vector>
#include <string>
#include "enums.h"
#include "card.h"
#include "move.h"


class Player {
   vector< Card* > hand;
   vector< Card* > library;
   vector< Card* > inPlay;

   int id_;
   string username_;
   int life = 20;
   bool drewFromEmptyLibrary = false;
   int landsPlayableThisTurn = 1;
   int landsPlayedThisTurn = 0;

   public:
   int id() const { return id_; }
   string username() const { return username_; }
   Player(int playerId, string playerUsername) {
      id_ = playerId;
      username_ = playerUsername;
   }

   void addCardToLibrary (Card* c) {
      library.push_back(c);
   }

   void drawCard () {
      if (library.size() == 0) {
         drewFromEmptyLibrary = true;
         return;
      }
      hand.push_back(library.back());
      library.pop_back();
      // cout << username_ << " drew a card.\n"; 
   }

   int librarySize() {
      return library.size();
   }

   int handSize() {
      return hand.size();      
   }

   int inPlaySize() {
      return inPlay.size();      
   }

   int getLife() {
      return life;      
   }

   int didDrawFromEmptyLibrary() {
      return drewFromEmptyLibrary;      
   }

   vector< Move* > validMoves() {
      vector< Move* > moves;

      // playable land moves
      for (int x=0;x<hand.size();x++) {
         Card* card = hand[x];
            if (card->cardType == Land && landsPlayedThisTurn < landsPlayableThisTurn) {
            Move* move = new Move();
            move->moveType = select_card;
            move->cardId = card->id;
            move->playerId = id_;
            moves.push_back(move);
         }
      }
      Move* move = new Move();
      move->moveType = pass;
      move->playerId = id_;
      moves.push_back(move);
      return moves;
   }

   void playMove(Move* move) {
      if (move->moveType == select_card) {
         int cardIndex = 0;
         for(int x=0;x<hand.size();x++) {
            if (hand[x]->id == move->cardId) {
               cardIndex = x;
               break;
            }
         }
         Card* c = hand[cardIndex];
         inPlay.push_back(c);
         hand.erase(hand.begin() + cardIndex);
         if (c->cardType == Land) {
            landsPlayedThisTurn++;
         }
         return;
      }
   }

   void resetLandsPlayedThisTurn() {
      landsPlayedThisTurn = 0;
   }

};


