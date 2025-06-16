#ifndef UNIT_H
#define UNIT_H
//座標
struct Vec2{
    int e1;
    int e2;
    int& x(){ return e1;}
    int& y(){ return e2;}
    // Add const overloads for x() and y()
    const int& x() const { return e1; } // Added const version
    const int& y() const { return e2; } // Added const version
    int& width(){return e1;}
    int& height(){return e2;}
    bool operator==(const Vec2& other){
        return e1==other.e1&&e2==other.e2;
    }
};

using Position = Vec2;

enum Color {BLACK=0, RED, GREEN, YELLOW, BLUE, PINK, CYAN, WHITE, NOCHANGE};

#endif