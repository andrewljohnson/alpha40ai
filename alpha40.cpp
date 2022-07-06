#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include "card.h"
#include "enums.h"
#include "move.h"
#include "player.h"
using namespace std;


class Game {

   // the index of the player that currently has priority to play moves
   int indexOfPlayerWithPriority_ = 0;

   // each new card created in a game has a sequential ID
   int lastCardId = 0;

   // the max number of cards a player can have at the end of a turn
   int maxHandSize = 7;

   // the current turn, which consists of a series of steps 
   int turn_ = 0;

   // the current step in a turn
   game_step gameStep = main_first; 

   // a vector of players in the game
   vector<Player*> players_;

   vector<Move*> moves_;

   public:

   int turn() {
      return turn_;
   }

   void addPlayer (Player* p) {
      players_.push_back(p);
   }   

   void addCard (Card* c, Player* p) {
      c->setId(lastCardId);
      lastCardId++;
      p->addCardToLibrary(c);
   }   

   void makeDecks() {
      int deckSize = 60;
      Player* p1 = players_[0];
      Player* p2 = players_[1];
      for (int x=0; x<deckSize; x++) {
         if (x % 3 == 0 && x % 2 == 0) {
            addCard(Card::forest(), p1);
            addCard(Card::forest(), p2);
         } else if (x % 3 == 0) {
            addCard(Card::mountain(), p1);
            addCard(Card::mountain(), p2);
         } else if (x % 2 == 0) {
            // addCard(Card::lightning_bolt(), p1);
            // addCard(Card::lightning_bolt(), p2);            
         } else if (x % 5 == 0) {
            addCard(Card::grizzly_bear(), p1);
         } else {
            // addCard(Card::grizzly_bear(), p1);
            addCard(Card::grizzly_bear(), p2);            
         }
      }
   }

   void drawOpeningHands() {
      for (int x=0; x<maxHandSize; x++) {
         players_[0]->drawCard();
         players_[1]->drawCard();
      }
      gameStep = main_first;
      cout << "~~~ START OF TURN 0 ~~~\n";
   }

   void printGameStatus() {
      cout << "\n";
      for (int x=0; x < players_.size(); x++) {
         Player* p = players_[x];
         cout << p->username() << ": " << p->life() << " (life)";
         cout << ", " << p->library().size() << " (lib)";
         cout << ", " << p->creatures().size() << " (creat)";
         cout << ", " << p->lands().size() << " (lands)";
         cout << ", " << p->graveyard().size() << " (grave)";
         cout << ", " << p->hand().size() << " (hand).\n";
      }
   }

   void printValidMoves() {
      cout << playerWithPriority()->username() << "'s valid moves in " << gameStep << ": ";
      for (Move* move: validMoves()) {
         cout << "(" << move->moveType << ", " << move->cardId << ", " << move->playerId << "), ";
      }   
      cout << "\n";   
   }

   Player* playerWithPriority() {
      return players_[indexOfPlayerWithPriority_];
   }

   Player* playerWithoutPriority() {
      if (indexOfPlayerWithPriority_ == 0) return players_[1];
      return players_[0];
   }

   Player* turnPlayer() {
      if( turn_ % 2 == 0) return players_[0];
      return players_[1];
   }

   vector<Move*> addAttackMoves(vector<Move*> moves) {
      vector <int> attackableCreatureIds;
      for (Card* card: playerWithPriority()->creatures()) {
         if (!card->tapped && card->turnPlayed < turn_) {
            attackableCreatureIds.push_back(card->id());
         }
      }
      if (attackableCreatureIds.size() > 0) {
         vector<int> combination;
         moves_ = moves;
         for (int x=0;x<attackableCreatureIds.size()+1;x++) {
            go(0, x, attackableCreatureIds, combination);
         }
         moves = moves_;
         // put the move to attack with everything first
         reverse(moves.begin(), moves.end());
      }
      return moves;
   }

   static void pretty_print(const vector<int>& v) {
     int count = 0;
     for (int i = 0; i < v.size(); ++i) { cout << v[i] << " "; }
     cout << "] " << endl;
   }


   void go(int offset, int k, vector<int>attackableCreatureIds, vector<int> combination) {
      if (k == 0) {
         Move *moveAttack = new Move(select_attackers, 0, playerWithPriority()->id());
         moveAttack->attackerIds = combination;
         if (combination.size() > 0) {
            moves_.push_back(moveAttack);
         }
         // Game::pretty_print(combination);
         return;
      }
      for (int i = offset; i <= attackableCreatureIds.size() - k; ++i) {
         combination.push_back(attackableCreatureIds[i]);
         go(i+1, k-1, attackableCreatureIds, combination);
         combination.pop_back();
      }
   }
   /*
     Print out all ways for attackers to be blocked. blockAssignments maps ids of attackers to 
     vectors of defender ids for which blocks have already been chosen. defenders/defendersIterator specify
     defenders that could block any of the attackers. This method iterates over all ways to choose blocks for 
     the remaining defenders.

     example input: groupDefenders({0:[2 3] 1:[4 5]}, [6 7 8 9], 3)
      
     output:

      0: 2 3
      1: 4 5
      
      0: 2 3 9
      1: 4 5
      
      0: 2 3
      1: 4 5 9
   */
   void groupDefenders(map<int, vector<int>> blockAssignments, const vector<int>& defenders, vector<int>::iterator defendersIterator) {
      int x = 0;
      for (auto const& x : blockAssignments) {
         cout << x.first << ": ";
         for (int defender:x.second) {
            cout << defender << " ";
         }
         cout << "\n";
      }

      if(defendersIterator == defenders.end()) {
         return;
      }
      
      // choose all the possible ways that defenders[indexOfDefender] can be allocated, and print out each of them.
      for (auto& attacker : blockAssignments) {
         // push a defender into blockAssignments
         attacker.second.push_back(*defendersIterator);
         // recurse to print it and continue to push more defenders recursively
         groupDefenders(blockAssignments, defenders, ++defendersIterator);
         // pop the defender so it can be pushed to the next possible attacker in blockAssignments when looping
         attacker.second.pop_back();
      }
   }

   vector<Move*> addDefenseMoves(vector<Move*> moves) {
      if (!playerWithoutPriority()->currentAttack()) {
         return moves;         
      }
      map<int, vector<int>> blockAssignments;
      vector<int> defenders;
      for(int attackerId: playerWithoutPriority()->currentAttack()->attackerIds) {
         vector<int> attackerBucket;
         blockAssignments[attackerId] = attackerBucket;
      }
      for (Card* defendingCreature: playerWithPriority()->creatures()) {
         if (defendingCreature->tapped) {
            continue;
         }
         defenders.push_back(defendingCreature->id());
      }
      vector<int>::iterator defenderIterator = defenders.begin();
      groupDefenders(blockAssignments, defenders, defenderIterator);

      map<int, int> blocks;
      for (Card* defendingCreature: playerWithPriority()->creatures()) {
         if (defendingCreature->tapped) {
            continue;
         }
         defenders.push_back(defendingCreature->id());
         for(int attackerId: playerWithoutPriority()->currentAttack()->attackerIds) {
            if (blocks.find(attackerId) == blocks.end()) {
               blocks[attackerId] = defendingCreature->id();
               break;
            }
         }
      }

      if (blocks.size() > 0) {
         Move* moveDefend = new Move(select_defenders, 0, playerWithPriority()->id());
         moveDefend->blocks = blocks;
         moves.push_back(moveDefend);
      }

      return moves;
   }

   vector<Move*> addPlayPermanentMoves(vector<Move*> moves) {
      map<card_name, bool> cardNamesWithMoves;
      for (Card* card: playerWithPriority()->hand()) {
         if (cardNamesWithMoves[card->name()]) {
            continue;
         }
         bool isPlayableLand = card->cardType() == Land && playerWithPriority()->landsPlayedThisTurn() < playerWithPriority()->landsPlayableThisTurn();
         bool isPlayableCreature = card->cardType() == Creature && playerWithPriority()->canAffordManaCost(card->manaCost);
         if (isPlayableLand || isPlayableCreature) {
            Move* move = new Move(select_card, card->id(), playerWithPriority()->id());
            cardNamesWithMoves[card->name()] = true;
            moves.push_back(move);
         }
      }
      return moves;
   }

   vector<Move*> addInstantMoves(vector<Move*> moves) {
      map<card_name, bool> cardNamesWithMoves;
      for (Card* card: playerWithPriority()->hand()) {
         if (cardNamesWithMoves[card->name()]) {
            continue;
         }
         // instants
         if (card->cardType() == Instant && playerWithPriority()->canAffordAndTarget(card)) {
            cardNamesWithMoves[card->name()] = true;
            if (card->effects[0]->targetType == any_player_or_creature) {
               Move* moveOpponent = new Move(select_card_with_targets, card->id(), playerWithPriority()->id());
               moveOpponent->targetId = playerWithoutPriority()->id(); 
               moveOpponent->targetType = player; 
               moves.push_back(moveOpponent);

               Move* moveSelf = new Move(select_card_with_targets, card->id(), playerWithPriority()->id());
               moveSelf->targetId = playerWithPriority()->id(); 
               moveSelf->targetType = player; 
               moves.push_back(moveSelf);

               for (Player* player: players_) {
                  for (Card* permanent: player->inPlay()) {
                     if (permanent->cardType() != Creature) {
                        continue;
                     }
                     Move* moveCreature = new Move(select_card_with_targets, card->id(), playerWithPriority()->id());
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

   vector<Move*> addPassMove(vector<Move*> moves) {
      Move* move = new Move(pass, 0, playerWithPriority()->id());
      moves.push_back(move);
      return moves;
   }

   vector<Move*> validMoves() {
      vector<Move*> moves;
      if (turnPlayer()->id() == playerWithPriority()->id()) {
         if (gameStep == attack_step) {
            moves = addAttackMoves(moves);
         } else if (gameStep == main_first || gameStep == main_second) {
            moves = addPlayPermanentMoves(moves);
         }
      } else {
         if (gameStep == attack_step) {
            moves = addDefenseMoves(moves);
         }
      }
      moves = addInstantMoves(moves);
      moves = addPassMove(moves);
      return moves;
   }

   void passPriority() {
      if (indexOfPlayerWithPriority_ == 0) {
         indexOfPlayerWithPriority_ = 1;
      } else indexOfPlayerWithPriority_ = 0;      
   }

   void playMove(Move* move) {
      if (move->moveType == pass) {
         passPriority();
         if(turnPlayer()->id() == playerWithPriority()->id()) {
            if (playerWithPriority()->currentAttack()) {
               playerWithPriority()->doUnblockedAttack(players_);
            }

            switch(gameStep) {
               case untap_step:
                  // no moves allowed during untap
                  break;             
               case upkeep_step:
                  gameStep = draw_step;               
                  break;             
               case main_first:
                  gameStep = attack_step;               
                  break;             
               case attack_step:
                  gameStep = main_second;               
                  break;             
               case main_second:
                  gameStep = end_step;               
                  break;             
               case draw_step:
                  gameStep = draw_step;                   
                  playerWithPriority()->drawCard();
                  if (playerWithPriority()->didDrawFromEmptyLibrary()) {
                     return;
                  }
                  gameStep = main_first;                   
                  break;             
               case end_step:
                  if(turnPlayer()->hand().size() > 7) {
                     turnPlayer()->discardDown();
                  }
                  printGameStatus();
                  passPriority();
                  turn_ += 1;                          
                  cout << "\n~~~ START OF TURN " << turn_ << "  (" << turnPlayer()->username() << ") ~~~\n";
                  gameStep = untap_step;                   
                  turnPlayer()->resetLandsPlayedThisTurn();
                  turnPlayer()->untapPermanents();
                  gameStep = upkeep_step;     
                  break;              
            }
         } else {
         }
      } else {
         playerWithPriority()->playMove(move, players_, turn_);
         if (move->moveType == select_attackers) {            
            passPriority();
         }
         if (move->moveType == select_defenders) {
            passPriority();
            gameStep = main_second;               
         }
      }
   }

   void playRandomMove() {
      vector<Move*> moves = validMoves();
      // use arc4random() instead if we want to change the seed each run on Mac 
      // playMove(moves[rand() % moves. size()]);
      playMove(moves[0]);
   }

   bool isOver() {
      if (players_[0]->life() <= 0) return true;
      if (players_[1]->life() <= 0) return true;
      if (players_[0]->didDrawFromEmptyLibrary()) return true;
      if (players_[1]->didDrawFromEmptyLibrary()) return true;
      return false;
   }
};


int main() {
   Game game;
   game.addPlayer(new Player(0, "Spk"));
   game.addPlayer(new Player(1, "Lee"));
   game.makeDecks();
   game.drawOpeningHands();

   int turnsToPlay = 5;
   while (!game.isOver()) {
      game.playRandomMove();
      if (game.isOver()) {
         cout << "\n~~~~~~GAME OVER~~~~~~\n";
      }
      if (game.turn() >= turnsToPlay) {
         break;
      }
   }
}
