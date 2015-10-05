//
//  game_graph_interface.cpp
//  gbCore
//
//  Created by sergey.sergeev on 10/5/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "game_graph_interface.h"

namespace gb
{
    game_graph_interface::game_graph_interface() :
    m_systems_feeder(nullptr)
    {
        
    }
    
    void game_graph_interface::set_systems_feeder(const ces_systems_feeder_shared_ptr& systems_feeder)
    {
        m_systems_feeder = systems_feeder;
    }
}