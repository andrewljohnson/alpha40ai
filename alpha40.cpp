#include "player.h"

// Current Cards
// Mountain

// Future Cards
// Forest
// Lightning Bolt, Giant Growth, Earthquake
// Kird Ape, Granite Gargoyle, Llanowar Elf


class Game {
   // the index of the player that currently has priority
   int indexOfPlayerWithPriority = 0;

   // each new card created in a game has a sequential ID
   int lastCardId = 0;

   // the max number of cards a player can have
   int maxHandSize = 7;

   game_step gameStep = main_first; 

   int turn = 0;

   // a vector of players in the game
   vector< Player > players;

   public:

   void addPlayer (Player p) {
      players.push_back(p);
   }   

   void addCardToLibraryForPlayerAtIndex (Card c, int index) {
      Player& p = players[index];
      c.id = lastCardId;
      lastCardId++;
      p.addCardToLibrary(c);
      // cout << "Added a " << c.name << " (" << c.cardType << ") to " << p.username() << "'s library.\n"; 
   }   

   void drawCardForPlayerAtIndex (int index) {
      Player& p = players[index];
      p.drawCard();
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
         cout << players[x].username() << " has " << players[x].librarySize() << " cards in library";
         cout << ", " << players[x].inPlaySize() << " cards in play";
         cout << " and " << players[x].handSize() << " cards in hand.\n";
      }
      cout << "Valid moves are: ";
      for (Move move: validMoves())
         cout << "(" << move.moveType << ", " << move.cardId << ", " << move.playerId << "), ";
      cout << "\n";


   }

   vector< Move > validMoves() {
      if (turnPlayer().id() != currentPlayer().id()) {
         vector< Move > moves;
         moves.push_back((struct Move){.moveType = pass, .playerId = currentPlayer().id()});         
         return moves;
      }
      Player p = players[indexOfPlayerWithPriority];
      return p.validMoves();

   }

   Player& currentPlayer() {
      return players[indexOfPlayerWithPriority];
   }

   Player& turnPlayer() {
      if( turn % 2 == 0) return players[0];
      return players[1];
   }


   void passPriority() {
      if (indexOfPlayerWithPriority == 0) {
         indexOfPlayerWithPriority = 1;
      } else indexOfPlayerWithPriority = 0;      
   }

   void playMove(Move move) {
      if (move.moveType == pass) {
         passPriority();

         if (gameStep == main_first) {
            if(turnPlayer().id() == currentPlayer().id()) {
               gameStep = end_step;               
            }
         }
         if (gameStep == end_step) {
            if(turnPlayer().id() == currentPlayer().id()) {
               passPriority();
               gameStep = draw_step;                   
               turnPlayer().resetLandsPlayedThisTurn();
               turn += 1;                          
               currentPlayer().drawCard();
               if (currentPlayer().didDrawFromEmptyLibrary()) {
                  return;
               }
               gameStep = main_first;                   
            }
         }
         return;
      }

      currentPlayer().playMove(move);
   }

   void playRandomMove() {
      vector< Move > moves = validMoves();
      // use arc4random() instead if we want to change the seed each run on Mac 
      // playMove(moves[rand() % moves. size()]);
      playMove(moves[0]);
   }

   bool isOver() {
      if (players[0].getLife() <= 0) return true;
      if (players[1].getLife() <= 0) return true;
      if (players[0].didDrawFromEmptyLibrary()) return true;
      if (players[1].didDrawFromEmptyLibrary()) return true;
      return false;
   }
};


int main() {
   Game game;
   game.addPlayer(Player(0, "Spike"));
   game.addPlayer(Player(1, "Lee"));
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