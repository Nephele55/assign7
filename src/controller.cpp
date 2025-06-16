#include <iostream>
#include <ctime>
#include <thread>
#include <unistd.h>
#include <termios.h>
#include <chrono>      // For std::chrono::milliseconds
#include <random>      // For std::mt19937 and std::uniform_int_distribution

#include "environment.h"       // Game window dimensions
#include "controller.h"        // Controller header
#include "gameObjectFactory.h" // Factory for game objects
#include "snake.h"             // Snake class
#include "food.h"              // Food class


// --- Terminal Control Functions ---

// Define old_termios and new_termios here.
// These are the actual definitions (memory allocation) for the variables
// that were declared as 'extern' in controller.h.
struct termios old_termios;
struct termios new_termios;


void reset_terminal() {
    printf("\e[m");     // reset color changes
    printf("\e[?25h");   // show cursor
    fflush(stdout);      // Flush output buffer
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios); // Restore original terminal settings
}

void configure_terminal() {
    tcgetattr(STDIN_FILENO, &old_termios); // Get current terminal settings
	new_termios = old_termios; // Save it to be able to reset on exit

    new_termios.c_lflag &= ~(ICANON | ECHO); // Turn off canonical mode (raw input) and echoing characters
    new_termios.c_cc[VMIN] = 0; // Set minimum number of characters for non-canonical read (0 means read returns immediately)
    new_termios.c_cc[VTIME] = 0; // Set timeout for non-canonical read (0 means no timeout)

    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios); // Apply new terminal settings

    printf("\e[?25l"); // Hide cursor
    std::atexit(reset_terminal); // Register reset_terminal to be called on program exit
}

int read_input() {
    fflush(stdout); // Flush stdout before reading input
   	char buf[4096]; // Maximum input buffer
	int n = read(STDIN_FILENO, buf, sizeof(buf)); // Attempt to read input
    return n > 0 ? buf[n - 1] : -1; // Return the last character read, or -1 if no input
}

// --- Random Number Generation Setup ---

std::random_device rd;
std::mt19937 gen(rd());
// Distribution for X coordinates within game window width
std::uniform_int_distribution<> distrib_x(0, GAME_WINDOW_WIDTH - 1);
// Distribution for Y coordinates within game window height
std::uniform_int_distribution<> distrib_y(0, GAME_WINDOW_HEIGHT - 1);


// --- Controller Class Implementation ---

Controller::Controller(View& view) : _view(view), _gameOver(false), _frameCounter(0) { // Initialize _frameCounter
    // Game object initialization
    // Create snake in the center of the game window with an initial length of 3
    _snake = GameObjectFactory::snakegameObjectFactory({GAME_WINDOW_WIDTH / 2, GAME_WINDOW_HEIGHT / 2}, 2);
    generateFood(); // Generate initial food item
}

void Controller::run() {
    // Initial setup
    std::cin.tie(0); // Untie cin from cout for faster I/O
    std::ios::sync_with_stdio(0); // Untie C++ streams from C stdio for faster I/O
    configure_terminal(); // Configure terminal for non-blocking input and hide cursor

    int input = -1;
    auto start_time = std::chrono::high_resolution_clock::now(); // For frame rate calculation

    // Main game loop
    while (!_gameOver) {
        start_time = std::chrono::high_resolution_clock::now(); // Record start time for current frame

        input = read_input(); // Read user input

        // Check for ESC key to exit program
        if (input == 27) { // ASCII code for ESC key
            _gameOver = true;
            break;
        }

        handleInput(input); // Process user input
        update();           // Update game state (snake movement logic here)
        renderGame();       // Render the current game frame

        auto end_time = std::chrono::high_resolution_clock::now(); // Record end time for current frame
        std::chrono::duration<double> time_taken = end_time - start_time; // Calculate time elapsed for the frame

        // Frame rate normalization: Pause if frame rendered too quickly
        // We target a specific SPF (seconds per frame) from environment.h
        if (time_taken.count() < SPF) {
            int frameDelay = static_cast<int>((SPF - time_taken.count())*30); // Calculate delay in milliseconds
            if (frameDelay > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(frameDelay)); // Sleep to maintain desired frame rate
            }
        }
    }

    // Game over message and cleanup
    reset_terminal(); // Ensure terminal settings are restored
    // Calculate final score based on snake length (e.g., each extra segment is 100 points)
    std::cout << "\nGame Over! Your score: " << (static_cast<int>(_snake->getBodyPositions().size()) - 3) * 100 << std::endl;
}

void Controller::handleInput(int keyInput) {
    // If there is no input, do nothing.
    if (keyInput == -1) return;

    Direction currentSnakeDir = _snake->getDirection();

    // Update snake direction based on key input, preventing 180-degree turns
    switch (keyInput) {
        case 'W': case 'w':
            if (currentSnakeDir != DOWN) _snake->setDirection(UP);
            break;
        case 'S': case 's':
            if (currentSnakeDir != UP) _snake->setDirection(DOWN);
            break;
        case 'A': case 'a':
            if (currentSnakeDir != RIGHT) _snake->setDirection(LEFT);
            break;
        case 'D': case 'd':
            if (currentSnakeDir != LEFT) _snake->setDirection(RIGHT);
            break;
    }
}

void Controller::update() {
    // Increment frame counter
    _frameCounter++;

    // Snake moves only every 5 frames
    if (_frameCounter % 5 == 0) { // Snake moves every 5 frames
        // 1. Move the snake
        _snake->move(_snake->getDirection(), false); // Move snake without growing

        // 2. Check for collisions and update game state
        // Check if snake head collides with food
        if (_snake->getHeadPosition() == _food->getPosition()) {
            _snake->move(_snake->getDirection(), true); // Snake grows
            generateFood(); // Generate new food
        }

        // Check if snake head collides with walls
        Position headPos = _snake->getHeadPosition();
        if (headPos.x() < 0 || headPos.x() >= GAME_WINDOW_WIDTH ||
            headPos.y() < 0 || headPos.y() >= GAME_WINDOW_HEIGHT) {
            _gameOver = true; // Game over: snake hit a wall
        }

        // Check if snake head collides with its own body
        if (_snake->checkSelfCollision()) {
            _gameOver = true; // Game over: snake hit itself
        }
    }
    // No else: renderGame() is always called, but update() logic (snake movement)
    // only runs when _frameCounter is a multiple of 5.
}

void Controller::renderGame() {
    _view.resetLatest(); // Clear the view buffer (fill with spaces)

    // Render food first
    _view.updateGameObject(_food.get()); // Pass the raw pointer to food

    // Render snake segments
    const auto& bodyPositions = _snake->getBodyPositions();
    
    // Iterate through all body positions to update them in the view
    // Note: This creates temporary GameObject instances for rendering.
    // In a more optimized or robust system, View might have a method to take a snake directly.
    for (size_t i = 1; i < bodyPositions.size(); ++i) { // Start from 1 for body segments (exclude head)
        // Create a temporary GameObject for the current body segment
        // This is necessary because View::updateGameObject expects a GameObject*.
        GameObject temp_segment_obj(bodyPositions[i], _snake->getBodySegmentIcon());
        _view.updateGameObject(&temp_segment_obj); // Pass the address of the temporary object
    }
    // Render snake head last to ensure it's on top of a body segment if they overlap temporarily
    GameObject temp_head_obj(_snake->getHeadPosition(), _snake->getHeadIcon());
    _view.updateGameObject(&temp_head_obj); // Pass the address of the temporary head object


    _view.render(); // Finally, render the prepared frame to the terminal
}

void Controller::generateFood() {
    Position newFoodPos;
    bool validPos;
    do {
        // Generate random coordinates within game window boundaries
        newFoodPos = {distrib_x(gen), distrib_y(gen)};

        validPos = true;
        // Ensure new food does not spawn on the snake's body
        for (const auto& segmentPos : _snake->getBodyPositions()) {
            if (newFoodPos == segmentPos) {
                validPos = false;
                break;
            }
        }
    } while (!validPos); // Loop until a valid (non-snake-overlapping) position is found

    // Create the new food object using the GameObjectFactory
    _food = GameObjectFactory::foodgameObjectFactory(newFoodPos);
}