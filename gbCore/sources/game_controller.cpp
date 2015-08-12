//
//  game_controller.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/12/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "game_controller.h"
namespace gb
{
    game_controller::game_controller(const std::shared_ptr<ogl_window>& window)
    {
        
    }
    
    game_controller::~game_controller(void)
    {
        m_transitions.clear();
    }
    
    void game_controller::add_transition(const std::shared_ptr<game_transition>& transition)
    {
        m_transitions.insert(std::make_pair(transition, ))
    }
    
    void game_controller::remove_transition(const std::shared_ptr<game_transition>& transition)
    {
        
    }
    
    void game_controller::goto_transition(const std::string& guid)
    {
        
    }
}