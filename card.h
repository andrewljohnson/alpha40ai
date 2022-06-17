#include "effect.h"


struct Card {             
  int id;              // assign when card is instantiated for a deck
  card_name name;         
  card_type cardType;     

  bool tapped = false;

  vector<Effect> effects; 
  vector<EffectDef> activatedEffectDefs; 
};       


void doManaEffect(int effectOwner, Effect e, TargetInfo t) { 
   // add mana to a player's pool
   // do Effect e for Card c based on TargetInfo t 
}


Card mountain() {
   // make a card with one effect
   Card m;
   m.name = Mountain;
   m.cardType = Land;

   Effect mManaEffect;
   mManaEffect.name = mana_red;
   mManaEffect.amount = 1;
   mManaEffect.targetType = self;
   mManaEffect.trigger = count_mana;
   m.effects.push_back(mManaEffect);
   m.activatedEffectDefs.push_back(doManaEffect);
   return m;
}


