//
//  game_transition.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/12/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "game_transition.h"
#include "ces_systems_feeder.h"
#include "game_loop.h"

namespace gb
{
    game_transition::game_transition(const std::string& guid, bool is_offscreen) :
    m_guid(guid)
    {
        m_system_feeder = std::make_shared<ces_systems_feeder>();
    }
    
    game_transition::~game_transition(void)
    {
        
    }
    
    std::string game_transition::get_guid(void) const
    {
        return m_guid;
    }
    
    void game_transition::on_activated(void)
    {
        add_listener_to_game_loop(m_system_feeder);
        m_system_feeder->add_entity(shared_from_this());
    }
    
    void game_transition::on_deactivated(void)
    {
        
    }
    
    void game_transition::on_configuration_loaded(const std::shared_ptr<configuration>& configuration, bool success)
    {
        
    }
    
    void game_transition::on_update(f32 deltatime)
    {
        
    }
    
    void game_transition::create_scene(void)
    {
        
    }
}