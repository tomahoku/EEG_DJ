//
//  looper.h
//  emptyExample
//
//  Created by tomahoku on 6/5/13.
//
//

#ifndef __emptyExample__looper__
#define __emptyExample__looper__

#include <iostream>
#include "testApp.h"

class looper : ofSoundPlayer{
public:
    looper();
    ~looper();
    void setLoop(bool a);
};

#endif /* defined(__emptyExample__looper__) */
