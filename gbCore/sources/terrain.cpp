//
//  terrain.cpp
//  gbCore
//
//  Created by sergey.sergeev on 11/9/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#include "terrain.h"
#include "ces_terrain_component.h"

namespace gb
{
    terrain::terrain(const std::string& filename)
    {
        ces_terrain_component_shared_ptr terrain_component = std::make_shared<ces_terrain_component>(filename);
        ces_entity::add_component(terrain_component);
    }
    
    terrain::~terrain()
    {
        
    }
    
    void terrain::set_scene_graph(const scene_graph_shared_ptr& scene_graph)
    {
        m_scene_graph = scene_graph;
    }
    
    scene_graph_shared_ptr terrain::get_scene_graph() const
    {
        return m_scene_graph.lock();
    }
    
    void terrain::on_added_to_scene(const scene_graph_shared_ptr& scene_graph)
    {
        terrain::set_scene_graph(scene_graph);
        for(const auto& component : ces_entity::get_components())
        {
            if(component != nullptr)
            {
                component->set_scene_graph(scene_graph);
            }
        }
    }
    
    void terrain::on_removed_from_scene()
    {
        terrain::set_scene_graph(nullptr);
        for(const auto& component : ces_entity::get_components())
        {
            component->set_scene_graph(nullptr);
        }
    }
    
    void terrain::add_material(const std::string& technique_name, i32 technique_pass, const material_shared_ptr& material)
    {
        unsafe_get_terrain_component_from_this->add_material(technique_name, technique_pass, material);
    }
    
    void terrain::set_position(const glm::vec3& position)
    {
        unsafe_get_terrain_component_from_this->set_position(position);
    }
    
    glm::vec3 terrain::get_position() const
    {
        return unsafe_get_terrain_component_from_this->get_position();
    }
    
    void terrain::set_splatting_diffuse_textures(const std::array<texture_shared_ptr, terrain_texture_generator::e_splatting_texture_max>& splatting_diffuse_textures)
    {
        unsafe_get_terrain_component_from_this->set_splatting_diffuse_textures(splatting_diffuse_textures);
    }
    
    void terrain::set_splatting_normal_textures(const std::array<texture_shared_ptr, terrain_texture_generator::e_splatting_texture_max>& splatting_normal_textures)
    {
        unsafe_get_terrain_component_from_this->set_splatting_normal_textures(splatting_normal_textures);
    }
    
    void terrain::set_splatting_displace_textures(const std::array<texture_shared_ptr, terrain_texture_generator::e_splatting_texture_max>& splatting_displace_textures)
    {
        unsafe_get_terrain_component_from_this->set_splatting_displace_textures(splatting_displace_textures);
    }
    
    f32 terrain::get_height(const glm::vec3& position) const
    {
        return unsafe_get_terrain_component_from_this->get_height(position - terrain::get_position());
    }
    
    glm::vec2 terrain::get_angles_xz(const glm::vec3& position) const
    {
        return unsafe_get_terrain_component_from_this->get_angles_xz(position - terrain::get_position());
    }
}