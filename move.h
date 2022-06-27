#ifndef MOVE_H
#define MOVE_H

#include <vector>
using namespace std;

enum move_type :  int;
enum target_type :  int;

class Move {
   public:
   move_type moveType;
   int playerId;
   int cardId;
   int targetId;
   target_type targetType;
   vector<int> attackerIds;

   Move(move_type mtype, int cid, int pid) {
      moveType = mtype;
      cardId = cid;
      playerId = pid;
   }

};


#endif


