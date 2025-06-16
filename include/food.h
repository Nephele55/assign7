#ifndef FOOD_H
#define FOOD_H
#include "gameObject.h"
#include "unit.h"
#include "icon.h"
#include "collider.h"
#include "iconFactory.h"
class food: public GameObject, public ICollider{
    public:
        food(const Position& p, const Icon ic): GameObject(p, ic){}
        void onCollision(ICollider*) override;
        bool intersect(ICollider*) override;
};
#endif