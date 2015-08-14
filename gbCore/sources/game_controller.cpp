//
//  game_controller.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/12/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "game_controller.h"
#include "game_transition.h"
#include "game_loop.h"
#include "configuration_types.h"

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
        m_transitions.insert(std::make_pair(transition->get_guid(), transition));
    }
    
    void game_controller::remove_transition(const std::shared_ptr<game_transition>& transition)
    {
        assert(m_transitions.find(transition->get_guid()) != m_transitions.end());
        m_transitions.erase(m_transitions.find(transition->get_guid()));
    }
    
    void game_controller::goto_transition(const std::string& guid)
    {
        assert(m_transitions.find(guid) != m_transitions.end());
        if(m_current_transition != nullptr)
        {
            m_current_transition->on_deactivated();
            remove_listener_from_game_loop(m_current_transition);
        }
        m_current_transition = m_transitions.find(guid)->second;
        m_current_transition->on_activated();
        
        std::shared_ptr<ces_configuration_component> configuration_component =
        std::make_shared<ces_configuration_component>("transition.demo.xml", e_configuration_type_transition);
        m_current_transition->add_component(configuration_component);
        configuration_component->add_listener(m_current_transition);
        
        add_listener_to_game_loop(m_current_transition);
    }
}