#include "card.h"
#include "move.h"
#include "player.h"
#include "enums.h"
#include <iostream>
using namespace std;


// Current Cards
// Mountain
// Lightning Bolt
// Grizzly Bear

// Future Cards
// Forest
// Giant Growth, Earthquake
// Kird Ape, Granite Gargoyle, Llanowar Elf, 

void doManaEffect(Move* m, Card* card, Effect* effect, vector<Player*>players) { 
   // add mana to a player's pool
   // do Effect e for Card c based on TargetInfo t 
}

void doDamageEffect(Move* m, Card* card, Effect* effect, vector<Player*>players) {
   if (m->targetType == player) {
      for (Player* p: players) {
         if (p->id() == m->targetId) {
            cout << card->name << " deals " << effect->amount << ".\n";
            p->decrementLife(effect->amount);
         }
      } 
   } 
   if (m->targetType == creature) {
      for (Player* p: players) {
         for (Card* c: p->inPlay()) {
            if (c->id == m->targetId) {
               cout << "Implement bolt a creature";
            }
         } 
      } 
   }
}

Card* Card::mountain() {
   // make a card with one effect
   Card* m = new Card();
   m->name = Mountain;
   m->cardType = Land;
   Effect* mManaEffect = new Effect();
   mManaEffect->name = mana_red;
   mManaEffect->amount = 1;
   mManaEffect->targetType = self;
   mManaEffect->trigger = count_mana;
   m->effects.push_back(mManaEffect);
   m->activatedEffectDefs.push_back(doManaEffect);
   return m;
};

Card* Card::forest() {
   // make a card with one effect
   Card* m = new Card();
   m->name = Forest;
   m->cardType = Land;
   Effect* mManaEffect = new Effect();
   mManaEffect->name = mana_green;
   mManaEffect->amount = 1;
   mManaEffect->targetType = self;
   mManaEffect->trigger = count_mana;
   m->effects.push_back(mManaEffect);
   m->activatedEffectDefs.push_back(doManaEffect);
   return m;
};

Card* Card::grizzly_bear() {
   Card* bear = new Card();
   bear->name = GrizzlyBear;
   bear->cardType = Creature;
   bear->power = 2;
   bear->toughness = 2;
   map<mana_type, int> manaCost;
   manaCost[green] = 1;
   manaCost[colorless] = 1;
   bear->manaCost = manaCost;
   return bear;
};

Card* Card::lightning_bolt() {
   // make a card with one effect
   Card* bolt = new Card();
   bolt->name = LightningBolt;
   bolt->cardType = Instant;
   map<mana_type, int> manaCost;
   manaCost[red] = 1;
   bolt->manaCost = manaCost;
   Effect* damageEffect = new Effect();
   damageEffect->name = damage;
   damageEffect->amount = 3;
   damageEffect->targetType = any_player_or_creature;
   damageEffect->trigger = cast_this;
   bolt->effects.push_back(damageEffect);
   bolt->castSpellEffectDefs.push_back(doDamageEffect);
   return bolt;
};
