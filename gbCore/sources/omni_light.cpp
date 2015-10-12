//
//  omni_light.cpp
//  gbCore
//
//  Created by sergey.sergeev on 9/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "omni_light.h"
#include "ces_transformation_component.h"
#include "ces_geometry_component.h"
#include "ces_render_component.h"
#include "material.h"

namespace gb
{
    omni_light::omni_light() :
    m_radius(1.f)
    {
        ces_transformation_component_shared_ptr transformation_component = std::make_shared<ces_transformation_component>();
        ces_entity::add_component(transformation_component);
        
        ces_geometry_component_shared_ptr geometry_component = std::make_shared<ces_geometry_component>();
        ces_entity::add_component(geometry_component);
    }
    
    omni_light::~omni_light()
    {
        
    }
    
    void omni_light::set_position(const glm::vec3& position)
    {
        unsafe_get_transformation_component_from_this->set_position(position);
        unsafe_get_render_component_from_this->set_custom_shader_uniform(position, "u_light_position");
    }
    
    glm::vec3 omni_light::get_position() const
    {
        return unsafe_get_transformation_component_from_this->get_position();
    }
    
    void omni_light::set_radius(f32 radius)
    {
        m_radius = radius;
        unsafe_get_render_component_from_this->set_custom_shader_uniform(m_radius, "u_light_radius");
        unsafe_get_transformation_component_from_this->set_scale(glm::vec3(m_radius));
    }
    
    f32 omni_light::get_radius() const
    {
        return m_radius;
    }
    
    void omni_light::add_material(const std::string& technique_name, i32 technique_pass, const material_shared_ptr& material)
    {
        renderable_interface::add_material(technique_name, technique_pass, material);
        unsafe_get_render_component_from_this->set_custom_shader_uniform(unsafe_get_transformation_component_from_this->get_position(), "u_light_position");
        unsafe_get_render_component_from_this->set_custom_shader_uniform(m_radius, "u_light_radius");
    }
    
    void omni_light::set_mesh(const mesh_shared_ptr& mesh)
    {
        unsafe_get_geometry_component_from_this->set_mesh(mesh);
    }
}