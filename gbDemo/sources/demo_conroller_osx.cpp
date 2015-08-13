//
//  demo_conroller_osx.cpp
//  gbDemo
//
//  Created by Sergey Sergeev on 8/13/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "demo_conroller_osx.h"
#include "demo_game_controller.h"
#include "demo_game_transition.h"
#include "material.h"

demo_conroller_osx::demo_conroller_osx(void) :
m_game_controller(std::make_shared<demo_game_controller>(nullptr)),
m_game_transition(std::make_shared<demo_game_transition>("demo", false))
{
    m_game_controller->add_transition(m_game_transition);
    m_game_controller->goto_transition("demo");
    //gb::material *mat = new gb::material();
}

demo_conroller_osx::~demo_conroller_osx(void)
{
    
}