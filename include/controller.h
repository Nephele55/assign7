#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <termios.h>
#include <vector>
#include "view.h"
#include "gameObject.h"
#include "snake.h"
#include "food.h"
#include <queue>
class Controller{

public:
       
    Controller(View&);
    void run();
private:
    bool conti=1;
    bool mov=1;
    void handleInput(int);
    void update();
    void judge();
    void chooseAPlayer();
    // Model
    std::queue<unisnake*> snake;
    food* foodp;
    // View
    View& _view; 
};

static struct termios old_termios, new_termios;
void reset_terminal();
void configure_terminal();
int read_input();

#endif
