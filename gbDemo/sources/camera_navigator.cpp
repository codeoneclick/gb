//
//  camera_navigator.cpp
//  gbDemo
//
//  Created by sergey.sergeev on 11/24/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#include "camera_navigator.h"
#include "camera.h"

namespace koth
{
    camera_navigator::camera_navigator(f32 move_forward_speed,
                                       f32 move_backward_speed,
                                       f32 move_side_speed,
                                       f32 rotate_speed,
                                       const gb::camera_shared_ptr& camera) :
    koth::common_navigator(move_forward_speed,
                           move_backward_speed,
                           move_side_speed,
                           rotate_speed),
    m_camera(camera),
    m_horizontal_angle(0.f),
    m_vertical_angle(0.f)
    {
        
    }
    
    camera_navigator::~camera_navigator()
    {
        
    }
    
    void camera_navigator::update(f32 deltatime)
    {
        switch (m_navigation_state_rotate)
        {
            case e_navigation_state_rotate_left:
            {
                m_horizontal_angle += m_rotate_speed  * deltatime;
            }
                break;
                
            case e_navigation_state_rotate_right:
            {
                m_horizontal_angle -= m_rotate_speed  * deltatime;
            }
                break;
                
            case e_navigation_state_rotate_up:
            {
                m_vertical_angle += m_rotate_speed  * deltatime;
            }
                break;
                
            case e_navigation_state_rotate_down:
            {
                m_vertical_angle -= m_rotate_speed  * deltatime;
            }
                break;
                
            case e_navigation_state_rotate_none:
            {
                
            }
                break;
                
            default:
            {
                assert(false);
            }
                break;
        }
        m_navigation_state_rotate = e_navigation_state_rotate_none;
        
        glm::vec3 direction(cosf(m_vertical_angle) * sinf(m_horizontal_angle),
                            sinf(m_vertical_angle),
                            cosf(m_vertical_angle) * cosf(m_horizontal_angle));
        
        glm::vec3 right = glm::vec3(sinf(m_horizontal_angle - M_PI * .5f),
                                    0.f,
                                    cosf(m_horizontal_angle - M_PI * .5f));
        
        glm::vec3 up = glm::cross(right, direction);
        
        switch (m_navigation_state_move)
        {
            case e_navigation_state_move_forward:
            {
                m_position += direction * m_move_forward_speed * deltatime;
            }
                break;
                
            case e_navigation_state_move_backward:
            {
                m_position -= direction * m_move_backward_speed * deltatime;
            }
                break;
                
            case e_navigation_state_move_left:
            {
                m_position += right * m_move_side_speed * deltatime;
            }
                break;
                
            case e_navigation_state_move_right:
            {
                m_position -= right * m_move_side_speed * deltatime;
            }
                break;
                
            case e_navigation_state_move_none:
            {
                
            }
                break;
                
            default:
            {
                assert(false);
            }
                break;
        }
        m_navigation_state_move = e_navigation_state_move_none;
        
        m_camera->set_position(m_position);
        m_camera->set_look_at(m_position + direction);
        m_camera->set_up(up);
    }
}