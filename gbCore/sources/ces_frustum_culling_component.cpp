//
//  ces_frustum_culling_component.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "ces_frustum_culling_component.h"

namespace gb
{
    ces_frustum_culling_component::ces_frustum_culling_component(void) :
    m_frustum(nullptr)
    {
        m_type = e_ces_component_type_frustum_culling;
    }
    
    ces_frustum_culling_component::~ces_frustum_culling_component(void)
    {
        
    }
    
    void ces_frustum_culling_component::set_frustum(const std::shared_ptr<frustum>& frustum)
    {
        m_frustum = frustum;
    }
    
    std::shared_ptr<frustum> ces_frustum_culling_component::get_frustum(void) const
    {
        return m_frustum;
    }
}