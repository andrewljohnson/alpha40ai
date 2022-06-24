#include "card.h"
#include "move.h"
#include "player.h"
#include "enums.h"
#include <iostream>
using namespace std;

void Card::doManaEffect(Move* m, Effect* effect, vector<Player*>players) { 
   // add mana to a player's pool
   // do Effect e for Card c based on TargetInfo t 
}

void Card::doDamageEffect(Move* m, Effect* effect, vector<Player*>players) {
   if (m->targetType == player) {
      for (Player* p: players) {
         if (p->id() == m->playerId) {
            p->decrementLife(effect->amount);
         }
      }
      
   } 
   if (m->targetType == creature) {

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
   m->activatedEffectDefs.push_back(Card::doManaEffect);
   return m;
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
   bolt->castSpellEffectDefs.push_back(Card::doDamageEffect);
   return bolt;
};