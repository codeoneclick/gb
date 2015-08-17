//
//  ces_camera_component.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "ces_camera_component.h"

namespace gb
{
    ces_camera_component::ces_camera_component(void) :
    m_camera(nullptr)
    {
        m_type = e_ces_component_type_camera;
    }
    
    ces_camera_component::~ces_camera_component(void)
    {
        
    }
    
    void ces_camera_component::set_camera(const std::shared_ptr<camera>& camera)
    {
        m_camera = camera;
    }
    
    std::shared_ptr<camera> ces_camera_component::get_camera(void) const
    {
        return m_camera;
    }
}