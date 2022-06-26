#include <map>
#include <vector>
#include "player.h"
#include "card.h"
#include "move.h"
#include "enums.h"
#include <iostream>
using namespace std;

class Game {

   // the index of the player that currently has priority to play moves
   int indexOfPlayerWithPriority_ = 0;

   // each new card created in a game has a sequential ID
   int lastCardId = 0;

   // the max number of cards a player can have at the end of a turn
   int maxHandSize = 7;

   // the current turn, which consists of a series of steps 
   int turn = 0;

   // the current step in a turn
   game_step gameStep = main_first; 

   // a vector of players in the game
   vector<Player*> players_;


   public:

   void addPlayer (Player* p) {
      players_.push_back(p);
   }   

   void addCardToLibraryForPlayerAtIndex (Card* c, int index) {
      Player* p = players_[index];
      c->id = lastCardId;
      lastCardId++;
      p->addCardToLibrary(c);
      // cout << "Added a " << c.name << " (" << c.cardType << ") to " << p.username() << "'s library.\n"; 
   }   

   void drawCardForPlayerAtIndex (int index) {
      Player* p = players_[index];
      p->drawCard();
   }   

   void makeDecks() {
      int deckSize = 60;
      for (int x=0; x<deckSize; x++) {
         if (x % 3 == 0 && x % 2 == 0) {
            addCardToLibraryForPlayerAtIndex(Card::forest(), 0);
            addCardToLibraryForPlayerAtIndex(Card::forest(), 1);
         } else if (x % 3 == 0) {
            addCardToLibraryForPlayerAtIndex(Card::mountain(), 0);
            addCardToLibraryForPlayerAtIndex(Card::mountain(), 1);
         } else if (x % 2 == 0) {
            addCardToLibraryForPlayerAtIndex(Card::lightning_bolt(), 0);
            addCardToLibraryForPlayerAtIndex(Card::lightning_bolt(), 1);            
         } else {
            addCardToLibraryForPlayerAtIndex(Card::grizzly_bear(), 0);
            addCardToLibraryForPlayerAtIndex(Card::grizzly_bear(), 1);            
         }
      }
   }

   void drawOpeningHands() {
      for (int x=0; x<maxHandSize; x++) {
         drawCardForPlayerAtIndex(0);
         drawCardForPlayerAtIndex(1);
      }
      gameStep = main_first;
   }

   void printGameStatus() {
      for (int x=0; x < players_.size(); x++) {
         Player* p = players_[x];
         cout << p->username() << ": " << p->library().size() << " (lib)";
         cout << ", " << p->creatures().size() << " (creat)";
         cout << ", " << p->lands().size() << " (lands)";
         cout << ", " << p->graveyard().size() << " (grave)";
         cout << ", " << p->hand().size() << " (hand).\n";
      }
      cout << "Valid moves: ";
      for (Move* move: validMoves()) {
         cout << "(" << move->moveType << ", " << move->cardId << ", " << move->playerId << "), ";
      }
      cout << "\n";
   }

   vector<Move*> validMoves() {
      vector<Move*> moves;
      Player* p = players_[indexOfPlayerWithPriority_];
      map<card_name, bool> cardNamesWithMoves;
      if (turnPlayer()->id() == p->id()) {
         // playable land moves
         if (gameStep == main_first) {
            for (int x=0;x<p->hand().size();x++) {
               Card* card = p->hand()[x];
               if (cardNamesWithMoves[card->name]) {
                  continue;
               }
               bool isPlayableLand = card->cardType == Land && p->landsPlayedThisTurn() < p->landsPlayableThisTurn();
               bool isPlayableCreature = card->cardType == Creature && p->canAffordManaCost(card->manaCost);
               if (isPlayableLand || isPlayableCreature) {
                  Move* move = new Move(select_card, card->id, p->id());
                  cardNamesWithMoves[card->name] = true;
                  moves.push_back(move);
               }
            }

         }
      }

      for (Card* card: p->hand()) {
         if (cardNamesWithMoves[card->name]) {
            continue;
         }
         // instants
         if (card->cardType == Instant && p->canAffordAndTarget(card)) {
            cardNamesWithMoves[card->name] = true;
            if (card->effects[0]->targetType == any_player_or_creature) {
               Move* moveSelf = new Move(select_card_with_targets, card->id, p->id());
               moveSelf->targetId = playerWithPriority()->id(); 
               moveSelf->targetType = player; 
               moves.push_back(moveSelf);

               Move* moveOpponent = new Move(select_card_with_targets, card->id, p->id());
               moveOpponent->targetId = playerWithoutPriority()->id(); 
               moveOpponent->targetType = player; 
               moves.push_back(moveOpponent);

               for (Player* player: players_) {
                  for (Card* permanent: player->inPlay()) {
                     if (permanent->cardType != Creature) {
                        continue;
                     }
                     Move* moveCreature = new Move(select_card_with_targets, card->id, p->id());
                     moveCreature->targetId = permanent->id; 
                     moveCreature->targetType = creature; 
                     moves.push_back(moveCreature);
                  }
               }

            }
         }
      }

      // default pass move
      Move* move = new Move(pass, 0, p->id());
      moves.push_back(move);
      return moves;
   }

   Player* playerWithPriority() {
      return players_[indexOfPlayerWithPriority_];
   }

   Player* playerWithoutPriority() {
      if (indexOfPlayerWithPriority_ == 0) return players_[1];
      return players_[0];
   }

   Player* turnPlayer() {
      if( turn % 2 == 0) return players_[0];
      return players_[1];
   }


   void passPriority() {
      if (indexOfPlayerWithPriority_ == 0) {
         indexOfPlayerWithPriority_ = 1;
      } else indexOfPlayerWithPriority_ = 0;      
   }

   void playMove(Move* move) {
      if (move->moveType == pass) {
         passPriority();

         if (gameStep == main_first) {
            if(turnPlayer()->id() == playerWithPriority()->id()) {
               gameStep = end_step;               
            }
         }
         if (gameStep == end_step) {
            if(turnPlayer()->id() == playerWithPriority()->id()) {
               passPriority();
               gameStep = draw_step;                   
               turnPlayer()->resetLandsPlayedThisTurn();
               turnPlayer()->untapPermanents();
               turn += 1;                          
               playerWithPriority()->drawCard();
               if (playerWithPriority()->didDrawFromEmptyLibrary()) {
                  return;
               }
               gameStep = main_first;                   
            }
         }
         return;
      }
      playerWithPriority()->playMove(move, players_);
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
   game.printGameStatus();
   
   int movesToPlay = 20;
   int i = 0;
   while (!game.isOver()) {
      game.playRandomMove();
      if (game.isOver()) {
         game.printGameStatus();
         cout << "GAME OVER";
      } else {
         game.printGameStatus();
      }
      i++;
      if (i >= movesToPlay) {
      break;
      }
   }
}
