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
#include "declarations.h"

namespace gb
{
    class camera
    {
    private:
        
        glm::mat4 m_matrix_v;
        glm::mat4 m_matrix_i_v;
        glm::mat4 m_matrix_i_vp;
        glm::mat4 m_matrix_p;
        glm::mat4 m_matrix_n;
        
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
        
        frustum_shared_ptr m_frustum;
        
    public:
        
        camera(f32 fov, f32 near, f32 far, glm::ivec4 viewport);
        ~camera(void);
        
        void update(f32 deltatime);
        
        inline glm::mat4 get_matrix_v() const;
        inline glm::mat4 get_matrix_i_v() const;
        inline glm::mat4 get_matrix_i_vp() const;
        inline glm::mat4 get_matrix_p() const;
        inline glm::mat4 get_matrix_n() const;
        
        inline void set_position(const glm::vec3& position);
        inline glm::vec3 get_position() const;
        
        inline void set_look_at(const glm::vec3& look_at);
        inline glm::vec3 get_look_at() const;
        
        inline void set_up(const glm::vec3& up);
        inline glm::vec3 get_up() const;
        
        inline void set_rotation(f32 rotation);
        inline f32 get_rotation() const;
        
        inline void set_distance_to_look_at(const glm::vec3& distance);
        inline glm::vec3 get_distance_to_look_at() const;
        
        inline void set_fov(f32 fov);
        inline f32 get_fov() const;
        
        inline f32 get_aspect() const;
        inline f32 get_near() const;
        inline f32 get_far() const;
        
        inline glm::ivec4 get_viewport() const;
        
        inline frustum_shared_ptr get_frustum() const;
        
        glm::mat4 get_matrix_s(const glm::vec3& position); // spherical
        glm::mat4 get_matrix_c(const glm::vec3& position); // cylindrical
        
        inline f32 get_yaw() const;
        inline f32 get_pitch() const;
        inline f32 get_roll() const;
    };
};

#include "camera.hpp"

#endif
