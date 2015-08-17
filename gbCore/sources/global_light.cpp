//
//  global_light.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "global_light.h"

namespace gb
{
    global_light::global_light(f32 fov, f32 near, f32 far) :
    m_position(glm::vec3(0.0)),
    m_look_at(glm::vec3(0.0)),
    m_distance_to_sun(0.0),
    m_distance_to_look_at(0.0),
    m_angle(0.0),
    m_rotation_center(glm::vec3(0.0))
    {
        m_matrix_p = glm::perspective(fov, 1.0f, near, far);
    }
    
    global_light::~global_light(void)
    {
        
    }
    
    glm::vec3 global_light::get_position(void) const
    {
        return m_position;
    }
    
    void global_light::set_look_at(const glm::vec3& look_at)
    {
        m_look_at = look_at;
    }
    
    glm::vec3 global_light::get_look_at(void) const
    {
        return m_look_at;
    }
    
    void global_light::set_angle(f32 angle)
    {
        m_angle = angle;
    }
    
    f32 global_light::get_angle(void) const
    {
        return m_angle;
    }
    
    glm::mat4x4 global_light::get_matrix_v(void) const
    {
        return m_matrix_v;
    }
    
    glm::mat4x4 global_light::get_matrix_p(void) const
    {
        return m_matrix_p;
    }
    
    void global_light::set_distance_to_sun(f32 distance)
    {
        m_distance_to_sun = distance;
    }
    
    void global_light::set_distance_to_look_at(f32 distance)
    {
        m_distance_to_look_at = distance;
    }
    
    void global_light::set_rotation_center(const glm::vec3& center)
    {
        m_rotation_center = center;
    }
    
    glm::vec3 global_light::get_rotation_center(void) const
    {
        return m_rotation_center;
    }
    
    void global_light::update(f32 deltatime)
    {
        m_position = glm::vec3(sinf(m_angle) * -m_distance_to_sun + m_rotation_center.x,
                               cosf(m_angle) * -m_distance_to_sun + m_rotation_center.y,
                               m_rotation_center.z);
        
        glm::vec3 position(sinf(m_angle) * -m_distance_to_look_at + m_look_at.x,
                           cosf(m_angle) * -m_distance_to_look_at + m_look_at.y,
                           m_look_at.z);
        
        m_matrix_v = glm::lookAt(position, m_look_at, glm::vec3(0.0, 1.0, 0.0));
    }
}