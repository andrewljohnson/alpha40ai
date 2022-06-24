#ifndef CARD_H
#define CARD_H

class Move;
class Player;
#include <map>
#include <vector>
using namespace std;
enum card_name :  int;
enum card_type :  int;
enum mana_type :  int;
enum effect_name :  int;
enum effect_trigger :  int;
enum target_type :  int;

// a struct, based on player actions, that gets passed into an EffectDef function
struct TargetInfo {             
   int amount;
   target_type targetType;   
   int targetId;        
};

// effects get mapped to EffectDef functions when cards are instantiated for a deck 
struct Effect {             
  effect_name name;          
  int amount;           
  target_type targetType;    
  effect_trigger trigger;       
};       

// map all of a Card's Effects into functions of this signature when cards are instantiated for a deck 
typedef void (*EffectDef) (Move* m, Effect* effect, vector<Player*>players); 


class Card {             
  public:
  int id;              // assign when card is instantiated for a deck
  card_name name;         
  card_type cardType;     
  map<mana_type, int> manaCost;

  bool tapped = false;

  vector<Effect*> effects; 
  vector<EffectDef> activatedEffectDefs; 
  vector<EffectDef> castSpellEffectDefs; 

   static Card* lightning_bolt();
   static Card* mountain();
   static void doManaEffect(Move* m, Effect* effect, vector<Player*>players);
   static void doDamageEffect(Move* m, Effect* effect, vector<Player*>players);
};       


#endif