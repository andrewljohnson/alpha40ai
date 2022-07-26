#include <assert.h>

#include "game.h"
#include "player.h"

void testStackOfTwoInstants() {
    Game game;
    game.addPlayer(new Player(0, "Spk"));
    game.addPlayer(new Player(1, "Lee"));

    vector<card_name> p1Deck = {Mountain, Mountain, Mountain, Mountain, Mountain, Mountain, LightningBolt};
    vector<card_name> p2Deck = {Mountain, Mountain, Mountain, Mountain, Mountain, Mountain, Mountain, LightningBolt};

    game.setDecks(p1Deck, p2Deck);
    game.drawOpeningHands();

    // play a mountain
    game.playRandomMove();
    // pass to next player's turn
    while (game.turn() == 0) {
        game.playMove(game.validMoves().back());
    }
    // pass to first main phase
    while (game.gameStep() != main_first) {
        game.playMove(game.validMoves().back());
    }
    // play a mountain
    game.playRandomMove();
    // play a lightning bolt
    game.playRandomMove();
    // pass priority
    game.playRandomMove();
    // p2 should now be able to respond with lightning bolt or pass
    assert(game.validMoves().size() == 3);
    game.playRandomMove();
    // p1 can only pass, since she has no responses
    assert(game.validMoves().size() == 1);
    // p0 and p1 passing should resolve a lightning bolt
    game.playRandomMove();
    game.playRandomMove();
    // one lightning bolt left on stack
    assert(game.stackMove().size() == 1);
    // the player with priority should be
    assert(game.stackMove().size() == 1);
    game.playRandomMove();
    game.playRandomMove();
    // both players got bolted
    assert(game.players()[0]->life() == 17);
    assert(game.players()[1]->life() == 17);
    // game step is still main first
    assert(game.gameStep() == main_first);
}

void testGameEnds() {
    Game game;
    game.addPlayer(new Player(0, "Spk"));
    game.addPlayer(new Player(1, "Lee"));
    game.makeDecks();
    game.drawOpeningHands();

    int turnsToPlay = 999;
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
    assert(game.isOver() == true);
}


int main() {
    testGameEnds();
    testStackOfTwoInstants();
}

