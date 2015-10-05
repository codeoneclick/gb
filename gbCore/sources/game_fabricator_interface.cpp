//
//  game_fabricator_interface.cpp
//  gbCore
//
//  Created by sergey.sergeev on 10/5/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "game_fabricator_interface.h"

namespace gb
{
    game_fabricator_interface::game_fabricator_interface() :
    m_configuration_accessor(nullptr),
    m_resource_accessor(nullptr)
    {
        
    }
    
    void game_fabricator_interface::set_configuration_accessor(const configuration_accessor_shared_ptr& configuration_accessor)
    {
        m_configuration_accessor = configuration_accessor;
    }
    
    void game_fabricator_interface::set_resource_accessor(const resource_accessor_shared_ptr& resource_accessor)
    {
        m_resource_accessor = resource_accessor;
    }
}