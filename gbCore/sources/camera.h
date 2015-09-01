//
//  camera.h
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef camera_h
#define camera_h

#include "main_headers.h"

namespace gb
{
    class camera
    {
    private:
        
        glm::mat4 m_matrix_v;
        glm::mat4 m_matrix_p;
        glm::mat4 m_matrix_n;
        glm::mat4 m_matrix_iv;
        glm::mat4 m_matrix_vp;
        glm::mat4 m_matrix_ivp;
        
        glm::vec3 m_position;
        glm::vec3 m_look_at;
        glm::vec3 m_up;
        f32 m_rotation;
        glm::vec3 m_distance;
        
        f32 m_fov;
        f32 m_aspect;
        f32 m_near;
        f32 m_far;
        
        glm::ivec4 m_viewport;
        
    protected:
        
    public:
        
        camera(f32 fov, f32 near, f32 far, glm::ivec4 viewport);
        ~camera(void);
        
        void update(f32 deltatime);
        
        inline glm::mat4 get_matrix_v(void) const;
        inline glm::mat4 get_matrix_p(void) const;
        inline glm::mat4 get_matrix_n(void) const;
        inline glm::mat4 get_matrix_iv(void) const;
        inline glm::mat4 get_matrix_vp(void) const;
        inline glm::mat4 get_matrix_ivp(void) const;
        
        inline void set_position(const glm::vec3& position);
        inline glm::vec3 get_position(void) const;
        
        inline void set_look_at(const glm::vec3& look_at);
        inline glm::vec3 get_look_at(void) const;
        
        inline glm::vec3 get_up(void) const;
        
        inline void set_rotation(f32 rotation);
        inline f32 get_rotation(void) const;
        
        inline void set_distance_to_look_at(const glm::vec3& distance);
        inline glm::vec3 get_distance_to_look_at(void) const;
        
        inline void set_fov(f32 fov);
        inline f32 get_fov(void) const;
        
        inline f32 get_aspect(void) const;
        inline f32 get_near(void) const;
        inline f32 get_far(void) const;
        
        inline glm::ivec4 get_viewport(void) const;
        
        static glm::mat4 get_matrix_s(const glm::mat4& mat_v, const glm::vec3& camera_position, const glm::vec3& position); // spherical
        static glm::mat4 get_matrix_c(const glm::vec3& camera_position, const glm::vec3& position); // cylindrical
    };
};

#include "camera.hpp"

#endif
