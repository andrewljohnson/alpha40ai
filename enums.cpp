#include "enums.h"

// https://stackoverflow.com/questions/3342726/c-print-out-enum-value-as-text
ostream& operator<<(ostream& out, const card_name value){
    const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
    switch(value){
        PROCESS_VAL(Forest);     
        PROCESS_VAL(GrizzlyBear);     
        PROCESS_VAL(Mountain);     
        PROCESS_VAL(LightningBolt);     
    }
#undef PROCESS_VAL
    return out << s;
}

ostream& operator<<(ostream& out, const mana_type value){
    const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
    switch(value){
        PROCESS_VAL(red);     
        PROCESS_VAL(green);     
        PROCESS_VAL(blue);     
        PROCESS_VAL(black);     
        PROCESS_VAL(white);     
        PROCESS_VAL(colorless);     
    }
#undef PROCESS_VAL
    return out << s;
}

ostream& operator<<(ostream& out, const card_type value){
    const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
    switch(value){
        PROCESS_VAL(Creature);     
        PROCESS_VAL(Instant);     
        PROCESS_VAL(Land);     
    }
#undef PROCESS_VAL
    return out << s;
}

ostream& operator<<(ostream& out, const move_type value){
    const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
    switch(value){
        PROCESS_VAL(pass);     
        PROCESS_VAL(select_attackers);     
        PROCESS_VAL(select_card);     
        PROCESS_VAL(select_card_with_targets);     
    }
#undef PROCESS_VAL
    return out << s;
}
