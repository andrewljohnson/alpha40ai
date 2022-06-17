// effects get mapped to EffectDef functions when cards are instantiated for a deck 
struct Effect {             
  effect_name name;          
  int amount;           
  target_type targetType;    
  effect_trigger trigger;       
};       


// a struct, based on player actions, that gets passed into an EffectDef function
struct TargetInfo {             
   int amount;
   target_type targetType;   
   int targetId;        
};


// map all of a Card's Effects into functions of this signature when cards are instantiated for a deck 
typedef void (*EffectDef) (int effectOwner, Effect e, TargetInfo targetInfo);


struct Card {             
  int id;              // assign when card is instantiated for a deck
  card_name name;         
  card_type cardType;     

  bool tapped = false;

  vector<Effect*> effects; 
  vector<EffectDef> activatedEffectDefs; 
};       


void doManaEffect(int effectOwner, Effect e, TargetInfo t) { 
   // add mana to a player's pool
   // do Effect e for Card c based on TargetInfo t 
}


Card* mountain() {
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
}


