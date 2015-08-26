//
//  ces_render_system.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "ces_render_system.h"
#include "render_pipeline.h"
#include "ces_system_types.h"

namespace gb
{
    ces_render_system::ces_render_system(const std::shared_ptr<graphics_context>& graphic_context, bool is_offscreen)
    {
        m_type = e_ces_system_type_render;
        m_render_pipeline = std::make_shared<render_pipeline>(graphic_context, is_offscreen);
    }
    
    ces_render_system::~ces_render_system(void)
    {
        
    }
    
    std::shared_ptr<render_pipeline> ces_render_system::get_render_pipeline(void) const
    {
        return m_render_pipeline;
    }
    
    void ces_render_system::on_feed_start()
    {
        
    }
    
    void ces_render_system::on_feed(const std::shared_ptr<ces_entity>& entity)
    {
        m_render_pipeline->update(0.0f);
    }
    
    void ces_render_system::on_feed_end()
    {
        
    }
}