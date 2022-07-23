#include "game.h"
#include "player.h"


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
         game.printGameStatus();
         cout << "\n~~~~~~GAME OVER~~~~~~\n";
      }
      if (game.turn() >= turnsToPlay) {
         break;
      }
   }
}
