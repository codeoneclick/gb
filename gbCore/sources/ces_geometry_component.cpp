//
//  ces_geometry_component.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "ces_geometry_component.h"

namespace gb
{
    ces_geometry_component::ces_geometry_component(void) :
    m_mesh(nullptr)
    {
        m_type = e_ces_component_type_geometry;
    }
    
    ces_geometry_component::~ces_geometry_component(void)
    {
        
    }
    
    void ces_geometry_component::set_mesh(const std::shared_ptr<mesh>& mesh)
    {
        m_mesh = mesh;
    }
    
    std::shared_ptr<mesh> ces_geometry_component::get_mesh(void) const
    {
        return m_mesh;
    }
}