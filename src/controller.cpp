#include <iostream>
#include <ctime>
#include <thread>
#include <unistd.h>
#include <termios.h>

#include "environment.h"
#include "controller.h"
#include "gameObjectFactory.h"
#include <random>

Controller::Controller(View& view) : _view(view){
}

void Controller::run() {
    // initial setup
    std::cin.tie(0);
    std::ios::sync_with_stdio(0);
    configure_terminal();
    srand(time(0));
    // init state
    int input = -1;
    clock_t start, end;
    snake.push(GameObjectFactory::snakegameObjectFactory({0, 0}, Icon()).release());
    foodp=GameObjectFactory::foodgameObjectFactory({rand()%20, rand()%20}, Icon()).release();
    // Main loop
    while (conti) {
        start = clock();
        // game loop goes here
        input = read_input();

        // ESC to exit program
        if(input==27)break;

        this->handleInput(input);
        update();
        _view.resetLatest();

        _view.render();


        end = clock();

        // frame rate normalization
        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        if (time_taken > SPF) continue;
        int frameDelay = int((SPF - time_taken) * 1000); // 0.1 seconds
        if(frameDelay > 0) std::this_thread::sleep_for(std::chrono::milliseconds(frameDelay)); // frame delay
        judge();
    }

}

void Controller::chooseAPlayer(){
    int rnum=0;
    if(player) player->setplayer(false);
    for(auto& obj:_objs){
        if(obj->getType()==ROCK) rnum++;
    }
    if(rnum==0) return;
    int tar=rand()%rnum+1;
    for(auto& obj:_objs){
        if(obj->getType()==ROCK) tar--;
        if(tar==0){
            player=obj;
            player->setplayer(true);
            return;
        }
    }
}

void Controller::handleInput(int keyInput){

    // If there is no input, do nothing.
    if(keyInput==-1)return;
    switch(keyInput){
        case 9:
            chooseAPlayer();
            break;
        case 'Q': case 'q':
            mov^=1;
            break;
        case 'A': case 'a':
            player->move(-1, 0);
            break;
        case 'W': case 'w':
            player->move(0, -1);
            break;
        case 'S': case 's':
            player->move(0, 1);
            break;
        case 'D': case 'd':
            player->move(1, 0);
            break;
    }
    // TODO 
    // handle key events.

}

const int dir[4][2]={{-1, 0}, {0, -1}, {0, 1}, {1, 0}};
void Controller::update(){
    for(auto it=_objs.begin(); it!=_objs.end(); ++it){
        for(auto it2=it+1; it2!=_objs.end(); ++it2){
            if((*it)->intersect(*it2)){
                if((*it)->onCollision(*it2)||
                   (*it2)->onCollision(*it)) chooseAPlayer();
            }
        }
    }
    for(int i=0; i<_objs.size(); ++i){
        if(_objs[i]!= player&&mov) _objs[i]->move(dir[rand()%4][0], dir[rand()%4][1]);
    }

}
void Controller::judge(){
    int rnum=0, pnum=0, snum=0;
    for(auto& obj:_objs){
        if(obj->getType()==ROCK) rnum++;
        else if(obj->getType()==PAPER) pnum++;
        else if(obj->getType()==SCISSORS) snum++;
    }
    if(rnum==0){
        conti=0;
        std::cout<<"Paper wins!"<<std::endl;
    }
    else if(pnum==0){
        conti=0;
        std::cout<<"Scissors wins!"<<std::endl;
    }
    else if(snum==0){
        conti=0;
        std::cout<<"Rock wins!"<<std::endl;
    }
}
void reset_terminal() {
    printf("\e[m"); // reset color changes
    printf("\e[?25h"); // show cursor
    fflush(stdout);
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
}

// terminal initial configuration setup
void configure_terminal() {
    tcgetattr(STDIN_FILENO, &old_termios);
	new_termios = old_termios; // save it to be able to reset on exit
    
    new_termios.c_lflag &= ~(ICANON | ECHO); // turn off echo + non-canonical mode
    new_termios.c_cc[VMIN] = 0;
    new_termios.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    printf("\e[?25l"); // hide cursor
    std::atexit(reset_terminal);
}


int read_input() {
    fflush(stdout);
   	char buf[4096]; // maximum input buffer
	int n = read(STDIN_FILENO, buf, sizeof(buf));
    return n > 0 ? buf[n - 1] : -1;
}

