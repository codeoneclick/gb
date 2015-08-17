//
//  camera.hpp
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef camera_hpp
#define camera_hpp

namespace gb
{
    inline glm::mat4 camera::get_matrix_v(void) const
    {
        return m_matrix_v;
    }
    
    inline glm::mat4 camera::get_matrix_p(void) const
    {
        return m_matrix_p;
    }
    
    inline glm::mat4 camera::get_matrix_n(void) const
    {
        return m_matrix_n;
    }
    
    inline glm::mat4 camera::get_matrix_iv(void) const
    {
        return m_matrix_iv;
    }
    
    inline glm::mat4 camera::get_matrix_vp(void) const
    {
        return m_matrix_vp;
    }
    
    inline glm::mat4 camera::get_matrix_ivp(void) const
    {
        return m_matrix_ivp;
    }
    
    inline void camera::set_position(const glm::vec3& position)
    {
        m_position = position;
    }
    
    inline glm::vec3 camera::get_position(void) const
    {
        return m_position;
    }
    
    inline void camera::set_look_at(const glm::vec3& look_at)
    {
        m_look_at = look_at;
    }
    
    inline glm::vec3 camera::get_look_at(void) const
    {
        return m_look_at;
    }
    
    inline glm::vec3 camera::get_up(void) const
    {
        return m_up;
    }
    
    inline void camera::set_rotation(f32 rotation)
    {
        m_rotation = rotation;
    }
    
    inline f32 camera::get_rotation(void) const
    {
        return m_rotation;
    }
    
    inline void camera::set_distance_to_look_at(const glm::vec3& distance)
    {
        m_distance = distance;
    }
    
    inline glm::vec3 camera::get_distance_to_look_at(void) const
    {
        return m_distance;
    }
    
    inline void camera::set_fov(f32 fov)
    {
        m_fov = fov;
        m_matrix_p = glm::perspective(m_fov, m_aspect, m_near, m_far);
    }
    
    inline f32 camera::get_fov(void) const
    {
        return m_fov;
    }
    
    inline f32 camera::get_aspect(void) const
    {
        return m_aspect;
    }
    
    inline f32 camera::get_near(void) const
    {
        return m_near;
    }
    
    inline f32 camera::get_far(void) const
    {
        return m_far;
    }
    
    inline glm::ivec4 camera::get_viewport(void) const
    {
        return m_viewport;
    }
}

#endif
