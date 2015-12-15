//
//  i_scene_graph.cpp
//  gbCore
//
//  Created by sergey.sergeev on 10/5/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "i_scene_graph.h"

namespace gb
{
    i_scene_graph::i_scene_graph() :
    m_systems_feeder(nullptr),
    m_resource_accessor(nullptr)
    {
        
    }
    
    void i_scene_graph::set_systems_feeder(const ces_systems_feeder_shared_ptr& systems_feeder)
    {
        m_systems_feeder = systems_feeder;
    }
    
    void i_scene_graph::set_resource_accessor(const resource_accessor_shared_ptr& resource_accessor)
    {
        m_resource_accessor = resource_accessor;
    }
}