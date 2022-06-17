#include "player.h"

// Current Cards
// Mountain

// Future Cards
// Forest
// Lightning Bolt, Giant Growth, Earthquake
// Kird Ape, Granite Gargoyle, Llanowar Elf


class Game {

   // the index of the player that currently has priority to play moves
   int indexOfPlayerWithPriority = 0;

   // each new card created in a game has a sequential ID
   int lastCardId = 0;

   // the max number of cards a player can have at the end of a turn
   int maxHandSize = 7;

   // the current turn, which consists of a series of steps 
   int turn = 0;

   // the current step in a turn
   game_step gameStep = main_first; 

   // a vector of players in the game
   vector<Player*> players;



   public:

   void addPlayer (Player* p) {
      players.push_back(p);
   }   

   void addCardToLibraryForPlayerAtIndex (Card* c, int index) {
      Player* p = players[index];
      c->id = lastCardId;
      lastCardId++;
      p->addCardToLibrary(c);
      // cout << "Added a " << c.name << " (" << c.cardType << ") to " << p.username() << "'s library.\n"; 
   }   

   void drawCardForPlayerAtIndex (int index) {
      Player* p = players[index];
      p->drawCard();
   }   

   void makeDecks() {
      int deckSize = 60;
      for (int x=0; x<deckSize; x++) {
         addCardToLibraryForPlayerAtIndex(mountain(), 0);
         addCardToLibraryForPlayerAtIndex(mountain(), 1);
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
      for (int x=0; x < players.size(); x++) {
         Player* p = players[x];
         cout << p->username() << " has " << p->librarySize() << " cards in library";
         cout << ", " << p->inPlaySize() << " cards in play";
         cout << " and " << p->handSize() << " cards in hand.\n";
      }
      cout << "Valid moves are: ";
      for (Move* move: validMoves())
         cout << "(" << move->moveType << ", " << move->cardId << ", " << move->playerId << "), ";
      cout << "\n";


   }

   vector< Move* > validMoves() {
      if (turnPlayer()->id() != playerWithPriority()->id()) {
         vector< Move* > moves;
         Move* move = new Move();
         move->moveType = pass;
         move->playerId = playerWithPriority()->id();
         moves.push_back(move);
         return moves;
      }
      Player* p = players[indexOfPlayerWithPriority];
      return p->validMoves();

   }

   Player* playerWithPriority() {
      return players[indexOfPlayerWithPriority];
   }

   Player* turnPlayer() {
      if( turn % 2 == 0) return players[0];
      return players[1];
   }


   void passPriority() {
      if (indexOfPlayerWithPriority == 0) {
         indexOfPlayerWithPriority = 1;
      } else indexOfPlayerWithPriority = 0;      
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

      playerWithPriority()->playMove(move);
   }

   void playRandomMove() {
      vector< Move* > moves = validMoves();
      // use arc4random() instead if we want to change the seed each run on Mac 
      // playMove(moves[rand() % moves. size()]);
      playMove(moves[0]);
   }

   bool isOver() {
      if (players[0]->getLife() <= 0) return true;
      if (players[1]->getLife() <= 0) return true;
      if (players[0]->didDrawFromEmptyLibrary()) return true;
      if (players[1]->didDrawFromEmptyLibrary()) return true;
      return false;
   }
};


int main() {
   Game game;
   game.addPlayer(new Player(0, "Spike"));
   game.addPlayer(new Player(1, "Lee"));
   game.makeDecks();
   game.drawOpeningHands();
   game.printGameStatus();
   
   int movesToPlay = 9;
   int i = 0;
   while (!game.isOver()) {
      game.playRandomMove();
      if (game.isOver()) {
         cout << "GAME OVER";
      } else {
         game.printGameStatus();
      }
      // i++;
      // if (i >= movesToPlay) {
      //    break;
      // }
   }


}