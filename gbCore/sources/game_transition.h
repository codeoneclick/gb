//
//  game_transition.h
//  gbCore
//
//  Created by sergey.sergeev on 8/12/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef game_transition_h
#define game_transition_h

#include "main_headers.h"
#include "game_loop.h"

namespace gb
{
    class game_transition : public game_loop_interface
    {
    private:
        
    protected:
        
        void on_update(f32 deltatime) = 0;
        
        friend class game_controller;
        void on_activated(void);
        void on_deactivated(void);
        
    public:
        
        game_transition(const std::string& filename, bool is_offscreen);
        virtual ~game_transition(void);
        
        virtual void create_scene(void);
    };
};

#endif
