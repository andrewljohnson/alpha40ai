#ifndef CARD_H
#define CARD_H

#include <map>
#include <vector>
using namespace std;

#include "enums.h"

class Card;
class Move;
class Player;


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
typedef void (*EffectDef) (Move* m, Card* card, Effect* effect, vector<Player*>players); 


class Card {             
  int id_;              // assign when card is instantiated for a deck
  card_name name_;         
  card_type cardType_;     
  int power_;
  int toughness_;
  int damage_;
  
  public:
  Card(card_name name, card_type type);
  card_name name(); 
  card_type cardType(); 
  void takeDamage(Player* owner, int newDamage);
  int power();
  int toughness();
  int damage();
  int id(); 
  void setId(int newId);
  void setPower(int newPower);
  int turnPlayed;
  map<mana_type, int> manaCost;

  bool tapped = false;

  vector<Effect*> effects; 
  vector<EffectDef> activatedEffectDefs; 
  vector<EffectDef> castSpellEffectDefs; 

  static Card* grizzly_bear();
  static Card* lightning_bolt();
  static Card* forest();
  static Card* mountain();
};       


#endif