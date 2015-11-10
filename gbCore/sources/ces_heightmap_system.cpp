//
//  ces_heightmap_system.cpp
//  gbCore
//
//  Created by sergey.sergeev on 11/9/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#include "ces_heightmap_system.h"
#include "ces_heightmap_component.h"

namespace gb
{
    ces_heightmap_system::ces_heightmap_system(const std::shared_ptr<graphics_context>& graphic_context_ptr) :
    m_graphic_context(graphic_context_ptr)
    {
         m_type = e_ces_system_type_heightmap;
    }
    
    ces_heightmap_system::~ces_heightmap_system()
    {
        
    }
    
    void ces_heightmap_system::on_feed_start(f32 deltatime)
    {

    }
    
    void ces_heightmap_system::on_feed(const ces_entity_shared_ptr& entity, f32 deltatime)
    {
        ces_heightmap_component_shared_ptr heightmap_component =
        std::static_pointer_cast<ces_heightmap_component>(entity->get_component(e_ces_component_type_heightmap));

        if(heightmap_component)
        {
            if(heightmap_component->get_generation_state() == ces_heightmap_component::e_heightmap_generation_state_none)
            {
                heightmap_component->generate(m_graphic_context);
            }
            else if(heightmap_component->get_generation_state() == ces_heightmap_component::e_heightmap_generation_state_generated)
            {
                heightmap_component->update(deltatime);
            }
        }
    }
    
    void ces_heightmap_system::on_feed_end(f32 deltatime)
    {

    }
}