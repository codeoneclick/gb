//
//  ces_system.cpp
//  gbCore
//
//  Created by Sergey Sergeev on 8/13/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "ces_system.h"

namespace gb
{
    const struct ces_system_type ces_system_type =
    {
        "index.01.ces_system_type_camera",
        "index.02.ces_system_type_animation",
        "index.03.ces_system_type_input",
        "index.04.ces_system_type_particle_emitter",
        "index.05.ces_system_type_box2d",
        "index.06.ces_system_type_skybox",
        "index.07.ces_system_type_ocean",
        "index.08.ces_system_type_batch",
        "index.09.ces_system_type_render"
    };
    
    ces_system::ces_system() :
    m_type("")
    {
        
    }
    
    ces_system::~ces_system()
    {
        
    }
    
    std::string ces_system::get_type() const
    {
        return m_type;
    }
}