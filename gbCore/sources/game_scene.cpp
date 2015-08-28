//
//  game_scene.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/27/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "game_scene.h"

namespace gb
{
    game_scene::game_scene(const game_transition_shared_ptr& transition) :
    m_transition(transition)
    {
        
    }
    
    game_scene::~game_scene()
    {
        
    }
    
    game_transition_shared_ptr game_scene::get_transition() const
    {
        return m_transition.lock();
    }
};