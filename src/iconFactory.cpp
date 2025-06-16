// TODO implementation
#include "iconFactory.h"
Icon iconFactory::makeframeicon(int width, int height, Color color, std::string s){
    Icon icon;
    for(int i=0; i<height ; i++){
        std::vector<Cell> col(width, {color, s});
        icon.push_back(col);
    }
    return icon;
}