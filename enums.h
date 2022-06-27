#ifndef ENUMS_H
#define ENUMS_H

#include <iostream>
using namespace std;

enum card_name : int  {
   Forest,
   GrizzlyBear,
   Mountain,
   LightningBolt
};

enum card_type : int {
   Creature,
   Instant,
   Land
};

enum effect_name : int  {
   mana_green,
   mana_red,
   damage
};

enum effect_trigger : int  {
   count_mana,
   cast_this
};

enum target_type : int  {
   any_player_or_creature,
   self,
   creature,
   player,
};

enum game_step : int  {
   draw_step,
   main_first,
   attack_step,
   end_step
};

enum move_type : int  {
   pass,
   select_attackers,
   select_card,
   select_card_with_targets
};

enum mana_type : int  {
   colorless,
   red,
   green,
   blue,
   white,
   black,
};

ostream& operator<<(ostream& out, const card_name value);
ostream& operator<<(ostream& out, const mana_type value);
ostream& operator<<(ostream& out, const card_type value);
ostream& operator<<(ostream& out, const move_type value);

#endif