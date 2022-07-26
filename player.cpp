#include <algorithm>
using namespace std;

#include "card.h"
#include "enums.h"
#include "game.h"
#include "move.h"
#include "player.h"


// public

Player::Player(int playerId, string playerUsername) {
   id_ = playerId;
   username_ = playerUsername;
   landsPlayableThisTurn_ = 1;
   life_ = 20;
   drewFromEmptyLibrary_ = false;
}

string Player::username() { return username_; }
vector<Card*> Player::hand()  { return hand_; }
vector<Card*> Player::library()  { return library_; }
vector<Card*> Player::graveyard()  { return graveyard_; }
vector<Card*> Player::inPlay()  { return inPlay_; }

vector<Card*> Player::creatures()  { 
   vector<Card*> creatures;
   for (Card* inPlayCard: inPlay_) {
      if (inPlayCard->cardType() == Creature) {
         creatures.push_back(inPlayCard);
      }
   }
   return creatures; 
}

vector<Card*> Player::lands()  { 
   vector<Card*> lands;
   for (Card* inPlayCard: inPlay_) {
      if (inPlayCard->cardType() == Land) {
         lands.push_back(inPlayCard);
      }
   }
   return lands; 
}

void Player::decrementLife(int life) {
   life_ -= life;
}

void Player::addCardToLibrary(Card* c) {
   library_.push_back(c);
}

void Player::drawCard() {
   if (library_.size() == 0) {
      drewFromEmptyLibrary_ = true;
      return;
   }
   hand_.push_back(library_.back());
   library_.pop_back();
}

bool Player::didDrawFromEmptyLibrary() {
   return drewFromEmptyLibrary_;      
}

void Player::bury(Card *permanent) {
   cout << permanent->name() << " (" << permanent->id() << ") was buried.\n";
   graveyard_.push_back(permanent);
   int cardIndex = 0;
   for(int x=0;x<inPlay_.size();x++) {
      if (inPlay_[x]->id() == permanent->id()) {
         cardIndex = x;
         break;
      }
   }
   inPlay_.erase(inPlay_.begin() + cardIndex);
}

void Player::doUnblockedAttack(vector<Player*>players) {
   Player *opponent = players[0];
   if (opponent->id() == id_) {
      opponent = players[1];
   }
   if (currentAttack()->attackerIds.size()) {
      for (int aid:currentAttack()->attackerIds) {
         Card *attacker;
         for (Card* creature:creatures()) {
            if (creature->id() == aid) {
               attacker = creature;
            }
         } 
         opponent->decrementLife(attacker->power());
      }

   }
   currentAttack_ = 0;
}

void Player::playMove(Move* move, Game *game) {
   switch(move->moveType) {
      case select_attackers:
         playMoveSelectAttackers_(move, game);
         break;             
      case select_defenders:
         playMoveSelectDefenders_(move, game);
         break;             
      case select_land:
      case select_card:
      case select_card_with_targets:
         playMoveSelectHandCard_(move, game);
         break;             
      case pass:
         // this case is never called, all logic for the pass move is handled by the caller in alpha40.cpp
         break;
   }
}

bool Player::canAffordManaCost(map<mana_type, int> manaCost) {
   map<mana_type, int> manaAvailable;
   for (Card* card: inPlay_) {
      if (!card->tapped && card->cardType() == Land) {
         if (card->effects[0]->name == mana_red) {
            manaAvailable[red] += card->effects[0]->amount;
         }
         if (card->effects[0]->name == mana_green) {
            manaAvailable[green] += card->effects[0]->amount;
         }
      }
   }
   if (manaCost[red] > manaAvailable[red]) {
      return false;
   }
   if (manaCost[green] > manaAvailable[green]) {
      return false;
   }
   if (manaCost[red] + manaCost[green] + manaCost[colorless] > 
      manaAvailable[red] + manaAvailable[green]) {
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

void Player::discardDown() {
   cout << "discarding down\n\n";
   while (hand_.size() > 7) {
      graveyard_.push_back(hand_[0]);
      hand_.erase(hand_.begin());   
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

Move* Player::currentAttack() {
   return currentAttack_;
}


// private

void Player::payMana_(map<mana_type, int> manaCost) {
   for ( const auto &p : manaCost ) {
      if (p.first != colorless) {
         bool tappedThisTime = false;
         for (int x=0;x<p.second;x++) {
            if (tappedThisTime) {
               continue;
            }
            for (Card* inPlayCard: inPlay_) {
               if (!inPlayCard->tapped && inPlayCard->cardType() == Land) {
                  if (inPlayCard->effects[0]->name == mana_red && p.first == red)
                      {
                     inPlayCard->tapped = true;
                     tappedThisTime = true;
                  }
                  if (inPlayCard->effects[0]->name == mana_green && p.first == green)
                     {
                     inPlayCard->tapped = true;
                     tappedThisTime = true;
                  }
               }
            }
         }   
      } 
   }
   for (int x=0;x<manaCost[colorless];x++) {
      bool tappedThisTime = false;
      for (Card* inPlayCard: inPlay_) {
            if (tappedThisTime) {
               continue;
            }
            if (!inPlayCard->tapped && inPlayCard->cardType() == Land) {
               inPlayCard->tapped = true;
               tappedThisTime = true;
            }
      }
   }   
}             

Player* Player::opponent_(vector<Player*>players) {
   Player *opponent = players[0];
   if (opponent->id() == id_) {
      opponent = players[1];
   }
   return opponent;
}

void Player::playMoveSelectAttackers_(Move* move, Game *game) {
   for (Card *creature: creatures()) {
      auto findIndex = find(move->attackerIds.begin(), move->attackerIds.end(), creature->id());
      if (findIndex != move->attackerIds.end()) {
         Card *attacker = creatures()[findIndex - move->attackerIds.begin()];
         int power = attacker->power();
         cout << username_ << " attacks for " << power << " with " << attacker->name() << " (" << attacker->id() << ").\n";
         creatures()[findIndex - move->attackerIds.begin()]->tapped = true;
      }
   }
   currentAttack_ = move;
}

void Player::playMoveSelectDefenders_(Move* move, Game *game) {
   for (int aid:opponent_(game->players())->currentAttack()->attackerIds) {
      Card *attacker;
      for (Card* creature:opponent_(game->players())->creatures()) {
         if (creature->id() == aid) {
            attacker = creature;
         }
      } 
      if (move->blocks[aid].size() > 0) {

         Card *defender = NULL;
         for (Card* defendingCreature:creatures()) {
            for(int defenderId:move->blocks[aid]) {
               if (defendingCreature->id() == defenderId) {
                  defender = defendingCreature;
               }
            }
         } 
         cout << defender->name() << " (" << defender->id() << ") blocks " << attacker->name() << ".\n";
         defender->takeDamage(this, attacker->power());
         if (defender->damage() >= defender->toughness()) {
            this->bury(defender);
         }
         attacker->takeDamage(opponent_(game->players()), defender->power());
         if (attacker->damage() >= attacker->toughness()) {
            opponent_(game->players())->bury(attacker);
         }
      } else {
         decrementLife(attacker->power());
      }
   }
   opponent_(game->players())->setCurrentAttack_(0);      
}

void Player::playMoveSelectHandCard_(Move* move, Game *game) {
   int cardIndex = 0;
   for(int x=0;x<hand_.size();x++) {
      if (hand_[x]->id() == move->cardId) {
         cardIndex = x;
         break;
      }
   }
   Card* card = hand_[cardIndex];
   if (move->moveType == select_land || move->moveType == select_card) {
      if (move->moveType == select_land) {
         cout << username_ << " plays " << card->name() << ".\n";
         landsPlayedThisTurn_++;
         card->turnPlayed = game->turn();
         inPlay_.push_back(card);
         hand_.erase(hand_.begin() + cardIndex);
      }            
      if (move->moveType == select_card) {
         cout << username_ << " starts to play " << card->name() << ".\n";
         payMana_(card->manaCost);
         card->turnPlayed = game->turn();
         game->addToStack(move, card);
         hand_.erase(hand_.begin() + cardIndex);
      }
   }
   // todo: stack
   if (move->moveType == select_card_with_targets) {
         cout << username_ << " starts to play " << card->name() << ".\n";
         map<mana_type, int> manaCost = card->manaCost;
         for ( const auto &p : manaCost )
         {
            for (int x=0;x<p.second;x++) {
               for (Card* inPlayCard: inPlay_) {
                  if (!inPlayCard->tapped && inPlayCard->cardType() == Land) {
                     if ((inPlayCard->effects[0]->name == mana_red && p.first == red)
                        || p.first == colorless) {
                        inPlayCard->tapped = true;
                     }
                  }
               }   

            } 
         } 

         game->addToStack(move, card);
         hand_.erase(hand_.begin() + cardIndex);
   }
}

void Player::resolveMove(Move* move, Card* card, Game* game) {
   cout << username_ << " plays " << card->name() << ".\n";
   if (move->moveType == select_card) {
      inPlay_.push_back(card);
   } else if (move->moveType == select_card_with_targets) {
      for (Effect* effect: card->effects) {
         if (effect->trigger == cast_this) {
            card->castSpellEffectDefs[0](move, card, effect, game->players());
         }
      }
      graveyard_.push_back(card);
   }
 }

void Player::setCurrentAttack_(Move *move) {
   currentAttack_ = move;
}

