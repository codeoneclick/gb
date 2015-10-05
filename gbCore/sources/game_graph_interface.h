//
//  game_graph_interface.h
//  gbCore
//
//  Created by sergey.sergeev on 10/5/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef game_graph_interface_h
#define game_graph_interface_h

#include "main_headers.h"
#include "declarations.h"
#include "game_loop.h"

namespace gb
{
    class game_graph_interface : public game_loop_interface
    {
    private:
        
    protected:
        
        ces_systems_feeder_shared_ptr m_systems_feeder;
        
    public:
        
        game_graph_interface();
        ~game_graph_interface() = default;
        
        void set_systems_feeder(const ces_systems_feeder_shared_ptr& systems_feeder);
    };
};


#endif 
