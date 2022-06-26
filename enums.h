#ifndef ENUMS_H
#define ENUMS_H

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
   end_step
};

enum move_type : int  {
   pass,
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

#endif