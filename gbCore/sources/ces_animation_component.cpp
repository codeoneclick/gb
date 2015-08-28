//
//  ces_animation_component.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/28/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "ces_animation_component.h"
#include "animation_mixer.h"

namespace gb
{
    ces_animation_component::ces_animation_component() :
    m_animation_mixer(nullptr)
    {
        m_type = e_ces_component_type_animation;
    }
    
    ces_animation_component::~ces_animation_component()
    {
        
    }
    
    void ces_animation_component::set_animation_mixer(const animation_mixer_shared_ptr& animation_mixer)
    {
        m_animation_mixer = animation_mixer;
    }
    
    animation_mixer_shared_ptr ces_animation_component::get_animation_mixer() const
    {
        return m_animation_mixer;
    }
    
    void ces_animation_component::update(f32 deltatime)
    {
        if(m_animation_mixer)
        {
            m_animation_mixer->update(deltatime);
        }
    }
}