//
//  ces_global_light_component.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "ces_global_light_component.h"

namespace gb
{
    ces_global_light_component::ces_global_light_component(void) :
    m_global_light(nullptr)
    {
        
    }
    
    ces_global_light_component::~ces_global_light_component(void)
    {
        
    }
    
    void ces_global_light_component::set_global_light(const std::shared_ptr<global_light>& light)
    {
        m_global_light = light;
    }
    
    std::shared_ptr<global_light> ces_global_light_component::get_global_light(void) const
    {
        return m_global_light;
    }
}