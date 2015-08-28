//
//  ces_animation_system.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/28/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "ces_animation_system.h"
#include "ces_entity.h"
#include "ces_animation_component.h"

namespace gb
{
    ces_animation_system::ces_animation_system()
    {
        m_type = e_ces_system_type_animation;
    }
    
    ces_animation_system::~ces_animation_system()
    {
        
    }
    
    void ces_animation_system::on_feed_start(f32 deltatime)
    {
        
    }
    
    void ces_animation_system::on_feed(const ces_entity_shared_ptr& entity, f32 deltatime)
    {
        ces_animation_component_shared_ptr animation_component =
        std::static_pointer_cast<ces_animation_component>(entity->get_component(e_ces_component_type_animation));
        
        if(animation_component)
        {
            animation_component->update(deltatime);
        }
    }
    
    void ces_animation_system::on_feed_end(f32 deltatime)
    {
        
    }
}