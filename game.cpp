#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

#include "card.h"
#include "enums.h"
#include "game.h"
#include "move.h"
#include "player.h"


// public 

Game::Game() {
   indexOfPlayerWithPriority_ = 0;
   lastCardId_ = 0;
   maxHandSize_ = 7;
   turn_ = 0;
   gameStep_ = main_first; 
}

vector<Player*> Game::players() {
   return players_;
}

int Game::turn() {
   return turn_;
}

void Game::addPlayer(Player* p) {
   players_.push_back(p);
}   

void Game::makeDecks() {
   int deckSize = 60;
   Player* p1 = players_[0];
   Player* p2 = players_[1];
   for (int x=0; x<deckSize; x++) {
      if (x % 3 == 0 && x % 2 == 0) {
         addCard_(Card::forest(), p1);
         addCard_(Card::forest(), p2);
      } else if (x % 3 == 0) {
         addCard_(Card::mountain(), p1);
         addCard_(Card::mountain(), p2);
      } else if (x % 2 == 0) {
         // addCard(Card::lightning_bolt(), p1);
         // addCard(Card::lightning_bolt(), p2);            
      } else if (x % 5 == 0) {
         // addCard(Card::grizzly_bear(), p1);
      } else {
         addCard_(Card::grizzly_bear(), p1);
         addCard_(Card::grizzly_bear(), p2);            
      }
   }

   cout << "p1 deck: ";
   for (int x=0; x<players_[0]->library().size(); x++) {
      cout << players_[0]->library()[x]->name() << "(" << players_[0]->library()[x]->id() << ") ";
   }
   cout <<"\n\n";

   cout << "p2 deck: ";
   for (int x=0; x<players_[1]->library().size(); x++) {
      cout << players_[1]->library()[x]->name() << "(" << players_[1]->library()[x]->id() << ") ";
   }
   cout <<"\n\n";
}

void Game::drawOpeningHands() {
   for (int x=0; x<maxHandSize_; x++) {
      players_[0]->drawCard();
      players_[1]->drawCard();
   }
   cout << "p1 hand: ";
   for (int x=0; x<maxHandSize_; x++) {
      cout << players_[0]->hand()[x]->name() << " ";
   }
   cout <<"\n\n";

   cout << "p2 hand: ";
   for (int x=0; x<maxHandSize_; x++) {
      cout << players_[1]->hand()[x]->name() << " ";
   }
   cout <<"\n\n";

   gameStep_ = main_first;
   cout << "~~~ START OF TURN 0 ~~~\n";
}

void Game::printGameStatus() {
   cout << "\n";
   for (int x=0; x < players_.size(); x++) {
      Player* p = players_[x];
      cout << p->username() << ": " << p->life() << " (life)";
      cout << ", " << p->library().size() << " (lib)";
      cout << ", " << p->creatures().size() << " (creat)";
      cout << ", " << p->lands().size() << " (lands)";
      cout << ", " << p->graveyard().size() << " (grave)";
      cout << ", " << p->hand().size() << " (hand).\n";
      if (p->lands().size() > 0) {
         cout << "  ";
         for (Card *land:p->lands()) {
            cout << land->name() << " (" << land->id() << ") ";
         }
         cout << "\n";
      }
      if (p->creatures().size() > 0) {
         cout << "  ";
         for (Card *creature:p->creatures()) {
            cout << creature->name() << " (" << creature->id() << ") ";
         }
         cout << "\n";
      }
   }
}

void Game::printValidMoves() {
   cout << playerWithPriority_()->username() << "'s valid moves in " << gameStep_ << ": ";
   for (Move* move: validMoves_()) {
      cout << "(" << move->moveType << ", " << move->cardId << ", " << move->playerId << "), ";
   }   
   cout << "\n";   
}

void Game::playRandomMove() {
   vector<Move*> moves = validMoves_();
   // use arc4random() instead if we want to change the seed each run on Mac 
   // playMove(moves[rand() % moves. size()]);
   // cout << moves[0]->playerId << " ABOUT TO PLAY MOVE " << moves[0]->moveType << " in step " << gameStep_ << endl;
   playMove_(moves[0]);
}

bool Game::isOver() {
   if (players_[0]->life() <= 0) return true;
   if (players_[1]->life() <= 0) return true;
   if (players_[0]->didDrawFromEmptyLibrary()) return true;
   if (players_[1]->didDrawFromEmptyLibrary()) return true;
   return false;
}


// private

void Game::addCard_(Card* c, Player* p) {
   c->setId(lastCardId_);
   lastCardId_++;
   p->addCardToLibrary(c);
}   

Player* Game::playerWithPriority_() {
   return players_[indexOfPlayerWithPriority_];
}

Player* Game::playerWithoutPriority_() {
   if (indexOfPlayerWithPriority_ == 0) return players_[1];
   return players_[0];
}

Player* Game::turnPlayer_() {
   if( turn_ % 2 == 0) return players_[0];
   return players_[1];
}

vector<Move*> Game::addAttackMoves_(vector<Move*> moves) {
   vector <Card*> readyCreatures;
   for (Card* card: playerWithPriority_()->creatures()) {
      if (!card->tapped && card->turnPlayed < turn_) {
         readyCreatures.push_back(card);
      }
   }
   if (readyCreatures.size() > 0) {
      vector<int> attack;
      moves_ = moves;
      // create all possible attack moves for attack sizes between 0 and readyCreatures.size()
      for (auto it = readyCreatures.begin(); it <= readyCreatures.end(); ++it) {
         groupAttackers_(readyCreatures.begin(), it, readyCreatures, attack);
      }
      // printAttackMoves(moves_);
      moves = moves_;
      // put the move to attack with everything first
      // todo: this seems to expect moves is empty when passed to addAttackMoves, so why is it being passed in at all?
      reverse(moves.begin(), moves.end());
   }
   return moves;
}

vector<Move*> Game::addDefenseMoves_(vector<Move*> moves) {
   if (!playerWithoutPriority_()->currentAttack()) {
      return moves;         
   }
   moves_ = moves;
   map<int, vector<int>> blockAssignments;
   vector<int> defenders;
   for(int attackerId: playerWithoutPriority_()->currentAttack()->attackerIds) {
      vector<int> attackerBucket;
      blockAssignments[attackerId] = attackerBucket;
   }
   for (Card* defendingCreature: playerWithPriority_()->creatures()) {
      if (defendingCreature->tapped) {
         continue;
      }
      defenders.push_back(defendingCreature->id());
   }
   vector<int>::iterator defenderIterator = defenders.begin();
   groupDefenders_(blockAssignments, defenders, defenders.begin());
   moves = moves_;

   return moves;
}

/*
*/
void Game::groupAttackers_(vector<Card*>::iterator begin, vector<Card*>::iterator end, vector<Card*>&readyCreatures, vector<int>& attack) {
   if (end == readyCreatures.begin()) {
      if (attack.size() > 0) {
         Move *moveAttack = new Move(select_attackers, 0, playerWithPriority_()->id());
         moveAttack->attackerIds = attack;
         moves_.push_back(moveAttack);
      }
      return;
   }
   for (auto it = begin; it <= readyCreatures.end() - distance(readyCreatures.begin(), end); ++it) {
      attack.push_back((*it)->id());
      groupAttackers_(it + 1, end - 1, readyCreatures, attack);
      attack.pop_back();
   }
}

/*
   Add moves to this.moves_ for all ways attackers to be blocked. blockAssignments maps ids of attackers to 
   vectors of defender ids for which blocks have already been chosen. defenders specify defenders that 
   could block any of the attackers and defendersIterator specifies which defenders haven't bee allocated 
   yet. This method iterates over all ways to choose blocks for the remaining defenders.

   example input: groupDefenders({0:[2 3] 1:[4 5]}, [6 7 8 9], 3)

   output: adds 3 select_defenders moves to this.moves_:
   0: 2 3
   1: 4 5
   
   0: 2 3 9
   1: 4 5
   
   0: 2 3
   1: 4 5 9
*/

void Game::groupDefenders_(map<int, vector<int>>& blockAssignments, const vector<int>& defenders, vector<int>::iterator defendersIterator) {
   // don't include the move where all attackers are unblocked... that is covered by a "pass" move that gets added elsewhere
   if (defendersIterator - defenders.begin() != 0) {
      Move *moveDefend = new Move(select_defenders, 0, playerWithPriority_()->id());
      moveDefend->blocks = blockAssignments;
      moves_.push_back(moveDefend);
   }

   if(defenders.end() - defendersIterator <= 0) {
      return;
   }
   
   // choose all the possible ways that defenders[indexOfDefender] can be allocated
   for (auto& attacker : blockAssignments) {
      attacker.second.push_back(*defendersIterator);
      groupDefenders_(blockAssignments, defenders, ++defendersIterator);
      attacker.second.pop_back();
   }
}

void Game::printAttackMoves_(vector<Move*>moves) {
   for (Move* move: moves) {
      cout << move->moveType << "\n";
      for (int x : move->attackerIds) {
         cout << x << " ";
      }
      cout << "\n";
   }
}

void Game::printBlockMoves_(vector<Move*>moves) {
   for (Move* move: moves) {
      cout << move->moveType << "\n";
      for (auto const& x : move->blocks) {
         cout << x.first << ": ";
         for (int defender:x.second) {
            cout << defender << " ";
         }
         cout << "\n";
      }
   }
}

vector<Move*> Game::addPlayPermanentMoves_(vector<Move*> moves) {
   map<card_name, bool> cardNamesWithMoves;
   for (Card* card: playerWithPriority_()->hand()) {
      if (cardNamesWithMoves[card->name()]) {
         continue;
      }
      bool isPlayableLand = card->cardType() == Land && playerWithPriority_()->landsPlayedThisTurn() < playerWithPriority_()->landsPlayableThisTurn();
      bool isPlayableCreature = card->cardType() == Creature && playerWithPriority_()->canAffordManaCost(card->manaCost);
      if (isPlayableLand || isPlayableCreature) {
         Move* move = new Move(isPlayableLand ? select_land : select_card, card->id(), playerWithPriority_()->id());
         cardNamesWithMoves[card->name()] = true;
         moves.push_back(move);
      }
   }
   return moves;
}

vector<Move*> Game::addInstantMoves_(vector<Move*> moves) {
   map<card_name, bool> cardNamesWithMoves;
   for (Card* card: playerWithPriority_()->hand()) {
      if (cardNamesWithMoves[card->name()]) {
         continue;
      }
      // instants
      if (card->cardType() == Instant && playerWithPriority_()->canAffordAndTarget(card)) {
         cardNamesWithMoves[card->name()] = true;
         if (card->effects[0]->targetType == any_player_or_creature) {
            Move* moveOpponent = new Move(select_card_with_targets, card->id(), playerWithPriority_()->id());
            moveOpponent->targetId = playerWithoutPriority_()->id(); 
            moveOpponent->targetType = player; 
            moves.push_back(moveOpponent);

            Move* moveSelf = new Move(select_card_with_targets, card->id(), playerWithPriority_()->id());
            moveSelf->targetId = playerWithPriority_()->id(); 
            moveSelf->targetType = player; 
            moves.push_back(moveSelf);

            for (Player* player: players_) {
               for (Card* permanent: player->inPlay()) {
                  if (permanent->cardType() != Creature) {
                     continue;
                  }
                  Move* moveCreature = new Move(select_card_with_targets, card->id(), playerWithPriority_()->id());
                  moveCreature->targetId = permanent->id(); 
                  moveCreature->targetType = creature; 
                  moves.push_back(moveCreature);
               }
            }

         }
      }
   }
   return moves;
}

vector<Move*> Game::addPassMove_(vector<Move*> moves) {
   Move* move = new Move(pass, 0, playerWithPriority_()->id());
   moves.push_back(move);
   return moves;
}

vector<Move*> Game::validMoves_() {
   vector<Move*> moves;
   if (turnPlayer_()->id() == playerWithPriority_()->id()) {
      if (gameStep_ == attack_step) {
         moves = addAttackMoves_(moves);
      } else if (gameStep_ == main_first || gameStep_ == main_second) {
         moves = addPlayPermanentMoves_(moves);
      }
   } else {
      if (gameStep_ == attack_step) {
         moves = addDefenseMoves_(moves);
      }
   }
   moves = addInstantMoves_(moves);
   moves = addPassMove_(moves);
   return moves;
}

void Game::passPriority_() {
   if (indexOfPlayerWithPriority_ == 0) {
      indexOfPlayerWithPriority_ = 1;
   } else indexOfPlayerWithPriority_ = 0;      
}

void Game::playMove_(Move* move) {
   if (move->moveType == pass) {
      playPassMove_(move);
   } else {
      playerWithPriority_()->playMove(move, this);
      if (move->moveType == select_attackers) {            
         passPriority_();
      }
      if (move->moveType == select_defenders) {
         passPriority_();
         gameStep_ = main_second;               
      }
   }
}

void Game::playPassMove_(Move* move) {
   passPriority_();
   if(turnPlayer_()->id() != playerWithPriority_()->id()) {
      return;
   }
   if (playerWithPriority_()->currentAttack()) {
      playerWithPriority_()->doUnblockedAttack(players_);
   }
   switch(gameStep_) {
      case untap_step:
         // no moves allowed during untap
         break;             
      case upkeep_step:
         gameStep_ = draw_step;               
         break;             
      case main_first:
         gameStep_ = attack_step;               
         break;             
      case attack_step:
         gameStep_ = main_second;               
         break;             
      case main_second:
         gameStep_ = end_step;               
         break;             
      case draw_step:
         gameStep_ = draw_step;                   
         playerWithPriority_()->drawCard();
         if (playerWithPriority_()->didDrawFromEmptyLibrary()) {
            return;
         }
         gameStep_ = main_first;                   
         break;             
      case end_step:
         if(turnPlayer_()->hand().size() > 7) {
            turnPlayer_()->discardDown();
         }
         printGameStatus();
         passPriority_();
         turn_ += 1;                          
         cout << "\n~~~ START OF TURN " << turn_ << "  (" << turnPlayer_()->username() << ") ~~~\n";
         gameStep_ = untap_step;                   
         turnPlayer_()->resetLandsPlayedThisTurn();
         turnPlayer_()->untapPermanents();
         gameStep_ = upkeep_step;     
         break;              
   }
}
