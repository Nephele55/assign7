#ifndef SNAKE_H
#define SNAKE_H
#include "gameObject.h"
#include "collider.h"
class unisnake: public GameObject, public ICollider{
    unisnake(const Position& p, const Icon& ic): GameObject(p, iconFactory::makeframeicon(1, 1, Color::WHITE, "S")){}
    void onCollision(ICollider*);
    bool intersect(ICollider*);
};
#endif
