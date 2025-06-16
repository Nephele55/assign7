// TODO 
// Finish the declaration of GameObjectFactory that
// 1. offers some methods to create a GameObject.
// 2. encapsulates the constructor from the caller. 
#include "iconFactory.h"
#include "gameObject.h"
#include "food.h"
#include "snake.h"
#include <memory>
class GameObjectFactory{
    public:
        static std::unique_ptr<food> foodgameObjectFactory(const Position& p, const Icon& icon);
        static std::unique_ptr<unisnake> snakegameObjectFactory(const Position& p, const Icon& icon);
};