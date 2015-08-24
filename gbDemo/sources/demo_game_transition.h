//
//  demo_game_transition.h
//  gbDemo
//
//  Created by Sergey Sergeev on 8/13/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef demo_game_transition_h
#define demo_game_transition_h

#include "game_transition.h"

class demo_game_transition : public gb::game_transition
{
private:
    
protected:
    
public:
    
    demo_game_transition(const std::string& guid, bool is_offscreen);
    ~demo_game_transition(void);
};

#endif