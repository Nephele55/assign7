// TODO implementation
#include "gameObjectFactory.h"
#include "iconFactory.h" // Make sure iconFactory is included for makeframeicon

std::unique_ptr<food> GameObjectFactory::foodgameObjectFactory(const Position& p){
    // Create a default food icon, e.g., yellow '*'
    Icon foodIcon = iconFactory::makeframeicon(1, 1, Color::GREEN, "F");
    return std::make_unique<food>(p, foodIcon);
}

std::unique_ptr<unisnake> GameObjectFactory::snakegameObjectFactory(const Position& p, int initialLength){
    // The unisnake constructor now manages its own internal icons.
    return std::make_unique<unisnake>(p, initialLength);
}