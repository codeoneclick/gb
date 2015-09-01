//
//  ces_transformation_component.h
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef ces_transformation_component_h
#define ces_transformation_component_h

#include "ces_base_component.h"

namespace gb
{
    class ces_transformation_component : public ces_base_component
    {
    private:
        
    protected:
        
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;
        
        glm::mat4 m_matrix_t;
        glm::mat4 m_matrix_r;
        glm::mat4 m_matrix_s;
        
        glm::mat4 m_matrix_m;
        
        bool m_is_matrix_m_computed;
        
    public:
        
        ces_transformation_component(void);
        ~ces_transformation_component(void);
    
        void set_position(const glm::vec3& position);
        void set_rotation(const glm::vec3& rotation);
        void set_scale(const glm::vec3& scale);
        
        glm::vec3 get_position(void) const;
        glm::vec3 get_rotation(void) const;
        glm::vec3 get_scale(void) const;
        
        glm::vec3 get_forward(void) const;
        glm::vec3 get_up(void) const;
        glm::vec3 get_right(void) const;
        
        glm::mat4 get_matrix_m(void);
    };
};


#endif
