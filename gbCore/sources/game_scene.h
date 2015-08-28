//
//  game_scene.h
//  gbCore
//
//  Created by sergey.sergeev on 8/27/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef game_scene_h
#define game_scene_h

#include "main_headers.h"
#include "declarations.h"

namespace gb
{
    class game_scene
    {
    private:
        
        std::weak_ptr<game_transition> m_transition;
        
    protected:
        
        game_transition_shared_ptr get_transition() const;
        
    public:
        
        game_scene(const game_transition_shared_ptr& transition);
        ~game_scene();
        
        virtual void update(f32 deltatime) = 0;
    };
};

#endif
