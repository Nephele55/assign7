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
        // Changed foodgameObjectFactory to create a default food icon
        static std::unique_ptr<food> foodgameObjectFactory(const Position& p);
        // Changed snakegameObjectFactory to use the new unisnake constructor signature
        static std::unique_ptr<unisnake> snakegameObjectFactory(const Position& p, int initialLength = 3);
};