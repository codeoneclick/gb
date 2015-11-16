//
//  heightmap.cpp
//  gbCore
//
//  Created by sergey.sergeev on 11/9/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#include "heightmap.h"
#include "ces_heightmap_component.h"

namespace gb
{
    heightmap::heightmap(const std::string& filename)
    {
        ces_heightmap_component_shared_ptr heightmap_component = std::make_shared<ces_heightmap_component>(filename);
        ces_entity::add_component(heightmap_component);
    }
    
    heightmap::~heightmap()
    {
        
    }
    
    void heightmap::set_scene_graph(const scene_graph_shared_ptr& scene_graph)
    {
        m_scene_graph = scene_graph;
    }
    
    scene_graph_shared_ptr heightmap::get_scene_graph() const
    {
        return m_scene_graph.lock();
    }
    
    void heightmap::on_added_to_scene(const scene_graph_shared_ptr& scene_graph)
    {
        heightmap::set_scene_graph(scene_graph);
        for(const auto& component : ces_entity::get_components())
        {
            if(component != nullptr)
            {
                component->set_scene_graph(scene_graph);
            }
        }
    }
    
    void heightmap::on_removed_from_scene()
    {
        heightmap::set_scene_graph(nullptr);
        for(const auto& component : ces_entity::get_components())
        {
            component->set_scene_graph(nullptr);
        }
    }
    
    void heightmap::add_material(const std::string& technique_name, i32 technique_pass, const material_shared_ptr& material)
    {
        unsafe_get_heightmap_component_from_this->add_material(technique_name, technique_pass, material);
    }
    
    void heightmap::set_position(const glm::vec3& position)
    {
        unsafe_get_heightmap_component_from_this->set_position(position);
    }
    
    glm::vec3 heightmap::get_position() const
    {
        return unsafe_get_heightmap_component_from_this->get_position();
    }
    
    void heightmap::set_splatting_diffuse_textures(const std::array<texture_shared_ptr, heightmap_texture_generator::e_splatting_texture_max>& splatting_diffuse_textures)
    {
        unsafe_get_heightmap_component_from_this->set_splatting_diffuse_textures(splatting_diffuse_textures);
    }
    
    void heightmap::set_splatting_normal_textures(const std::array<texture_shared_ptr, heightmap_texture_generator::e_splatting_texture_max>& splatting_normal_textures)
    {
        unsafe_get_heightmap_component_from_this->set_splatting_normal_textures(splatting_normal_textures);
    }
    
    void heightmap::set_splatting_displace_textures(const std::array<texture_shared_ptr, heightmap_texture_generator::e_splatting_texture_max>& splatting_displace_textures)
    {
        unsafe_get_heightmap_component_from_this->set_splatting_displace_textures(splatting_displace_textures);
    }
    
    f32 heightmap::get_height(const glm::vec3& position) const
    {
        return unsafe_get_heightmap_component_from_this->get_height(position - heightmap::get_position());
    }
    
    glm::vec2 heightmap::get_angles_xz(const glm::vec3& position) const
    {
        return unsafe_get_heightmap_component_from_this->get_angles_xz(position - heightmap::get_position());
    }
}