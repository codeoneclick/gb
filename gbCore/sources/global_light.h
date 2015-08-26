//
//  global_light.h
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef global_light_h
#define global_light_h

#include "main_headers.h"

namespace gb
{
    class global_light
    {
    private:
        
    protected:
        
        glm::mat4 m_matrix_v;
        glm::mat4 m_matrix_p;
        
        f32 m_distance_to_sun;
        f32 m_distance_to_look_at;
        
        f32 m_angle;
        glm::vec3 m_position;
        glm::vec3 m_look_at;
        glm::vec3 m_rotation_center;
        
    public:
        
        global_light(f32 fov, f32 near, f32 far);
        ~global_light(void);
        
        glm::vec3 get_position(void) const;
        
        void set_look_at(const glm::vec3& look_at);
        glm::vec3 get_look_at(void) const;
        
        void set_rotation_center(const glm::vec3& center);
        glm::vec3 get_rotation_center(void) const;
        
        void set_angle(f32 angle);
        f32 get_angle(void) const;
        
        void set_distance_to_sun(f32 distance);
        void set_distance_to_look_at(f32 distance);
        
        glm::mat4 get_matrix_v(void) const;
        glm::mat4 get_matrix_p(void) const;
        
        void update(f32 deltatime);
    };
};

#endif
