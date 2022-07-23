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

Card::Card(card_name name, card_type type) {
   name_ = name;
   cardType_ = type;
}

int Card::id() {
   return id_;
}

int Card::power() {
   return power_;
}

int Card::toughness() {
   return toughness_;
}

card_name Card::name() {
   return name_;
}

card_type Card::cardType() {
   return cardType_;
}

int Card::damage() {
   return damage_;
}

void Card::takeDamage(Player* owner, int newDamage) {
   damage_ += newDamage;
}

void Card::setId(int newId) {
   id_ = newId;
}

void doManaEffect(Move* m, Card* card, Effect* effect, vector<Player*>players) { 
   // add mana to a player's pool
}

void doDamageEffect(Move* m, Card* card, Effect* effect, vector<Player*>players) {
   if (m->targetType == player) {
      Player *caster;
      for (Player* p: players) {
         if (p->id() == m->playerId) {
            caster = p;
         }
      }
      for (Player* p: players) {
         if (p->id() == m->targetId) {
            cout << caster->username() <<" casts "<< card->name() << " on " << p->username() << " and deals " << effect->amount << ".\n";
            p->decrementLife(effect->amount);
         }
      } 
   } 
   if (m->targetType == creature) {
      for (Player* p: players) {
         for (Card* c: p->inPlay()) {
            if (c->id() == m->targetId) {
               c->takeDamage(p, effect->amount);
            }
         } 
      }

      Card* damagedCard;
      Player* owner;
      for (Player* p: players) {
         for (Card* c: p->inPlay()) {
            if (c->id() == m->targetId) {
               damagedCard = c;
            }
         }
      }
      if (damagedCard->damage() >= damagedCard->toughness()) {
         owner->bury(damagedCard);
      }
   }
}

Card* Card::mountain() {
   // make a card with one effect
   Card* m = new Card(Mountain, Land);
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
   Card* m = new Card(Forest, Land);
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
   Card* bear = new Card(GrizzlyBear, Creature);
   bear->setPower(2);
   bear->toughness_ = 2;
   map<mana_type, int> manaCost;
   manaCost[green] = 1;
   manaCost[colorless] = 1;
   bear->manaCost = manaCost;
   return bear;
};

void Card::setPower(int newPower) {
   power_ = newPower;
}

Card* Card::lightning_bolt() {
   // make a card with one effect
   Card* bolt = new Card(LightningBolt, Instant);
   map<mana_type, int> manaCost;
   manaCost[red] = 1;
   bolt->manaCost = manaCost;
   Effect* damageEffect = new Effect();
   damageEffect->name = effect_name::damage;
   damageEffect->amount = 3;
   damageEffect->targetType = any_player_or_creature;
   damageEffect->trigger = cast_this;
   bolt->effects.push_back(damageEffect);
   bolt->castSpellEffectDefs.push_back(doDamageEffect);
   return bolt;
};
