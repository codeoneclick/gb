//
//  character_controller.cpp
//  gbDemo
//
//  Created by sergey.sergeev on 10/7/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "character_controller.h"
#include "game_object_navigator.h"
#include "model3d_animated.h"
#include "camera.h"

namespace koth
{
    character_controller::character_controller(const gb::game_object_shared_ptr& game_object,
                                               const gb::camera_shared_ptr& camera) :
    m_game_object(game_object),
    m_camera(camera),
    m_move_state(koth::e_navigation_state_move_none),
    m_rotate_state(koth::e_navigation_state_rotate_none),
    m_camera_move_speed(0.f)
    {
        m_game_object_navigator = std::make_shared<game_object_navigator>(3.f,
                                                                          1.5f,
                                                                          2.f,
                                                                          game_object);
    }
    
    character_controller::~character_controller()
    {
        
    }
    
    void character_controller::update(f32 deltatime)
    {
        bool isMoved = false;
        switch (m_move_state)
        {
            case koth::e_navigation_state_move_forward:
            {
                m_game_object_navigator->move_forward();
                isMoved = true;
                character_controller::increaseSpeed();
            }
                break;
            case koth::e_navigation_state_move_backward:
            {
                m_game_object_navigator->move_backward();
                character_controller::decreaseSpeed();
                isMoved = true;
            }
                break;
                
            default:
            {
                character_controller::decreaseSpeed();
            }
                break;
        }
        
        switch (m_rotate_state)
        {
            case koth::e_navigation_state_rotate_left:
            {
                m_game_object_navigator->rotate_left();
                isMoved = true;
            }
                break;
                
            case koth::e_navigation_state_rotate_right:
            {
                m_game_object_navigator->rotate_right();
                isMoved = true;
            }
                break;
                
            default:
                break;
        }
        
        gb::model3d_animated_shared_ptr animated_model = std::static_pointer_cast<gb::model3d_animated>(m_game_object);
        if(isMoved && m_camera)
        {
            animated_model->set_animation("RUN");
        }
        else if(m_camera)
        {
            animated_model->set_animation("IDLE");
        }
        m_game_object_navigator->update(deltatime);
        
        if(m_camera)
        {
            f32 fov = glm::mix(45.f, 50.f, m_camera_move_speed);
            m_camera->set_fov(fov);
            m_camera->set_look_at(glm::vec3(std::max(std::min(m_game_object->get_position().x, 12.f), 4.f),
                                            m_game_object->get_position().y + glm::mix(.5f, 1.5f, m_camera_move_speed),
                                            std::max(std::min(m_game_object->get_position().z, 12.f), 4.f)));
        }
    }
    
    void character_controller::set_position(const glm::vec3& position)
    {
        m_game_object_navigator->set_position(position);
    }
    
    void character_controller::set_rotation(const glm::vec3& rotation)
    {
        m_game_object_navigator->set_rotation(rotation);
    }
    
    void character_controller::set_move_state(i32 state)
    {
        m_move_state = state;
    }
    
    void character_controller::set_rotate_state(i32 state)
    {
        m_rotate_state = state;
    }
    
    void character_controller::increaseSpeed()
    {
        if(m_camera_move_speed < 1.f)
        {
            m_camera_move_speed += .025f;
        }
    }
    
    void character_controller::decreaseSpeed()
    {
        if(m_camera_move_speed > 0.f)
        {
            m_camera_move_speed -= .05f;
        }
    }

}