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

   vector<Move*> stack_;

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

   void drawOpeningHands() {
      for (int x=0; x<maxHandSize; x++) {
         players_[0]->drawCard();
         players_[1]->drawCard();
      }
      cout << "p1 hand: ";
      for (int x=0; x<maxHandSize; x++) {
         cout << players_[0]->hand()[x]->name() << " ";
      }
      cout <<"\n\n";

      cout << "p2 hand: ";
      for (int x=0; x<maxHandSize; x++) {
         cout << players_[1]->hand()[x]->name() << " ";
      }
      cout <<"\n\n";

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
      vector <Card*> readyCreatures;
      for (Card* card: playerWithPriority()->creatures()) {
         if (!card->tapped && card->turnPlayed < turn_) {
            readyCreatures.push_back(card);
         }
      }
      if (readyCreatures.size() > 0) {
         vector<int> attack;
         moves_ = moves;
         // create all possible attack moves for attack sizes between 0 and readyCreatures.size()
         for (auto it = readyCreatures.begin(); it <= readyCreatures.end(); ++it) {
            generateAttackMoves(readyCreatures.begin(), it, readyCreatures, attack);
         }
         // printAttackMoves(moves_);
         moves = moves_;
         // put the move to attack with everything first
         // todo: this seems to expect moves is empty when passed to addAttackMoves, so why is it being passed in at all?
         reverse(moves.begin(), moves.end());
      }
      return moves;
   }

   /*
   */
   void generateAttackMoves(vector<Card*>::iterator begin, vector<Card*>::iterator end, vector<Card*>&readyCreatures, vector<int>& attack) {
      if (end == readyCreatures.begin()) {
         if (attack.size() > 0) {
            Move *moveAttack = new Move(select_attackers, 0, playerWithPriority()->id());
            moveAttack->attackerIds = attack;
            moves_.push_back(moveAttack);
         }
         return;
      }
      for (auto it = begin; it <= readyCreatures.end() - distance(readyCreatures.begin(), end); ++it) {
         attack.push_back((*it)->id());
         generateAttackMoves(it + 1, end - 1, readyCreatures, attack);
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
   void groupDefenders(map<int, vector<int>>& blockAssignments, const vector<int>& defenders, vector<int>::iterator defendersIterator) {
      // don't include the move where all attackers are unblocked... that is covered by a "pass" move that gets added elsewhere
      if (defendersIterator - defenders.begin() != 0) {
         Move *moveDefend = new Move(select_defenders, 0, playerWithPriority()->id());
         moveDefend->blocks = blockAssignments;
         moves_.push_back(moveDefend);
      }
      if(defenders.end() - defendersIterator <= 0) {
         return;
      }
      
      // choose all the possible ways that defenders[indexOfDefender] can be allocated, and print out each of them.
      for (auto& attacker : blockAssignments) {
         attacker.second.push_back(*defendersIterator);
         groupDefenders(blockAssignments, defenders, ++defendersIterator);
         attacker.second.pop_back();
      }
   }

   vector<Move*> addDefenseMoves(vector<Move*> moves) {
      if (!playerWithoutPriority()->currentAttack()) {
         return moves;         
      }
      moves_ = moves;
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
      groupDefenders(blockAssignments, defenders, defenders.begin());
      moves = moves_;

      return moves;
   }

   void printAttackMoves(vector<Move*>moves) {
      for (Move* move: moves) {
         cout << move->moveType << "\n";
         for (int x : move->attackerIds) {
            cout << x << " ";
         }
         cout << "\n";
      }
   }

   void printBlockMoves(vector<Move*>moves) {
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

   vector<Move*> addPlayPermanentMoves(vector<Move*> moves) {
      map<card_name, bool> cardNamesWithMoves;
      for (Card* card: playerWithPriority()->hand()) {
         if (cardNamesWithMoves[card->name()]) {
            continue;
         }
         bool isPlayableLand = card->cardType() == Land && playerWithPriority()->landsPlayedThisTurn() < playerWithPriority()->landsPlayableThisTurn();
         bool isPlayableCreature = card->cardType() == Creature && playerWithPriority()->canAffordManaCost(card->manaCost);
         if (isPlayableLand || isPlayableCreature) {
            Move* move = new Move(isPlayableLand ? select_land : select_card, card->id(), playerWithPriority()->id());
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
         playPassMove_(move);
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

   void playPassMove_(Move* move) {
      passPriority();
      if(turnPlayer()->id() != playerWithPriority()->id()) {
         return;
      }
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

   int turnsToPlay = 100;
   while (!game.isOver()) {
      game.playRandomMove();
      if (game.isOver()) {
         game.printGameStatus();
         cout << "\n~~~~~~GAME OVER~~~~~~\n";
      }
      if (game.turn() >= turnsToPlay) {
         break;
      }
   }
}
