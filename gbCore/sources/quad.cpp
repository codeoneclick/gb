//
//  quad.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "quad.h"
#include "vbo.h"
#include "ibo.h"
#include "vao.h"

namespace gb
{
    quad::quad(void) :
    m_vbo(nullptr),
    m_ibo(nullptr),
    m_max_bound(glm::vec3(-4096.0f, -4096.0f, -4096.0f)),
    m_min_bound(glm::vec3( 4096.0f,  4096.0f,  4096.0f))
    {
        m_vbo = std::make_shared<vbo>(4, GL_STATIC_DRAW);
        vbo::vertex_attribute * vertices = m_vbo->lock();
        
        vertices[0].m_position = glm::vec3(-1.0f, -1.0f, 0.0f);
        vertices[0].m_texcoord = glm::packUnorm2x16(glm::vec2(0.0f, 0.0f));
        vertices[1].m_position = glm::vec3(-1.0f, 1.0f, 0.0f);
        vertices[1].m_texcoord = glm::packUnorm2x16(glm::vec2(0.0f, 1.0f));
        vertices[2].m_position = glm::vec3(1.0f, -1.0f, 0.0f);
        vertices[2].m_texcoord = glm::packUnorm2x16(glm::vec2(1.0f, 0.0f));
        vertices[3].m_position = glm::vec3(1.0f, 1.0f, 0.0f);
        vertices[3].m_texcoord = glm::packUnorm2x16(glm::vec2(1.0f, 1.0f));
        m_vbo->unlock();
        
        m_ibo = std::make_shared<ibo>(6, GL_STATIC_DRAW);
        ui16* indices = m_ibo->lock();
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 1;
        indices[4] = 2;
        indices[5] = 3;
        m_ibo->unlock();
    }
    
    quad::~quad(void)
    {
        m_vao_states.clear();
    }
    
    void quad::bind(const std::string& attributes_guid, const std::array<i32, e_shader_attribute_max>& attributes)
    {
        assert(m_vbo != nullptr);
        assert(m_ibo != nullptr);
        assert(attributes_guid.length() != 0);
        
        std::shared_ptr<vao> vao_state = m_vao_states[attributes_guid];
        if(!vao_state)
        {
            vao_state = std::make_shared<vao>(m_vbo, m_ibo);
            vao_state->init(attributes);
            m_vao_states[attributes_guid] = vao_state;
        }
        vao::bind(vao_state);
    }
    
    void quad::draw(void)
    {
        assert(m_vbo != nullptr);
        assert(m_ibo != nullptr);
        gl_draw_elements(GL_TRIANGLES, m_ibo->get_used_size(), GL_UNSIGNED_SHORT, NULL);
    }
    
    void quad::unbind(const std::string& attributes_guid, const std::array<i32, e_shader_attribute_max>& attributes)
    {
        assert(m_vbo != nullptr);
        assert(m_ibo != nullptr);
        vao::unbind();
    }
}