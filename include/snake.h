#ifndef SNAKE_H
#define SNAKE_H

#include "gameObject.h" // Base class for game objects
#include "collider.h"   // Interface for collidable objects
#include "unit.h"       // Contains Position (Vec2) and Color definitions
#include <vector>       // Used to store the snake's body segments
#include "iconFactory.h" // Used to create icons for snake head and segments

// Define the directions for snake movement
enum Direction { UP, DOWN, LEFT, RIGHT };

class unisnake : public GameObject, public ICollider {
public:
    /**
     * @brief Constructor for the unisnake (snake) object.
     * @param headPos The initial position of the snake's head.
     * @param initialLength The initial number of segments for the snake.
     */
    unisnake(const Position& headPos, int initialLength = 2); // Default length is 2

    // Override the collision handling methods from ICollider
    void onCollision(ICollider* other) override;
    bool intersect(ICollider* other) override;

    /**
     * @brief Moves the snake in the current direction.
     * @param dir The direction to move.
     * @param grow A boolean indicating whether the snake should grow after this move.
     */
    void move(Direction dir, bool grow = false);

    /**
     * @brief Gets the current position of the snake's head.
     * @return The Position of the snake's head.
     */
    Position getHeadPosition() const;

    /**
     * @brief Gets a constant reference to the vector containing all snake body segment positions.
     * This is useful for the View to render the entire snake.
     * @return A const reference to the vector of Position objects.
     */
    const std::vector<Position>& getBodyPositions() const;

    /**
     * @brief Gets the icon for the snake's head.
     * @return The Icon for the snake's head.
     */
    Icon getHeadIcon() const;

    /**
     * @brief Gets the icon for the snake's body segments.
     * @return The Icon for the snake's body segments.
     */
    Icon getBodySegmentIcon() const;

    /**
     * @brief Sets the current movement direction of the snake.
     * @param dir The new Direction for the snake.
     */
    void setDirection(Direction dir);

    /**
     * @brief Gets the current movement direction of the snake.
     * @return The current Direction of the snake.
     */
    Direction getDirection() const;

    /**
     * @brief Checks if the snake's head has collided with any part of its body.
     * @return True if self-collision occurred, false otherwise.
     */
    bool checkSelfCollision() const;

private:
    std::vector<Position> _body;       // Stores the positions of all snake segments. _body[0] is the head.
    Direction _currentDir;             // The current movement direction of the snake.
    Icon _headIcon;                    // The icon representing the snake's head.
    Icon _segmentIcon;                 // The icon representing a snake body segment.
};

#endif