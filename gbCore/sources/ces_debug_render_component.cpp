//
//  ces_debug_render_component.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/31/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "ces_debug_render_component.h"
#include "material.h"
#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "built_in_shaders.h"

namespace gb
{
    ces_debug_render_component::ces_debug_render_component(void) :
    m_mesh(nullptr)
    {
        m_type = e_ces_component_type_debug_render;
        
        m_material = std::make_shared<material>();
        shader_shared_ptr shader = shader::construct("bounding_box",
                                                     shader_bounding_box_vert,
                                                     shader_bounding_box_frag);
        assert(shader);
        m_material->set_shader(shader);
        m_material->set_culling(false);
        m_material->set_culling_mode(GL_BACK);
        m_material->set_blending(false);
        m_material->set_blending_function_source(GL_SRC_ALPHA);
        m_material->set_blending_function_destination(GL_ONE);
        m_material->set_depth_test(true);
        m_material->set_depth_mask(true);
        m_material->set_clipping(false);
        m_material->set_clipping_plane(glm::vec4(0.f));
        m_material->set_reflecting(false);
        m_material->set_shadowing(false);
        m_material->set_debugging(false);
    }
    
    ces_debug_render_component::~ces_debug_render_component(void)
    {
        
    }
    
    material_shared_ptr ces_debug_render_component::on_bind(const std::string& technique_name)
    {
        if(m_mesh)
        {
            assert(m_material);
            m_material->bind();
        }
        return m_material;
    }
    
    void ces_debug_render_component::on_draw(const std::string& technique_name)
    {
        if(m_mesh)
        {
            assert(m_material);
            assert(m_material->get_shader()->is_commited());
            assert(m_mesh->is_commited());
            
            m_mesh->bind(m_material->get_shader()->get_guid(), m_material->get_shader()->get_attributes());
            m_mesh->draw();
            m_mesh->unbind(m_material->get_shader()->get_guid(), m_material->get_shader()->get_attributes());
        }
    }
    
    void ces_debug_render_component::on_unbind(const std::string& technique_name)
    {
        if(m_mesh)
        {
            assert(m_material);
            m_material->unbind();
        }
    }
    
    void ces_debug_render_component::set_mesh(const mesh_shared_ptr& mesh)
    {
        m_mesh = mesh;
    }
}