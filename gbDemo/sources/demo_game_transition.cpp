//
//  demo_game_transition.cpp
//  gbDemo
//
//  Created by Sergey Sergeev on 8/13/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "demo_game_transition.h"
#include "demo_game_scene.h"
#include "ces_input_system.h"
#include "ces_system_types.h"

demo_game_transition::demo_game_transition(const std::string& guid, bool is_offscreen) :
game_transition(guid, is_offscreen)
{
    
}

demo_game_transition::~demo_game_transition(void)
{
    
}

void demo_game_transition::create_scene()
{
    m_scene = std::make_shared<demo_game_scene>(shared_from_this());
    std::shared_ptr<gb::ces_input_system> input_system = std::static_pointer_cast<gb::ces_input_system>(game_transition::get_system(gb::e_ces_system_type_input));
    input_system->add_touch_listener(std::static_pointer_cast<demo_game_scene>(m_scene));
}

void demo_game_transition::destroy_scene()
{
    m_scene = nullptr;
}