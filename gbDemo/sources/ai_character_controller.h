//
//  ai_character_controller.h
//  gbDemo
//
//  Created by sergey.sergeev on 10/15/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#ifndef ai_character_controller_h
#define ai_character_controller_h

#include "character_controller.h"

namespace koth
{
    class ai_character_controller : public character_controller
    {
    private:
        
    protected:
        
        pathfinder_shared_ptr m_pathfinder;
        glm::ivec2 m_target_position;
        
    public:
        
        ai_character_controller(const gb::game_object_shared_ptr& game_object);
        ~ai_character_controller();
        
        void update(f32 deltatime);
        
        void set_target_position(const glm::vec3& position);
    };
};

#endif
