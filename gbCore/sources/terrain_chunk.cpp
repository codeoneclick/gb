//
//  terrain_chunk.cpp
//  gbCore
//
//  Created by sergey.sergeev on 11/9/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#include "terrain_chunk.h"
#include "ces_geometry_component.h"
#include "ces_render_component.h"

namespace gb
{
    terrain_chunk::terrain_chunk() :
    m_current_lod(terrain_container::e_heigtmap_chunk_lod_unknown),
    m_inprogress_lod(terrain_container::e_heigtmap_chunk_lod_unknown),
    m_diffuse_texture(nullptr),
    m_normal_displace_texture(nullptr)
    {
        ces_geometry_component_shared_ptr geometry_component = std::make_shared<ces_geometry_component>();
        ces_entity::add_component(geometry_component);
    }
    
    terrain_chunk::~terrain_chunk()
    {
        
    }
    
    void terrain_chunk::set_mesh(const mesh_shared_ptr& mesh)
    {
        unsafe_get_geometry_component_from_this->set_mesh(mesh);
    }
    
    terrain_container::e_heigtmap_chunk_lod terrain_chunk::get_current_lod() const
    {
        return m_current_lod;
    }
    
    terrain_container::e_heigtmap_chunk_lod terrain_chunk::get_inprogress_lod() const
    {
        return m_inprogress_lod;
    }
    
    void terrain_chunk::set_current_lod(terrain_container::e_heigtmap_chunk_lod lod)
    {
        m_current_lod = lod;
    }
    
    void terrain_chunk::set_inprogress_lod(terrain_container::e_heigtmap_chunk_lod lod)
    {
        m_inprogress_lod = lod;
    }
    
    void terrain_chunk::set_diffuse_texture(const texture_shared_ptr& texture)
    {
        m_diffuse_texture = texture;
    }
    
    void terrain_chunk::set_normal_displace_texture(const texture_shared_ptr& texture)
    {
        m_normal_displace_texture = texture;
    }
    
    void terrain_chunk::on_bind_material_imposer_callback(const material_shared_ptr& material)
    {
        if(m_diffuse_texture)
        {
            material->set_texture(m_diffuse_texture, e_shader_sampler_01);
        }
        if(m_normal_displace_texture)
        {
            material->set_texture(m_normal_displace_texture, e_shader_sampler_02);
        }
    }
    
    void terrain_chunk::create_bind_material_imposer_link()
    {
        unsafe_get_render_component_from_this->set_bind_material_imposer_callback(std::bind(&terrain_chunk::on_bind_material_imposer_callback,
                                                                                            this, std::placeholders::_1));
    }
}
