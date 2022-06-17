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

