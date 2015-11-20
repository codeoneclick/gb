//
//  ces_terrain_system.cpp
//  gbCore
//
//  Created by sergey.sergeev on 11/9/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#include "ces_terrain_system.h"
#include "ces_terrain_component.h"

namespace gb
{
    ces_terrain_system::ces_terrain_system(const graphics_context_shared_ptr& graphic_context,
                                               const resource_accessor_shared_ptr& resource_accessor) :
    m_graphic_context(graphic_context),
    m_resource_accessor(resource_accessor)
    {
         m_type = e_ces_system_type_terrain;
    }
    
    ces_terrain_system::~ces_terrain_system()
    {
        
    }
    
    void ces_terrain_system::on_feed_start(f32 deltatime)
    {

    }
    
    void ces_terrain_system::on_feed(const ces_entity_shared_ptr& entity, f32 deltatime)
    {
        ces_terrain_component_shared_ptr terrain_component =
        std::static_pointer_cast<ces_terrain_component>(entity->get_component(e_ces_component_type_terrain));

        if(terrain_component)
        {
            if(terrain_component->get_generation_state() == ces_terrain_component::e_terrain_generation_state_none)
            {
                terrain_component->generate(m_graphic_context, m_resource_accessor);
            }
            else if(terrain_component->get_generation_state() == ces_terrain_component::e_terrain_generation_state_generated)
            {
                terrain_component->update(deltatime);
            }
        }
    }
    
    void ces_terrain_system::on_feed_end(f32 deltatime)
    {

    }
}