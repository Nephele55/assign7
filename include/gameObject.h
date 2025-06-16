#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include "unit.h"
#include "icon.h"
#include <iostream>

class GameObject{

public:
    GameObject(const Position&, const Icon&);
    Position getPosition() const;
    Icon getIcon() const;
    virtual void update();
    void move(int dx, int dy);
protected:

    Position _pos;
    Icon _icon;
};

#endif
