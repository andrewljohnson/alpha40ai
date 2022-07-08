#ifndef MOVE_H
#define MOVE_H

#include <vector>
using namespace std;

enum move_type :  int;
enum target_type :  int;

class Move {
   public:
   int playerId;
   int cardId;
   int targetId;
   map<int, vector<int>> blocks;
   move_type moveType;
   target_type targetType;
   vector<int> attackerIds;

   Move(move_type mtype, int cid, int pid) {
      cardId = cid;
      moveType = mtype;
      playerId = pid;
   }
};


#endif


