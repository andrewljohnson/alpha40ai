# About

An AI to play Magic: The Gathering, focused on the Alpha 40 format.

# Play a Game

     c++ alpha40ai.cpp card.cpp game.cpp player.cpp enums.cpp -std=c++11 -o alpha40ai && ./alpha40ai 

# Todo
 * implement add*Moves methods using ref for moves
 * implement instants before attackers
 * implement instants before blockers

later
 * test respond to yours/theirs lightning bolts
 * more cards
 * allow choosing discard EOT more than 7 cards instead of random
 * mulligan
 * dedupe identical attack moves? (denser game tree)
 * dedupe identical defense moves? (denser game tree)
 * make stack tuples instead of two stacks for stackMove and stackCard? 


