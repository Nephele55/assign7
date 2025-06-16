// TODO implementation
#include "gameObject.h"
#include "icon.h"

GameObject::GameObject(const Position& p, const Icon& icon): _pos(p), _icon(icon){}
void GameObject::update(){

}
Position GameObject::getPosition() const{
    return _pos;
}

Icon GameObject::getIcon()const{
    return _icon;
}
void GameObject::move(int dx, int dy){
    if(_pos.e1+dx<20&&_pos.e1+dx>=0) _pos.e1 += dx;
    if(_pos.e2+dy<20&&_pos.e2+dy>=0) _pos.e2 += dy;
}