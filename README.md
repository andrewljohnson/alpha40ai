# About

An AI to play Magic: The Gathering, focused on the Alpha 40 format.

# Play a Game

     c++ alpha40.cpp card.cpp player.cpp enums.cpp -std=c++11 -o alpha40 && ./alpha40 

# Todo
 * implement all moves as do the move without passing
 * implement add*Moves methods using ref for moves

later
 * test respond to yours/theirs lightning bolts
 * more cards
 * allow choosing discard EOT more than 7 cards instead of random
 * mulligan
 * dedupe identical attack moves? (denser game tree)
 * dedupe identical defense moves? (denser game tree)


