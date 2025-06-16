#include "RPSGameObject.h"
#include "gameObjectFactory.h"
#include "iconFactory.h"

Icon RPS[4]={
    iconFactory::makeframeicon(1, 1, Color::RED, "R"),
    iconFactory::makeframeicon(1, 1, Color::BLUE, "P"),
    iconFactory::makeframeicon(1, 1, Color::YELLOW, "S"),
    iconFactory::makeframeicon(1, 1, Color::WHITE, "R")
};
void RPSGameObject::update(){
    switch(_type){
        case ROCK:
            if(isplayer) _icon=RPS[3];
            else _icon=RPS[0];
            break;
        case PAPER:
            _icon=RPS[1];
            break;
        case SCISSORS:
            _icon=RPS[2];
            break;
    }
}

bool RPSGameObject::onCollision(ICollider *other){
    RPSGameObject* RPSother=dynamic_cast<RPSGameObject*>(other); 
    if(_type==ROCK && RPSother->getType() == PAPER) {
        setType(PAPER); // Rock loses to Paper
        if(isplayer) return true;
    } else if(_type == PAPER && RPSother->getType() == SCISSORS) {
        setType(SCISSORS);// Paper beats Rock
        if(isplayer) return true;
    } else if(_type == SCISSORS && RPSother->getType() == ROCK) {
        setType(ROCK);// Scissors beat Paper
        if(isplayer) return true;
    }
    return false;
}

bool RPSGameObject::intersect(ICollider *other){
    RPSGameObject* RPSother=dynamic_cast<RPSGameObject*>(other); 
    if(this->getPosition() == RPSother->getPosition()) {
        return true; // Collision detected
    }
    // Check intersection with another collider
    return false; // Placeholder return value
}

RPSType RPSGameObject::getType() const {
    return _type;
}

void RPSGameObject::setType(RPSType type) {
    _type = type;
}

/*void setDirection(Direction dir) {
    _dir = dir;
}*/
