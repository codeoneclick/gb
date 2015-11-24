//
//  camera_navigator.h
//  gbDemo
//
//  Created by sergey.sergeev on 11/24/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#ifndef camera_navigator_h
#define camera_navigator_h

#include "common_navigator.h"

namespace koth
{
    class camera_navigator : public common_navigator
    {
    private:
        
    protected:
        
        gb::camera_shared_ptr m_camera;
        
        f32 m_horizontal_angle;
        f32 m_vertical_angle;
        
    public:
        
        camera_navigator(f32 move_forward_speed,
                         f32 move_backward_speed,
                         f32 move_side_speed,
                         f32 rotate_speed,
                         const gb::camera_shared_ptr& camera);
        ~camera_navigator();
        
        void update(f32 deltatime);
    };
};

#endif 
