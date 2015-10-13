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
    inline glm::mat4 camera::get_matrix_v() const
    {
        return m_matrix_v;
    }
    
    inline glm::mat4 camera::get_matrix_i_v() const
    {
        return m_matrix_i_v;
    }
    
    inline glm::mat4 camera::get_matrix_i_vp() const
    {
        return m_matrix_i_vp;
    }
    
    inline glm::mat4 camera::get_matrix_p() const
    {
        return m_matrix_p;
    }
    
    inline glm::mat4 camera::get_matrix_n() const
    {
        return m_matrix_n;
    }
    
    inline void camera::set_position(const glm::vec3& position)
    {
        m_position = position;
    }
    
    inline glm::vec3 camera::get_position() const
    {
        return m_position;
    }
    
    inline void camera::set_look_at(const glm::vec3& look_at)
    {
        m_look_at = look_at;
    }
    
    inline glm::vec3 camera::get_look_at() const
    {
        return m_look_at;
    }
    
    inline glm::vec3 camera::get_up() const
    {
        return m_up;
    }
    
    inline void camera::set_rotation(f32 rotation)
    {
        m_rotation = rotation;
    }
    
    inline f32 camera::get_rotation() const
    {
        return m_rotation;
    }
    
    inline void camera::set_distance_to_look_at(const glm::vec3& distance)
    {
        m_distance = distance;
    }
    
    inline glm::vec3 camera::get_distance_to_look_at() const
    {
        return m_distance;
    }
    
    inline void camera::set_fov(f32 fov)
    {
        m_fov = fov;
        m_matrix_p = glm::perspective(m_fov, m_aspect, m_near, m_far);
    }
    
    inline f32 camera::get_fov() const
    {
        return m_fov;
    }
    
    inline f32 camera::get_aspect() const
    {
        return m_aspect;
    }
    
    inline f32 camera::get_near() const
    {
        return m_near;
    }
    
    inline f32 camera::get_far() const
    {
        return m_far;
    }
    
    inline glm::ivec4 camera::get_viewport() const
    {
        return m_viewport;
    }
    
    inline frustum_shared_ptr camera::get_frustum() const
    {
        return m_frustum;
    }
    
    
    inline f32 camera::get_yaw() const
    {
        if (m_matrix_v[0][0] == 1.f ||
            m_matrix_v[0][0] == -1.f)
        {
            return atan2f(m_matrix_v[0][2], m_matrix_v[2][3]);
        }
        return atan2(-m_matrix_v[2][0], m_matrix_v[0][0]);
    }
    
    inline f32 camera::get_pitch() const
    {
        if (m_matrix_v[0][0] == 1.f ||
            m_matrix_v[0][0] == -1.f)
        {
            return 0.f;
        }
        return asin(-m_matrix_v[1][0]);
    }
    
    inline f32 camera::get_roll() const
    {
        if (m_matrix_v[0][0] == 1.f ||
            m_matrix_v[0][0] == -1.f)
        {
            return 0.f;
        }
        return atan2(-m_matrix_v[1][2], m_matrix_v[1][1]);
    }
}

#endif
