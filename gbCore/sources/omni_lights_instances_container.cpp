//
//  omni_lights_instances_container.cpp
//  gbCore
//
//  Created by sergey.sergeev on 12/14/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#include "omni_lights_instances_container.h"
#include "shader.h"
#include "material.h"
#include "mesh_constructor.h"
#include "omni_light.h"
#include "instanced_mesh.h"

namespace gb
{
    static const std::string k_deffered_rendering_normal_texture = "ws.forward.rendering.normal.color";
    static const std::string k_deffered_rendering_depth_texture = "ws.base.depth";
    static const std::string k_deffered_rendering_material_name = "ws.deferred.lighting";
    
    const ui32 omni_lights_instances_container::k_max_lights_in_container = 64;
    
    std::once_flag g_instanced_omni_light_shader_created;
    omni_lights_instances_container::omni_lights_instances_container(const resource_accessor_shared_ptr& resource_accessor)
    {
        static shader_shared_ptr shader = nullptr;
        std::call_once(g_instanced_omni_light_shader_created, [this] {
            shader = shader::construct("omni_light",
                                       shader_instanced_omni_light_vert,
                                       shader_instanced_omni_light_frag);
            assert(shader);
        });
        
        m_material = std::make_shared<gb::material>();
        m_material->set_shader(shader);
        
        m_material->set_culling(false);
        m_material->set_culling_mode(GL_BACK);
        
        m_material->set_blending(true);
        m_material->set_blending_function_source(GL_ONE);
        m_material->set_blending_function_destination(GL_ONE);
        
        m_material->set_stencil_test(false);
        m_material->set_stencil_function(GL_ALWAYS);
        m_material->set_stencil_function_parameter_1(1);
        m_material->set_stencil_function_parameter_2(255);
        m_material->set_stencil_mask_parameter(255);
        
        m_material->set_depth_test(false);
        m_material->set_depth_mask(false);
        
        m_material->set_clipping(false);
        m_material->set_clipping_plane(glm::vec4(0.f));
        
        m_material->set_reflecting(false);
        m_material->set_shadowing(false);
        m_material->set_debugging(false);
        
        texture_shared_ptr texture_01 = resource_accessor->get_texture(k_deffered_rendering_normal_texture);
        assert(texture_01);
        texture_shared_ptr texture_02 = resource_accessor->get_texture(k_deffered_rendering_depth_texture);
        assert(texture_02);
        
        m_material->set_texture(texture_01, e_shader_sampler_01);
        m_material->set_texture(texture_02, e_shader_sampler_02);
        
        m_mesh = mesh_constructor::create_instanced_sphere(1.f, 16, 16);
        
        m_transformations = std::make_shared<std::vector<glm::vec4>>();
        m_colors = std::make_shared<std::vector<glm::vec4>>();
        
        m_transformations->resize(k_max_lights_in_container, glm::vec4(0.f, 0.f, 0.f, 1.f));
        m_colors->resize(k_max_lights_in_container, glm::vec4(1.f, 1.f, 1.f, 1.f));
    }
    
    omni_lights_instances_container::~omni_lights_instances_container()
    {
        
    }
    
    void omni_lights_instances_container::add_omni_light(const omni_light_shared_ptr& omni_light)
    {
        m_lights.insert(omni_light);
        
        omni_light->set_instance_id(static_cast<i32>(m_lights.size() - 1));
        omni_light->set_external_uniforms_data(m_transformations, m_colors);
        
        omni_light->add_material(k_deffered_rendering_material_name, 0, m_material);
        omni_light->set_mesh(m_mesh);
        
        m_mesh->set_num_instances(static_cast<i32>(m_lights.size()));
    }
    
    void omni_lights_instances_container::remove_omni_light(const omni_light_shared_ptr& omni_light)
    {
        m_lights.erase(omni_light);
    }
    
    ui32 omni_lights_instances_container::get_omni_lights_count() const
    {
        return static_cast<ui32>(m_lights.size());
    }
}