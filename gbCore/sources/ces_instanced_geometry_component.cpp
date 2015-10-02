//
//  ces_instanced_geometry_component.cpp
//  gbCore
//
//  Created by sergey.sergeev on 10/1/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "ces_instanced_geometry_component.h"
#include "instanced_mesh.h"

namespace gb
{
    ces_instanced_geometry_component::ces_instanced_geometry_component() :
    m_instanced_mesh(nullptr)
    {
        m_type = e_ces_component_type_geometry;
    }
    
    ces_instanced_geometry_component::~ces_instanced_geometry_component()
    {
        
    }
    
    void ces_instanced_geometry_component::set_mesh(const mesh_shared_ptr& mesh)
    {
        m_mesh = mesh;
        m_instanced_mesh = std::static_pointer_cast<instanced_mesh>(mesh);
    }
    
    mesh_shared_ptr ces_instanced_geometry_component::get_mesh() const
    {
        return m_instanced_mesh;
    }
}
