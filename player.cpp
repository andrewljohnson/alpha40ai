#include "player.h"
#include "move.h"
#include "card.h"
#include "enums.h"
#include <iostream>
using namespace std;


ostream& operator<<(ostream& out, const mana_type value){
    const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
    switch(value){
        PROCESS_VAL(red);     
        PROCESS_VAL(green);     
        PROCESS_VAL(blue);     
        PROCESS_VAL(black);     
        PROCESS_VAL(white);     
        PROCESS_VAL(colorless);     
    }
#undef PROCESS_VAL
    return out << s;
}


int id_;
string username_;

Player::Player(int playerId, string playerUsername) {
   id_ = playerId;
   username_ = playerUsername;
   landsPlayableThisTurn_ = 1;
   life_ = 20;
   drewFromEmptyLibrary_ = false;
}

vector<Card*> hand_;
vector<Card*> library_;
vector<Card*> inPlay_;

int Player::id() { return id_; }
int Player::life()  { return life_; }
vector<Card*> Player::hand()  { return hand_; }
vector<Card*> Player::library()  { return library_; }
vector<Card*> Player::inPlay()  { return inPlay_; }
string Player::username()  { return username_; }

void Player::decrementLife(int life) {
   life_ -= life;
}

void Player::addCardToLibrary (Card* c) {
   library_.push_back(c);
}

void Player::drawCard () {
   if (library_.size() == 0) {
      drewFromEmptyLibrary_ = true;
      return;
   }
   hand_.push_back(library_.back());
   library_.pop_back();
   // cout << username_ << " drew a card.\n"; 
}

int Player::didDrawFromEmptyLibrary() {
   return drewFromEmptyLibrary_;      
}

void Player::playMove(Move* move, vector<Player*>players) {
   if (move->moveType == select_card || move->moveType == select_card_with_targets) {
      int cardIndex = 0;
      for(int x=0;x<hand_.size();x++) {
         if (hand_[x]->id == move->cardId) {
            cardIndex = x;
            break;
         }
      }
      Card* card = hand_[cardIndex];
      if (move->moveType == select_card) {
         inPlay_.push_back(card);
         hand_.erase(hand_.begin() + cardIndex);
         if (card->cardType == Land) {
            landsPlayedThisTurn_++;
         }            
      }
      if (move->moveType == select_card_with_targets) {
            map<mana_type, int> manaCost = card->manaCost;
            for ( const auto &p : manaCost )
            {
               for (int x=0;x<p.second;x++) {
                  for (Card* inPlayCard: inPlay_) {
                     if (!inPlayCard->tapped && inPlayCard->cardType == Land) {
                        if ((inPlayCard->effects[0]->name == mana_red && p.first == red)
                           || p.first == colorless) {
                           inPlayCard->tapped = true;
                        }
                     }
                  }   

               } 
            } 

            for (Effect* effect: card->effects) {
               if (effect->trigger == cast_this) {
                  card->castSpellEffectDefs[0](move, effect, players);
               }
            }
            hand_.erase(hand_.begin() + cardIndex);
      }
   }
}

void Player::resetLandsPlayedThisTurn() {
   landsPlayedThisTurn_ = 0;
}

void Player::untapPermanents() {
  for (Card* inPlayCard: inPlay_) {
      inPlayCard->tapped = false;
  } 
}


bool Player::canAffordManaCost(map<mana_type, int> manaCost) {
   map<mana_type, int> manaAvailable;
   for (Card* card: inPlay_) {
      if (!card->tapped && card->cardType == Land) {
         if (card->effects[0]->name == mana_red) {
            manaAvailable[red] += card->effects[0]->amount;
         }
      }
   }
   if (manaCost[red] > manaAvailable[red]) {
      return false;
   }
   return true;
}

bool Player::canAffordAndTarget(Card* card) {
   bool canTarget = false;
   if (card->effects[0]->targetType == any_player_or_creature) {
      canTarget = true;
   }
   return canTarget && canAffordManaCost(card->manaCost);
}


