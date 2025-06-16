#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <termios.h>   // For terminal control
#include <vector>
#include "view.h"      // View class for rendering
#include "gameObject.h" // Base GameObject class
#include "snake.h"     // Snake class
#include "food.h"      // Food class
#include <memory>      // For std::unique_ptr
#include <random>      // For random number generation

class Controller {
public:
    Controller(View&);
    void run();

private:
    bool _gameOver = false; // Game over flag
    int _frameCounter = 0; // New: Counter to control snake movement speed

    void handleInput(int);
    void update();
    void renderGame(); // Handles rendering all game objects
    void generateFood(); // Generates food at a random valid position

    // Model objects for the game
    std::unique_ptr<unisnake> _snake;
    std::unique_ptr<food> _food;

    View& _view; // Reference to the View object
};

// Declare these variables as 'extern' in the header.
// This tells the compiler that these variables are defined elsewhere (in controller.cpp).
extern struct termios old_termios, new_termios;

// Function declarations for terminal control
void reset_terminal();
void configure_terminal();
int read_input();

#endif