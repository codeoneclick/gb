//
//  ces_base_component.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/11/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "ces_base_component.h"

namespace gb
{
    ces_base_component::ces_base_component() :
    m_type(e_ces_component_type_undefined)
    {
        
    }
    
    e_ces_component_type ces_base_component::get_type() const
    {
        return m_type;
    }
    
    void ces_base_component::set_scene_graph(const scene_graph_shared_ptr& scene_graph)
    {
        m_scene_graph= scene_graph;
    }
    
    scene_graph_shared_ptr ces_base_component::get_scene_graph() const
    {
        return m_scene_graph.lock();
    }
};