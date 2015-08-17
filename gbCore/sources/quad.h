//
//  quad.h
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef quad_h
#define quad_h

#include "main_headers.h"
#include "shader.h"

namespace gb
{
    class vbo;
    class ibo;
    class vao;
    class quad
    {
    private:
        
    protected:
        
        std::shared_ptr<vbo> m_vbo;
        std::shared_ptr<ibo> m_ibo;
        
        std::unordered_map<std::string, std::shared_ptr<vao>> m_vao_states;
        
        glm::vec3 m_max_bound;
        glm::vec3 m_min_bound;
        
    public:
        
        quad(void);
        ~quad(void);
        
        void bind(const std::string& attributes_guid, const std::array<i32, e_shader_attribute_max>& attributes);
        void draw(void);
        void unbind(const std::string& attributes_guid, const std::array<i32, e_shader_attribute_max>& attributes);
    };
};

#endif
