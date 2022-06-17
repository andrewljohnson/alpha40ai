#include <iostream>
using namespace std;

enum card_name {
   Mountain
};
// https://stackoverflow.com/questions/3342726/c-print-out-enum-value-as-text
std::ostream& operator<<(std::ostream& out, const card_name value){
    const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
    switch(value){
        PROCESS_VAL(Mountain);     
    }
#undef PROCESS_VAL
    return out << s;
}


enum card_type {
   Land,
};
ostream& operator<<(ostream& out, const card_type value){
    const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
    switch(value){
        PROCESS_VAL(Land);     
    }
#undef PROCESS_VAL
    return out << s;
}


enum effect_name {
   mana_red
};

enum effect_trigger {
   count_mana
};

enum target_type {
   self
};

enum game_step {
   draw_step,
   main_first,
   end_step
};

enum move_type {
   pass,
   select_card
};
ostream& operator<<(ostream& out, const move_type value){
    const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
    switch(value){
        PROCESS_VAL(pass);     
        PROCESS_VAL(select_card);     
    }
#undef PROCESS_VAL
    return out << s;
}


