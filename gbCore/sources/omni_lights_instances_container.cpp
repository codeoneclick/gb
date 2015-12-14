//
//  omni_lights_instances_container.cpp
//  gbCore
//
//  Created by sergey.sergeev on 12/14/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#include "omni_lights_instances_container.h"

namespace gb
{
    omni_lights_instances_container::omni_lights_instances_container()
    {
        
    }
    
    omni_lights_instances_container::~omni_lights_instances_container()
    {
        
    }
    
    void omni_lights_instances_container::add_omni_light(const omni_light_shared_ptr& omni_light)
    {
        m_lights.insert(omni_light);
    }
    
    void omni_lights_instances_container::remove_omni_light(const omni_light_shared_ptr& omni_light)
    {
        m_lights.erase(omni_light);
    }
    
    ui32 omni_lights_instances_container::get_omni_lights_count() const
    {
        return static_cast<ui32>(m_lights.size());
    }
}