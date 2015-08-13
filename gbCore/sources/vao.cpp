//
//  vao.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/13/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "vao.h"
#include "vbo.h"
#include "ibo.h"

namespace gb
{
    vao::vao(const std::shared_ptr<vbo>& vbo,
             const std::shared_ptr<ibo>& ibo) :
    m_vbo(vbo),
    m_ibo(ibo)
    {
        gl_create_vertex_arrays(1, &m_handle);
        gl_bind_vertex_array(m_handle);
    }
    
    vao::~vao(void)
    {
        gl_delete_vertex_arrays(1, &m_handle);
    }
    
    void vao::init(const std::array<i32, e_shader_attribute_max>& attributes)
    {
        assert(m_vbo != nullptr);
        assert(m_ibo != nullptr);
        
        gl_bind_vertex_array(m_handle);
        
        m_vbo->bind(attributes);
        m_ibo->bind();
        
        vao::unbind();
    }
    
    void vao::bind(const std::shared_ptr<vao>& state)
    {
        gl_bind_vertex_array(state->m_handle);
    }
    
    void vao::unbind(void)
    {
        gl_bind_vertex_array(NULL);
    }
}