#include "../include/food.h"
#include "../include/snake.h"
void food::onCollision(ICollider* other){}

bool food::intersect(ICollider* other){
    unisnake* snakeother=dynamic_cast<unisnake*>(other); 
    if(this->getPosition() == snakeother->getPosition()) {
        return true; // Collision detected
    }
    // Check intersection with another collider
    return false; // Placeholder return value
}