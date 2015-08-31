//
//  model3d_animated.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/28/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "model3d_animated.h"
#include "ces_animation_component.h"
#include "ces_touch_component.h"
#include "animation_mixer.h"

namespace gb
{
    model3d_animated::model3d_animated()
    {
        m_animation_component = std::make_shared<ces_animation_component>();
        ces_entity::add_component(m_animation_component);
        
        m_touch_component = std::make_shared<ces_touch_component>();
        ces_entity::add_component(m_touch_component);
    }
    
    model3d_animated::~model3d_animated()
    {
        ces_entity::remove_component(m_animation_component);
    }
    
    void model3d_animated::set_animation_mixer(const animation_mixer_shared_ptr& animation_mixer)
    {
        m_animation_component->set_animation_mixer(animation_mixer);
    }
    
    void model3d_animated::set_animation(const std::string& name)
    {
        if(m_animation_component->get_animation_mixer())
        {
            m_animation_component->get_animation_mixer()->set_animation(name);
        }
    }

}