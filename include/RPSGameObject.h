#ifndef RPS_H
#define RPS_H

#include "collider.h"
#include "gameObject.h"
#include "unit.h"
#include "icon.h"

enum RPSType { ROCK, PAPER, SCISSORS };
extern Icon RPS[4];

class RPSGameObject : public GameObject, public ICollider {

public:
  void update() override;
  bool onCollision(ICollider *other) override;
  bool intersect(ICollider *other) override;
  RPSGameObject(const Position& pos, const Icon& icon, int k): GameObject(pos, icon){
    _type = static_cast<RPSType>(k);
  }
  RPSType getType() const;
  void setType(RPSType type);
  //void setDirection(Direction);
  void setplayer(bool is){
    isplayer=is;
  };
private:
  RPSType _type;
  bool isplayer=0;
  //Direction _dir;
};
#endif
