//
//  camera_controller.h
//  gbDemo
//
//  Created by sergey.sergeev on 11/24/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#ifndef camera_controller_h
#define camera_controller_h

#include "main_headers.h"
#include "declarations.h"
#include "koth_declarations.h"

namespace koth
{
    class camera_controller
    {
    private:
        
    protected:
        
        gb::camera_shared_ptr m_camera;
        camera_navigator_shared_ptr m_camera_navigator;
        
        i32 m_move_state;
        i32 m_rotate_state;
        
        f32 m_camera_move_speed;
        
    public:
        
        camera_controller(const gb::camera_shared_ptr& camera);
        ~camera_controller();
        
        void update(f32 deltatime);
        
        void set_position(const glm::vec3& position);
        void set_rotation(const glm::vec3& rotation);
        
        void set_move_state(i32 state);
        void set_rotate_state(i32 state);
    };
};


#endif
