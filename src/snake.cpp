#include "../include/food.h"
#include "../include/snake.h"
void unisnake::onCollision(ICollider* other){}

bool unisnake::intersect(ICollider* other){
    food* foodother=dynamic_cast<food*>(other); 
    if(this->getPosition() == foodother->getPosition()) {
        return true; // Collision detected
    }
    // Check intersection with another collider
    return false; // Placeholder return value
}