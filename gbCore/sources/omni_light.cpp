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
        
        ces_render_component_shared_ptr render_component = std::make_shared<ces_render_component>();
        ces_entity::add_component(render_component);
        render_component->set_z_order(0);
    }
    
    omni_light::~omni_light()
    {
        
    }
    
    void omni_light::set_position(const glm::vec3& position)
    {
        unsafe_get_transformation_component_from_this->set_position(position);
        unsafe_get_render_component_from_this->set_custom_shader_uniform(position,
                                                                         "u_light_position",
                                                                         "ws.deferred.lighting");

    }
    
    glm::vec3 omni_light::get_position() const
    {
        return unsafe_get_transformation_component_from_this->get_position();
    }
    
    void omni_light::set_radius(f32 radius)
    {
        m_radius = radius;
        unsafe_get_render_component_from_this->set_custom_shader_uniform(m_radius,
                                                                         "u_light_radius",
                                                                         "ws.deferred.lighting");
        unsafe_get_transformation_component_from_this->set_scale(glm::vec3(radius));
    }
    
    f32 omni_light::get_radius() const
    {
        return m_radius;
    }
    
    void omni_light::set_scene_graph(const scene_graph_shared_ptr& scene_graph)
    {
        m_scene_graph = scene_graph;
    }
    
    scene_graph_shared_ptr omni_light::get_scene_graph() const
    {
        return m_scene_graph.lock();
    }
    
    void omni_light::on_added_to_scene(const scene_graph_shared_ptr& scene_graph)
    {
        omni_light::set_scene_graph(scene_graph);
        for(const auto& component : ces_entity::get_components())
        {
            if(component != nullptr)
            {
                component->set_scene_graph(scene_graph);
            }
        }
    }
    
    void omni_light::on_removed_from_scene()
    {
        omni_light::set_scene_graph(nullptr);
        for(const auto& component : ces_entity::get_components())
        {
            component->set_scene_graph(nullptr);
        }
    }
    
    void omni_light::add_material(const std::string& technique_name, const material_shared_ptr& material)
    {
        material->set_custom_shader_uniform(unsafe_get_transformation_component_from_this->get_position(), "u_light_position");
        material->set_custom_shader_uniform(m_radius, "u_light_radius");
        
        unsafe_get_render_component_from_this->add_material(technique_name, material);
    }
    
    void omni_light::remove_material(const std::string& technique_name)
    {
        unsafe_get_render_component_from_this->remove_material(technique_name);
    }
    
    material_shared_ptr omni_light::get_material(const std::string& technique_name) const
    {
        return unsafe_get_render_component_from_this->get_material(technique_name);
    }
    
    void omni_light::set_mesh(const mesh_shared_ptr& mesh)
    {
        unsafe_get_geometry_component_from_this->set_mesh(mesh);
    }
}