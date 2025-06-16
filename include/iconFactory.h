// TODO 
// Finish the declaration of IconFactory that
// 1. offers some methods to create an Icon.
// 2. encapsulates the constructor from the caller.
#ifndef ICONFACTORY_H
#define ICONFACTORY_H 
#include "icon.h"
#include <string>
class iconFactory{      
    public:
        static Icon makeframeicon(int width, int height, Color color, std::string s);
};
#endif