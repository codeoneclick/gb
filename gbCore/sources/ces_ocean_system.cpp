//
//  ces_ocean_system.cpp
//  gbCore
//
//  Created by sergey.sergeev on 10/13/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#include "ces_ocean_system.h"
#include "ces_ocean_component.h"
#include "ces_render_component.h"

namespace gb
{
    ces_ocean_system::ces_ocean_system()
    {
        m_type = e_ces_system_type_ocean;
    }
    
    ces_ocean_system::~ces_ocean_system()
    {
        
    }
    
    void ces_ocean_system::on_feed_start(f32 deltatime)
    {
    }
    
    void ces_ocean_system::on_feed(const ces_entity_shared_ptr& entity, f32 deltatime)
    {
        ces_ocean_component_shared_ptr ocean_component =
        std::static_pointer_cast<ces_ocean_component>(entity->get_component(e_ces_component_type_ocean));
        
        ces_render_component_shared_ptr render_component =
        std::static_pointer_cast<ces_render_component>(entity->get_component(e_ces_component_type_render));
        
        if(ocean_component && render_component)
        {
            ocean_component->update(deltatime);
            render_component->set_custom_shader_uniform(ocean_component->get_wave_generator_time(), "u_f32_timer");
        }
    }
    
    void ces_ocean_system::on_feed_end(f32 deltatime)
    {
        
    }
}