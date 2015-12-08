//
//  character_controller.h
//  gbDemo
//
//  Created by sergey.sergeev on 10/7/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef character_controller_h
#define character_controller_h

#include "main_headers.h"
#include "declarations.h"
#include "koth_declarations.h"

namespace koth
{
    enum e_jump_state
    {
        e_jump_state_undefined = 0,
        e_jump_state_starting,
        e_jump_state_ending
    };
    
    class character_controller
    {
    private:
        
    protected:
        
        gb::game_object_shared_ptr m_game_object;
        gb::camera_shared_ptr m_camera;
        game_object_navigator_shared_ptr m_game_object_navigator;
        
        i32 m_move_state;
        i32 m_rotate_state;
        
        f32 m_camera_move_speed;
        
        void increaseSpeed();
        void decreaseSpeed();
        
        bool m_is_jump_forward;
        e_jump_state m_jump_state;
        
    public:
        
        character_controller(const gb::game_object_shared_ptr& game_object,
                             const gb::camera_shared_ptr& camera);
        ~character_controller();
        
        virtual void update(f32 deltatime);
        
        virtual void set_position(const glm::vec3& position);
        void set_rotation(const glm::vec3& rotation);
        
        void set_move_state(i32 state);
        void set_rotate_state(i32 state);
        
        void jump_forward();
    };
};

#endif
