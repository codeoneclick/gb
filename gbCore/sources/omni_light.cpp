//
//  omni_light.cpp
//  gbCore
//
//  Created by sergey.sergeev on 9/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "omni_light.h"
#include "ces_transformation_component.h"
#include "ces_instanced_geometry_component.h"
#include "ces_render_component.h"
#include "instanced_mesh.h"
#include "material.h"

namespace gb
{
    omni_light::omni_light() :
    m_transform_parameters_ref(nullptr),
    m_colors_ref(nullptr),
    m_instance_id(-1)
    {
        ces_transformation_component_shared_ptr transformation_component = std::make_shared<ces_transformation_component>();
        ces_entity::add_component(transformation_component);
        
        ces_instanced_geometry_component_shared_ptr geometry_component = std::make_shared<ces_instanced_geometry_component>();
        ces_entity::add_component(geometry_component);
    }
    
    omni_light::~omni_light()
    {
        
    }
    
    void omni_light::set_instance_id(i32 instance_id)
    {
        m_instance_id = instance_id;
    }
    
    void omni_light::set_parameters_ref(std::vector<glm::vec4>* transform_parameters_ref,
                                        std::vector<glm::vec4>* colors_ref)
    {
        m_transform_parameters_ref = transform_parameters_ref;
        m_colors_ref = colors_ref;
    }
    
    void omni_light::set_position(const glm::vec3& position)
    {
        if(m_instance_id != -1 && m_transform_parameters_ref->size() > m_instance_id)
        {
            (*m_transform_parameters_ref)[m_instance_id].x = position.x;
            (*m_transform_parameters_ref)[m_instance_id].y = position.y;
            (*m_transform_parameters_ref)[m_instance_id].z = position.z;
            
            unsafe_get_render_component_from_this->set_custom_shader_uniform_array(&(*m_transform_parameters_ref)[0],
                                                                                   static_cast<i32>((*m_transform_parameters_ref).size()),
                                                                                   "u_transform_parameters");
        }
        else
        {
            assert(false);
        }
    }
    
    glm::vec3 omni_light::get_position() const
    {
        if(m_instance_id != -1 && m_transform_parameters_ref->size() > m_instance_id)
        {
            return glm::vec3((*m_transform_parameters_ref)[m_instance_id].x,
                             (*m_transform_parameters_ref)[m_instance_id].y,
                             (*m_transform_parameters_ref)[m_instance_id].z);
        }

        assert(false);
        return glm::vec3(0.f);
    }
    
    void omni_light::set_radius(f32 radius)
    {
        if(m_instance_id != -1 && m_transform_parameters_ref->size() > m_instance_id)
        {
            (*m_transform_parameters_ref)[m_instance_id].w = radius;
            unsafe_get_render_component_from_this->set_custom_shader_uniform_array(&(*m_transform_parameters_ref)[0],
                                                                                   static_cast<i32>((*m_transform_parameters_ref).size()),
                                                                                   "u_transform_parameters");
        }
        else
        {
            assert(false);
        }
    }
    
    f32 omni_light::get_radius() const
    {
        if(m_instance_id != -1 && m_transform_parameters_ref->size() > m_instance_id)
        {
            return (*m_transform_parameters_ref)[m_instance_id].w;
        }

        assert(false);
        return 0.f;
    }
    
    void omni_light::set_color(const glm::vec4& color)
    {
        if(m_instance_id != -1 && m_colors_ref->size() > m_instance_id)
        {
            (*m_colors_ref)[m_instance_id] = color;
            unsafe_get_render_component_from_this->set_custom_shader_uniform_array(&(*m_colors_ref)[0],
                                                                                   static_cast<i32>((*m_colors_ref).size()),
                                                                                   "u_lights_colors");
        }
        else
        {
            assert(false);
        }
    }
    
    glm::vec4 omni_light::get_color() const
    {
        if(m_instance_id != -1 && m_colors_ref->size() > m_instance_id)
        {
            return (*m_colors_ref)[m_instance_id];
        }
        else
        {
            assert(false);
            return glm::vec4(0.f);
        }
    }
    
    void omni_light::add_material(const std::string& technique_name, i32 technique_pass, const material_shared_ptr& material)
    {
        renderable_interface::add_material(technique_name, technique_pass, material);
        unsafe_get_render_component_from_this->set_custom_shader_uniform_array(&(*m_transform_parameters_ref)[0],
                                                                               static_cast<i32>((*m_transform_parameters_ref).size()),
                                                                               "u_transform_parameters");
        unsafe_get_render_component_from_this->set_custom_shader_uniform_array(&(*m_colors_ref)[0],
                                                                               static_cast<i32>((*m_colors_ref).size()),
                                                                               "u_lights_colors");
    }
    
    void omni_light::set_mesh(const instanced_mesh_shared_ptr& mesh)
    {
        unsafe_get_instanced_geometry_component_from_this->set_mesh(mesh);
    }
}