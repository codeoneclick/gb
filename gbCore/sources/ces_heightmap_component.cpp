//
//  ces_heightmap_component.cpp
//  gbCore
//
//  Created by sergey.sergeev on 11/9/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#include "ces_heightmap_component.h"
#include "heightmap_accessor.h"
#include "heightmap_chunk.h"
#include "scene_graph.h"
#include "camera.h"
#include "frustum.h"
#include "resource_accessor.h"
#include "texture.h"

namespace gb
{
    ces_heightmap_component::ces_heightmap_component(const std::string& filename) :
    m_filename(filename),
    m_generation_state(e_heightmap_generation_state_none),
    m_heightmap_accessor(std::make_shared<heightmap_accessor>())
    {
        m_type = e_ces_component_type_heightmap;
    }
    
    ces_heightmap_component::~ces_heightmap_component()
    {
        m_materials.clear();
    }
    
    ces_heightmap_component::e_heightmap_generation_state ces_heightmap_component::get_generation_state() const
    {
        return m_generation_state;
    }
    
    heightmap_container::e_heigtmap_chunk_lod ces_heightmap_component::get_current_lod(const glm::vec3& position,
                                                                                       const glm::vec3& min_bound,
                                                                                       const glm::vec3& max_bound) const
    {
        glm::vec2 center = glm::vec2((max_bound.x - min_bound.x) / 2.f + min_bound.x,
                                     (max_bound.z - min_bound.z) / 2.f + min_bound.z);
        f32 distance = glm::distance(glm::vec2(position.x, position.z), center);
        heightmap_container::e_heigtmap_chunk_lod lod = heightmap_container::e_heigtmap_chunk_lod_04;
        if(distance < 64.f)
        {
            lod = heightmap_container::e_heigtmap_chunk_lod_01;
        }
        else if(distance < 192.f)
        {
            lod = heightmap_container::e_heigtmap_chunk_lod_02;
        }
        else if(distance < 256.f)
        {
            lod = heightmap_container::e_heigtmap_chunk_lod_03;
        }
        return lod;
    }
    
    void ces_heightmap_component::generate(const graphics_context_shared_ptr& graphics_context,
                                           const resource_accessor_shared_ptr& resource_accessor)
    {
        m_generation_state = e_heightmap_generation_state_generating;
        m_heightmap_accessor->generate(m_filename, graphics_context, m_splatting_diffuse_textures, m_splatting_normal_textures, m_splatting_displace_textures,
                                       [this, resource_accessor] {
            
            i32 chunks_num = m_heightmap_accessor->get_chunks_num().x * m_heightmap_accessor->get_chunks_num().y;
            m_chunks.resize(chunks_num);
            
            for(i32 i = 0; i < chunks_num; ++i)
            {
                m_chunks[i] = std::make_shared<heightmap_chunk>();
                for(const auto& material : m_materials)
                {
                    m_chunks[i]->add_material(std::get<0>(material), std::get<1>(material), std::get<2>(material));
                    ces_base_component::get_scene_graph()->add_game_object(m_chunks[i]);
                }
                m_chunks[i]->create_bind_material_imposer_link();
            }
            
            resource_accessor->add_custom_resource("heightmap.deep.texture", m_heightmap_accessor->get_deep_texture());
            m_generation_state = e_heightmap_generation_state_generated;
        });
    }
    
    void ces_heightmap_component::update(f32 deltatime)
    {
        for(i32 i = 0; i < m_heightmap_accessor->get_chunks_num().x; ++i)
        {
            for(i32 j = 0; j < m_heightmap_accessor->get_chunks_num().y; ++j)
            {
                i32 index = i + j * m_heightmap_accessor->get_chunks_num().x;
                
                glm::vec3 position = m_chunks[index]->get_position();
                
                glm::vec3 min_bound = std::get<0>(m_heightmap_accessor->get_chunk_bounds(i, j)) + position;
                glm::vec3 max_bound = std::get<1>(m_heightmap_accessor->get_chunk_bounds(i, j)) + position;
                
                camera_shared_ptr camera = ces_base_component::get_scene_graph()->get_camera();
                frustum_shared_ptr frustum = camera->get_frustum();
                
                e_frustum_bound_result result = frustum->is_bounding_box_in_frustum(min_bound, max_bound);
                
                if(result == e_frustum_bound_result_inside || result == e_frustum_bound_result_intersect)
                {
                    m_chunks[index]->set_visible(true);
                    
                    heightmap_container::e_heigtmap_chunk_lod lod = ces_heightmap_component::get_current_lod(camera->get_look_at(), min_bound, max_bound);
                    
                    if(m_chunks[index]->get_inprogress_lod() == m_chunks[index]->get_current_lod() &&
                       m_chunks[index]->get_current_lod() != lod)
                    {
                        m_chunks[index]->set_inprogress_lod(lod);
                        
                        m_heightmap_accessor->start_chunk_loading(i, j, lod, [this, index] (const mesh_shared_ptr& mesh) {
                            m_chunks[index]->set_mesh(mesh);
                        }, [this, index, lod] (const texture_shared_ptr& diffuse_texture, const texture_shared_ptr& normal_displace_texture) {
                            m_chunks[index]->set_diffuse_texture(diffuse_texture);
                            m_chunks[index]->set_normal_displace_texture(normal_displace_texture);
                            m_chunks[index]->set_current_lod(lod);
                        });
                    }
                }
                else
                {
                    m_chunks[index]->set_visible(false);
                }
            }
        }
    }
    
    void ces_heightmap_component::add_material(const std::string& technique_name, i32 technique_pass, const material_shared_ptr& material)
    {
        m_materials.push_back(std::make_tuple(technique_name, technique_pass, material));
    }
    
    void ces_heightmap_component::set_position(const glm::vec3& position)
    {
        for(i32 i = 0; i < m_chunks.size(); ++i)
        {
            m_chunks[i]->set_position(position);
        }
    }
    
    glm::vec3 ces_heightmap_component::get_position() const
    {
        return m_chunks.size() > 0 && m_chunks.at(0) ? m_chunks.at(0)->get_position() : glm::vec3(0.f);
    }
    
    void ces_heightmap_component::set_splatting_diffuse_textures(const std::array<texture_shared_ptr, heightmap_texture_generator::e_splatting_texture_max>& splatting_diffuse_textures)
    {
        m_splatting_diffuse_textures[heightmap_texture_generator::e_splatting_texture_01] = splatting_diffuse_textures[heightmap_texture_generator::e_splatting_texture_01];
        m_splatting_diffuse_textures[heightmap_texture_generator::e_splatting_texture_02] = splatting_diffuse_textures[heightmap_texture_generator::e_splatting_texture_02];
        m_splatting_diffuse_textures[heightmap_texture_generator::e_splatting_texture_03] = splatting_diffuse_textures[heightmap_texture_generator::e_splatting_texture_03];
    }
    
    void ces_heightmap_component::set_splatting_normal_textures(const std::array<texture_shared_ptr, heightmap_texture_generator::e_splatting_texture_max>& splatting_normal_textures)
    {
        m_splatting_normal_textures[heightmap_texture_generator::e_splatting_texture_01] = splatting_normal_textures[heightmap_texture_generator::e_splatting_texture_01];
        m_splatting_normal_textures[heightmap_texture_generator::e_splatting_texture_02] = splatting_normal_textures[heightmap_texture_generator::e_splatting_texture_02];
        m_splatting_normal_textures[heightmap_texture_generator::e_splatting_texture_03] = splatting_normal_textures[heightmap_texture_generator::e_splatting_texture_03];
    }
    
    void ces_heightmap_component::set_splatting_displace_textures(const std::array<texture_shared_ptr, heightmap_texture_generator::e_splatting_texture_max>& splatting_displace_textures)
    {
        m_splatting_displace_textures[heightmap_texture_generator::e_splatting_texture_01] = splatting_displace_textures[heightmap_texture_generator::e_splatting_texture_01];
        m_splatting_displace_textures[heightmap_texture_generator::e_splatting_texture_02] = splatting_displace_textures[heightmap_texture_generator::e_splatting_texture_02];
        m_splatting_displace_textures[heightmap_texture_generator::e_splatting_texture_03] = splatting_displace_textures[heightmap_texture_generator::e_splatting_texture_03];
    }
    
    f32 ces_heightmap_component::get_height(const glm::vec3& position) const
    {
        return m_heightmap_accessor->get_height(position);
    }
    
    glm::vec2 ces_heightmap_component::get_angles_xz(const glm::vec3& position) const
    {
        return m_heightmap_accessor->get_angles_xz(position);
    }
}