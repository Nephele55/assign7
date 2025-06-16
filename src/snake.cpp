#include "snake.h"
#include "food.h"           // Needed for collision detection with food
#include "../include/environment.h"     // Used for game window boundaries (GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT)
#include <iostream>                     // For debugging purposes

// Constructor implementation for the unisnake class
unisnake::unisnake(const Position& headPos, int initialLength)
    : GameObject(headPos, iconFactory::makeframeicon(1, 1, Color::CYAN, "S")), // Initialize GameObject's _pos with head position
      _currentDir(RIGHT) // Set initial direction to RIGHT
{
    // Initialize the snake head
    _body.push_back(headPos);
    _headIcon = iconFactory::makeframeicon(1, 1, Color::CYAN, "S"); // Icon for the snake head
    _segmentIcon = iconFactory::makeframeicon(1, 1, Color::WHITE, "S"); // Icon for snake body segments

    // Add initial body segments based on initialLength
    for (int i = 1; i < initialLength; ++i) {
        _body.push_back({headPos.x() - i, headPos.y()}); // Simple initial body extending to the left
    }
}

// Moves the snake in the specified direction.
void unisnake::move(Direction dir, bool grow) {
    // Prevent 180-degree turns
    if ((dir == UP && _currentDir == DOWN) ||
        (dir == DOWN && _currentDir == UP) ||
        (dir == LEFT && _currentDir == RIGHT) ||
        (dir == RIGHT && _currentDir == LEFT)) {
        // If attempting a 180-degree turn, maintain the current direction
        dir = _currentDir;
    }
    _currentDir = dir; // Update the snake's current direction

    Position newHeadPos = _body[0]; // Get the current head position

    // Calculate the new head position based on the current direction
    switch (_currentDir) {
        case UP:    newHeadPos.y()--; break;
        case DOWN:  newHeadPos.y()++; break;
        case LEFT:  newHeadPos.x()--; break;
        case RIGHT: newHeadPos.x()++; break;
    }

    // Insert the new head position at the front of the body vector
    _body.insert(_body.begin(), newHeadPos);

    // If the snake is not growing, remove the tail segment
    if (!grow) {
        _body.pop_back();
    }

    // Update the GameObject's _pos member to the new head position.
    // This might be useful if the View primarily uses GameObject's position.
    // Note: The View will likely need to iterate through _body to render the whole snake.
    _pos = _body[0]; //
}

// Returns the position of the snake's head.
Position unisnake::getHeadPosition() const {
    return _body[0];
}

// Returns a constant reference to the vector of all snake body positions.
const std::vector<Position>& unisnake::getBodyPositions() const {
    return _body;
}

// Returns the icon for the snake's head.
Icon unisnake::getHeadIcon() const {
    return _headIcon;
}

// Returns the icon for the snake's body segments.
Icon unisnake::getBodySegmentIcon() const {
    return _segmentIcon;
}

// Sets the current movement direction of the snake.
void unisnake::setDirection(Direction dir) {
    _currentDir = dir;
}

// Gets the current movement direction of the snake.
Direction unisnake::getDirection() const {
    return _currentDir;
}

// Checks for self-collision (snake head hitting its own body).
bool unisnake::checkSelfCollision() const {
    // If the snake is too short, it cannot collide with itself in a game-ending way.
    // Assuming the head + next 3 segments are not considered self-collision for game over.
    if (_body.size() <= 4) {
        return false;
    }
    // Get the head's position
    Position head = _body[0];
    // Check if the head collides with any other part of the body, starting from a safe distance (e.g., 4th segment).
    for (size_t i = 4; i < _body.size(); ++i) {
        if (head == _body[i]) {
            return true; // Self-collision detected
        }
    }
    return false;
}

// Handles collision with another ICollider object.
// In the Snake game, the Controller typically manages the consequences of collisions (e.g., food eaten, game over).
void unisnake::onCollision(ICollider* other){
    // This method can remain largely empty, as the Controller will coordinate
    // the game logic based on collision detection.
    // For example, if 'other' is food, the Controller will detect it and grow the snake.
    // If it's a self-collision or wall collision, the Controller will end the game.
}

// Checks for intersection with another ICollider object.
bool unisnake::intersect(ICollider* other){
    // Check if the other object is a food object
    food* foodother = dynamic_cast<food*>(other); //
    if (foodother != nullptr) {
        // Collision occurs if the snake's head position matches the food's position.
        return _body[0] == foodother->getPosition();
    }

    // This method could also be used to check for collision with other game objects,
    // like other snakes in a multiplayer scenario, or obstacles.
    // For self-collision, 'checkSelfCollision()' is more specific.

    return false; // No intersection with other collider types handled here by default
}