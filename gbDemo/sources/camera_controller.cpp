//
//  camera_controller.cpp
//  gbDemo
//
//  Created by sergey.sergeev on 11/24/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#include "camera_controller.h"
#include "camera_navigator.h"

namespace koth
{
    camera_controller::camera_controller(const gb::camera_shared_ptr& camera) :
    m_camera(camera),
    m_move_state(koth::e_navigation_state_move_none),
    m_rotate_state(koth::e_navigation_state_rotate_none),
    m_camera_move_speed(0.f)
    {
        m_camera_navigator = std::make_shared<camera_navigator>(3.f,
                                                                1.5f,
                                                                3.f,
                                                                2.f,
                                                                m_camera);
    }
    
    camera_controller::~camera_controller()
    {
        
    }
    
    void camera_controller::update(f32 deltatime)
    {
        switch (m_move_state)
        {
            case koth::e_navigation_state_move_forward:
            {
                m_camera_navigator->move_forward();
            }
                break;
            case koth::e_navigation_state_move_backward:
            {
                m_camera_navigator->move_backward();
            }
                break;
                
            default:
            {
            }
                break;
        }
        
        switch (m_rotate_state)
        {
            case koth::e_navigation_state_rotate_left:
            {
                m_camera_navigator->rotate_left();
            }
                break;
                
            case koth::e_navigation_state_rotate_right:
            {
                m_camera_navigator->rotate_right();
            }
                break;
                
            default:
            {
                
            }
                break;
        }
        m_camera_navigator->update(deltatime);
    }
    
    void camera_controller::set_position(const glm::vec3& position)
    {
        m_camera_navigator->set_position(position);
    }
    
    void camera_controller::set_rotation(const glm::vec3& rotation)
    {
        m_camera_navigator->set_rotation(rotation);
    }
    
    void camera_controller::set_move_state(i32 state)
    {
        m_move_state = state;
    }
    
    void camera_controller::set_rotate_state(i32 state)
    {
        m_rotate_state = state;
    }
}