// TODO implementation
#include "gameObjectFactory.h"

std::unique_ptr<food> GameObjectFactory::foodgameObjectFactory(const Position& p,const Icon& icon){
    return std::make_unique<food>(p, icon);
}
std::unique_ptr<unisnake> GameObjectFactory::snakegameObjectFactory(const Position& p,const Icon& icon){
    return std::make_unique<unisnake>(p, icon);
}