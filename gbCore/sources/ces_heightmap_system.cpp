//
//  ces_heightmap_system.cpp
//  gbCore
//
//  Created by sergey.sergeev on 11/9/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#include "ces_heightmap_system.h"

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

    }
    
    void ces_heightmap_system::on_feed_end(f32 deltatime)
    {

    }
}