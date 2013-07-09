//
//  misc.cpp
//  emptyExample
//
//  Created by tomahoku on 6/5/13.
//
//

#include "misc.h"

std::string IntToString(int number){
    std::stringstream ss;
    ss << number;
    return ss.str();
}