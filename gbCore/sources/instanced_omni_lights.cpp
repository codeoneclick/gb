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
        
        ces_render_component_shared_ptr render_component = std::make_shared<ces_render_component>();
        ces_entity::add_component(render_component);
        render_component->set_z_order(0);
        
        m_parameters.resize(num_instances, glm::vec4(0.f, 0.f, 0.f, 1.f));
    }
    
    instanced_omni_lights::~instanced_omni_lights()
    {
        
    }
    
    void instanced_omni_lights::set_position(const glm::vec3& position, i32 instance_id)
    {
        if(instance_id < m_num_instances)
        {
            m_parameters[instance_id].x = position.x;
            m_parameters[instance_id].y = position.y;
            m_parameters[instance_id].z = position.z;

            unsafe_get_render_component_from_this->set_custom_shader_uniform_array(&m_parameters[0], static_cast<i32>(m_parameters.size()),
                                                                                   "u_parameters", "ws.deferred.lighting");
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
            return glm::vec3(m_parameters[instance_id].x,
                             m_parameters[instance_id].y,
                             m_parameters[instance_id].z);
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
            m_parameters[instance_id].w = radius;
            unsafe_get_render_component_from_this->set_custom_shader_uniform_array(&m_parameters[0], static_cast<i32>(m_parameters.size()),
                                                                                   "u_parameters", "ws.deferred.lighting");
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
            return m_parameters[instance_id].w;
        }
        else
        {
            assert(false);
            return 0.f;
        }
    }
    
    void instanced_omni_lights::set_scene_graph(const scene_graph_shared_ptr& scene_graph)
    {
        m_scene_graph = scene_graph;
    }
    
    scene_graph_shared_ptr instanced_omni_lights::get_scene_graph() const
    {
        return m_scene_graph.lock();
    }
    
    void instanced_omni_lights::on_added_to_scene(const scene_graph_shared_ptr& scene_graph)
    {
        instanced_omni_lights::set_scene_graph(scene_graph);
        for(const auto& component : ces_entity::get_components())
        {
            if(component != nullptr)
            {
                component->set_scene_graph(scene_graph);
            }
        }
    }
    
    void instanced_omni_lights::on_removed_from_scene()
    {
        instanced_omni_lights::set_scene_graph(nullptr);
        for(const auto& component : ces_entity::get_components())
        {
            component->set_scene_graph(nullptr);
        }
    }
    
    void instanced_omni_lights::add_material(const std::string& technique_name, const material_shared_ptr& material)
    {
        unsafe_get_render_component_from_this->add_material(technique_name, material);
    }
    
    void instanced_omni_lights::remove_material(const std::string& technique_name)
    {
        unsafe_get_render_component_from_this->remove_material(technique_name);
    }
    
    material_shared_ptr instanced_omni_lights::get_material(const std::string& technique_name) const
    {
        return unsafe_get_render_component_from_this->get_material(technique_name);
    }
    
    void instanced_omni_lights::set_mesh(const instanced_mesh_shared_ptr& mesh)
    {
        unsafe_get_instanced_geometry_component_from_this->set_mesh(mesh);
    }
}