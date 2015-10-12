//
//  instanced_omni_lights.cpp
//  gbCore
//
//  Created by sergey.sergeev on 10/1/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "instanced_omni_lights.h"
#include "ces_transformation_component.h"
#include "ces_instanced_geometry_component.h"
#include "ces_render_component.h"
#include "material.h"
#include "instanced_mesh.h"

namespace gb
{
    instanced_omni_lights::instanced_omni_lights(i32 num_instances) :
    m_num_instances(num_instances)
    {
        ces_transformation_component_shared_ptr transformation_component = std::make_shared<ces_transformation_component>();
        ces_entity::add_component(transformation_component);
        
        ces_instanced_geometry_component_shared_ptr geometry_component = std::make_shared<ces_instanced_geometry_component>();
        ces_entity::add_component(geometry_component);
        
        m_transform_parameters.resize(num_instances, glm::vec4(0.f, 0.f, 0.f, 1.f));
        m_colors.resize(num_instances, glm::vec4(1.f, 1.f, 1.f, 1.f));
    }
    
    instanced_omni_lights::~instanced_omni_lights()
    {
        
    }
    
    void instanced_omni_lights::set_position(const glm::vec3& position, i32 instance_id)
    {
        if(instance_id < m_num_instances)
        {
            m_transform_parameters[instance_id].x = position.x;
            m_transform_parameters[instance_id].y = position.y;
            m_transform_parameters[instance_id].z = position.z;

            unsafe_get_render_component_from_this->set_custom_shader_uniform_array(&m_transform_parameters[0], static_cast<i32>(m_transform_parameters.size()),
                                                                                   "u_transform_parameters");
        }
        else
        {
            assert(false);
        }
    }
    
    glm::vec3 instanced_omni_lights::get_position(i32 instance_id) const
    {
        if(instance_id < m_num_instances)
        {
            return glm::vec3(m_transform_parameters[instance_id].x,
                             m_transform_parameters[instance_id].y,
                             m_transform_parameters[instance_id].z);
        }
        else
        {
            assert(false);
            return glm::vec3(0.f);
        }
    }
    
    void instanced_omni_lights::set_radius(f32 radius, i32 instance_id)
    {
        if(instance_id < m_num_instances)
        {
            m_transform_parameters[instance_id].w = radius;
            unsafe_get_render_component_from_this->set_custom_shader_uniform_array(&m_transform_parameters[0], static_cast<i32>(m_transform_parameters.size()),
                                                                                   "u_transform_parameters");
        }
        else
        {
            assert(false);
        }
    }
    
    f32 instanced_omni_lights::get_radius(i32 instance_id) const
    {
        if(instance_id < m_num_instances)
        {
            return m_transform_parameters[instance_id].w;
        }
        else
        {
            assert(false);
            return 0.f;
        }
    }
    
    void instanced_omni_lights::set_color(const glm::vec4& color, i32 instance_id)
    {
        if(instance_id < m_num_instances)
        {
            m_colors[instance_id] = color;
            unsafe_get_render_component_from_this->set_custom_shader_uniform_array(&m_colors[0], static_cast<i32>(m_colors.size()),
                                                                                   "u_lights_colors");
        }
        else
        {
            assert(false);
        }

    }
    
    glm::vec4 instanced_omni_lights::get_color(i32 instance_id) const
    {
        if(instance_id < m_num_instances)
        {
            return m_colors[instance_id];
        }
        else
        {
            assert(false);
            return glm::vec4(0.f);
        }
    }
    
    void instanced_omni_lights::add_material(const std::string& technique_name, i32 technique_pass, const material_shared_ptr& material)
    {
        renderable_interface::add_material(technique_name, technique_pass, material);
        unsafe_get_render_component_from_this->set_custom_shader_uniform_array(&m_transform_parameters[0], static_cast<i32>(m_transform_parameters.size()),
                                                                               "u_transform_parameters");
        unsafe_get_render_component_from_this->set_custom_shader_uniform_array(&m_colors[0], static_cast<i32>(m_colors.size()),
                                                                               "u_lights_colors");

    }
    
    void instanced_omni_lights::set_mesh(const instanced_mesh_shared_ptr& mesh)
    {
        unsafe_get_instanced_geometry_component_from_this->set_mesh(mesh);
    }
}