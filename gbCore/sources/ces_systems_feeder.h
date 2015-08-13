//
//  systems_feeder.h
//  gbCore
//
//  Created by Sergey Sergeev on 8/13/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef systems_feeder_h
#define systems_feeder_h

#include "ces_systems_feeder.h"
#include "game_loop.h"

namespace gb
{
    class ces_systems_feeder : public game_loop_interface
    {
    private:
        
    protected:
        
        void on_update(f32 deltatime);
        
    public:
        
        ces_systems_feeder(void) = default;
        ~ces_systems_feeder(void);
        
    };
};

#endif
