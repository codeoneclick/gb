//
//  heightmap_container.cpp
//  gbCore
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "heightmap_container.h"
#include "heightmap_loader.h"
#include "heightmap_texture_generator.h"
#include "texture.h"

namespace gb
{
    const f32 heightmap_container::k_max_height = 32.f;
    const f32 heightmap_container::k_raise = 32.f;
    const f32 heightmap_container::k_deep = heightmap_container::k_raise * .25f;
    
    const f32 heightmap_container::k_layer_section_01 = .33f;
    const f32 heightmap_container::k_layer_section_02 = .66f;
    const f32 heightmap_container::k_layer_section_offset = .11f;
    
    const i32 heightmap_container::k_max_chunk_size = 65;
    
    static const std::string k_deep_texture_name = "deep.texture";
    
    heightmap_container::heightmap_container() :
    m_uncompressed_vertices(nullptr),
    m_compressed_vertices(nullptr),
    m_faces(nullptr),
    m_uncompressed_vertices_mmap_descriptor(nullptr),
    m_compressed_vertices_mmap_descriptor(nullptr),
    m_faces_mmap_descriptor(nullptr),
    m_vbos_mmap_descriptor(nullptr),
    m_ibos_mmap_descriptor(nullptr),
    m_splatting_mask_textures_mmap_descriptor(nullptr),
    m_splatting_diffuse_textures_mmap_descriptor(nullptr),
    m_splatting_normal_textures_mmap_descriptor(nullptr),
    m_splatting_displace_textures_mmap_descriptor(nullptr),
    m_main_size(0),
    m_deep_texture(nullptr)
    {
        
    }
    
    heightmap_container::~heightmap_container()
    {
        heightmap_container::erase_geometry();
        heightmap_container::erase_mask_textures();
        heightmap_container::erase_diffuse_textures();
        heightmap_container::erase_normal_textures();
        heightmap_container::erase_displace_textures();
    }
    
    void heightmap_container::init(const glm::ivec2& size)
    {
        m_main_size = size;
        
        m_chunk_size = glm::ivec2(std::min(size.x, k_max_chunk_size),
                                  std::min(size.y, k_max_chunk_size));
        
        m_chunk_lods_sizes.at(e_heigtmap_chunk_lod_01) = glm::ivec2(std::min(size.x, m_chunk_size.x),
                                                                    std::min(size.y, m_chunk_size.y));
        
        m_chunk_lods_sizes.at(e_heigtmap_chunk_lod_02) = glm::ivec2(std::min(size.x, static_cast<i32>(floor(static_cast<f32>(k_max_chunk_size) / 2.f))),
                                                                    std::min(size.y, static_cast<i32>(floor(static_cast<f32>(k_max_chunk_size) / 2.f))));
        
        m_chunk_lods_sizes.at(e_heigtmap_chunk_lod_03) = glm::ivec2(std::min(size.x, static_cast<i32>(ceil(static_cast<f32>(k_max_chunk_size) / 4.f))),
                                                                    std::min(size.y, static_cast<i32>(ceil(static_cast<f32>(k_max_chunk_size) / 4.f))));
        
        m_chunk_lods_sizes.at(e_heigtmap_chunk_lod_04) = glm::ivec2(std::min(size.x, static_cast<i32>(floor(static_cast<f32>(k_max_chunk_size) / 8.f))),
                                                                    std::min(size.y, static_cast<i32>(floor(static_cast<f32>(k_max_chunk_size) / 8.f))));
        
        m_textures_lods_sizes.at(e_heigtmap_chunk_lod_01) = heightmap_texture_generator::k_splatting_texture_size_lod_01;
        m_textures_lods_sizes.at(e_heigtmap_chunk_lod_02) = heightmap_texture_generator::k_splatting_texture_size_lod_02;
        m_textures_lods_sizes.at(e_heigtmap_chunk_lod_03) = heightmap_texture_generator::k_splatting_texture_size_lod_03;
        m_textures_lods_sizes.at(e_heigtmap_chunk_lod_04) = heightmap_texture_generator::k_splatting_texture_size_lod_04;
        
        m_chunks_num = glm::ivec2(size.x / (m_chunk_size.x - 1),
                                  size.y / (m_chunk_size.y - 1));
        
        m_uncompressed_vertices = new uncompressed_vertex[m_main_size.x * m_main_size.y];
        m_compressed_vertices = new compressed_vertex[m_main_size.x * m_main_size.y];
        m_faces = new face[(m_main_size.x - 1) * (m_main_size.y - 1) * 2];
        
        m_vbos_mmap.resize(m_chunks_num.x * m_chunks_num.y);
        m_ibos_mmap.resize(m_chunks_num.x * m_chunks_num.y);
        
        m_splatting_mask_textures_mmap.resize(m_chunks_num.x * m_chunks_num.y);
        m_splatting_diffuse_textures_mmap.resize(m_chunks_num.x * m_chunks_num.y);
        m_splatting_normal_textures_mmap.resize(m_chunks_num.x * m_chunks_num.y);
        m_splatting_displace_textures_mmap.resize(m_chunks_num.x * m_chunks_num.y);
    }
    
    void heightmap_container::erase_geometry()
    {
        if(m_uncompressed_vertices_mmap_descriptor != 0)
        {
            m_uncompressed_vertices_mmap_descriptor->deallocate();
            m_uncompressed_vertices_mmap_descriptor = nullptr;
        }
        else
        {
            delete [] m_uncompressed_vertices;
        }
        m_uncompressed_vertices = nullptr;
        
        if(m_compressed_vertices_mmap_descriptor != nullptr)
        {
            m_compressed_vertices_mmap_descriptor->deallocate();
            m_compressed_vertices_mmap_descriptor = nullptr;
        }
        else
        {
            delete [] m_compressed_vertices;
        }
        m_compressed_vertices = nullptr;
        
        if(m_faces_mmap_descriptor != nullptr)
        {
            m_faces_mmap_descriptor->deallocate();
            m_faces_mmap_descriptor = nullptr;
        }
        else
        {
            delete [] m_faces;
        }
        m_faces = nullptr;
        
        if(m_vbos_mmap_descriptor != nullptr)
        {
            m_vbos_mmap_descriptor->deallocate();
            m_vbos_mmap_descriptor = nullptr;
        }
        
        if(m_ibos_mmap_descriptor != nullptr)
        {
            m_ibos_mmap_descriptor->deallocate();
            m_ibos_mmap_descriptor = nullptr;
        }
    }
    
    void heightmap_container::erase_mask_textures()
    {
        if(m_splatting_mask_textures_mmap_descriptor)
        {
            m_splatting_mask_textures_mmap_descriptor->deallocate();
            m_splatting_mask_textures_mmap_descriptor = nullptr;
        }
    }
    
    void heightmap_container::erase_diffuse_textures()
    {
        if(m_splatting_diffuse_textures_mmap_descriptor)
        {
            m_splatting_diffuse_textures_mmap_descriptor->deallocate();
            m_splatting_diffuse_textures_mmap_descriptor = nullptr;
        }
    }
    
    void heightmap_container::erase_normal_textures()
    {
        if(m_splatting_normal_textures_mmap_descriptor)
        {
            m_splatting_normal_textures_mmap_descriptor->deallocate();
            m_splatting_normal_textures_mmap_descriptor = nullptr;
        }
    }
    
    void heightmap_container::erase_displace_textures()
    {
        if(m_splatting_displace_textures_mmap_descriptor)
        {
            m_splatting_displace_textures_mmap_descriptor->deallocate();
            m_splatting_displace_textures_mmap_descriptor = nullptr;
        }
    }
    
    void heightmap_container::mmap_geometry(const std::string& filename)
    {
        heightmap_container::erase_geometry();
        
        m_compressed_vertices_mmap_descriptor = std::make_shared<memory_map>();
        m_compressed_vertices = static_cast<compressed_vertex*>(m_compressed_vertices_mmap_descriptor->allocate(heightmap_loader::get_compressed_vertices_mmap_filename(filename)));
        if (!m_compressed_vertices)
        {
            assert(false);
        }
        
        m_uncompressed_vertices_mmap_descriptor = std::make_shared<memory_map>();
        m_uncompressed_vertices = static_cast<uncompressed_vertex*>(m_uncompressed_vertices_mmap_descriptor->allocate(heightmap_loader::get_uncompressed_vertices_mmap_filename(filename)));
        if (!m_uncompressed_vertices)
        {
            assert(false);
        }
        
        m_faces_mmap_descriptor = std::make_shared<memory_map>();
        m_faces = static_cast<face*>(m_faces_mmap_descriptor->allocate(heightmap_loader::get_faces_mmap_filename(filename)));
        if (!m_faces)
        {
            assert(false);
        }
        
        m_vbos_mmap_descriptor = std::make_shared<memory_map>();
        m_vbos_mmap_descriptor->allocate(heightmap_loader::get_vbos_mmap_filename(filename));
        
        ui32 offset = 0;
        for(ui32 i = 0; i < m_chunks_num.x; ++i)
        {
            for(ui32 j = 0; j < m_chunks_num.y; ++j)
            {
                m_vbos_mmap[i + j * m_chunks_num.x] = std::make_shared<heightmap_vbo_memory_map>(m_vbos_mmap_descriptor);
                m_vbos_mmap[i + j * m_chunks_num.x]->set_size(m_chunk_size.x * m_chunk_size.y);
                m_vbos_mmap[i + j * m_chunks_num.x]->set_offset(offset);
                offset += m_chunk_size.x * m_chunk_size.y;
            }
        }
        
        m_ibos_mmap_descriptor = std::make_shared<memory_map>();
        m_ibos_mmap_descriptor->allocate(heightmap_loader::get_ibos_mmap_filename(filename));
        
        offset = 0;
        for(ui32 i = 0; i < m_chunks_num.x; ++i)
        {
            for(ui32 j = 0; j < m_chunks_num.y; ++j)
            {
                for(ui32 k = 0; k < e_heigtmap_chunk_lod_max; ++k)
                {
                    glm::ivec2 current_chunk_size = glm::ivec2(m_chunk_lods_sizes[k].x % 2 == 0 ? m_chunk_lods_sizes[k].x : m_chunk_lods_sizes[k].x - 1,
                                                               m_chunk_lods_sizes[k].y % 2 == 0 ? m_chunk_lods_sizes[k].y : m_chunk_lods_sizes[k].y - 1);
                    
                    glm::ivec2 extended_chunk_size = current_chunk_size;
                    extended_chunk_size -= glm::ivec2(k != e_heigtmap_chunk_lod_01 ? 2 : 0,
                                                      k != e_heigtmap_chunk_lod_01 ? 2 : 0);
                    
                    ui32 additional_indices_count = k != e_heigtmap_chunk_lod_01 ? 12 * (m_chunk_size.x - 1 + current_chunk_size.x) : 0;
                    ui32 indices_count = extended_chunk_size.x * extended_chunk_size.y * 6 + additional_indices_count;
                    
                    m_ibos_mmap[i + j * m_chunks_num.x][k] = std::make_shared<heightmap_ibo_memory_map>(m_ibos_mmap_descriptor);
                    m_ibos_mmap[i + j * m_chunks_num.x][k]->set_size(indices_count);
                    m_ibos_mmap[i + j * m_chunks_num.x][k]->set_offset(offset);
                    offset += indices_count * 2;
                }
            }
        }
    }
    
    void heightmap_container::mmap_mask_textures(const std::string& filename)
    {
        heightmap_container::erase_mask_textures();
        
        m_splatting_mask_textures_mmap_descriptor = std::make_shared<memory_map>();
        m_splatting_mask_textures_mmap_descriptor->allocate(heightmap_loader::get_splatting_mask_textures_mmap_filename(filename));
        
        ui32 offset = 0;
        for(ui32 i = 0; i < m_chunks_num.x; ++i)
        {
            for(ui32 j = 0; j < m_chunks_num.y; ++j)
            {
                m_splatting_mask_textures_mmap[i + j * m_chunks_num.x] = std::make_shared<heightmap_texture_memory_map_RGB565>(m_splatting_mask_textures_mmap_descriptor);
                m_splatting_mask_textures_mmap[i + j * m_chunks_num.x]->set_size(heightmap_texture_generator::k_splatting_texture_mask_size.x *
                                                                                 heightmap_texture_generator::k_splatting_texture_mask_size.y);
                m_splatting_mask_textures_mmap[i + j * m_chunks_num.x]->set_offset(offset);
                offset += heightmap_texture_generator::k_splatting_texture_mask_size.x * heightmap_texture_generator::k_splatting_texture_mask_size.y;
            }
        }
    }
    
    void heightmap_container::mmap_diffuse_textures(const std::string& filename)
    {
        heightmap_container::erase_diffuse_textures();
        
        m_splatting_diffuse_textures_mmap_descriptor = std::make_shared<memory_map>();
        m_splatting_diffuse_textures_mmap_descriptor->allocate(heightmap_loader::get_splatting_diffuse_textures_mmap_filename(filename));
        
        ui32 offset = 0;
        for(ui32 i = 0; i < m_chunks_num.x; ++i)
        {
            for(ui32 j = 0; j < m_chunks_num.y; ++j)
            {
                for(ui32 k = 0; k < e_heigtmap_chunk_lod_max; ++k)
                {
                    m_splatting_diffuse_textures_mmap[i + j * m_chunks_num.x][k] = std::make_shared<heightmap_texture_memory_map_RGB565>(m_splatting_diffuse_textures_mmap_descriptor);
                    m_splatting_diffuse_textures_mmap[i + j * m_chunks_num.x][k]->set_size(heightmap_container::get_textures_lod_size(static_cast<e_heigtmap_chunk_lod>(k)).x *
                                                                                           heightmap_container::get_textures_lod_size(static_cast<e_heigtmap_chunk_lod>(k)).y);
                    m_splatting_diffuse_textures_mmap[i + j * m_chunks_num.x][k]->set_offset(offset);
                    offset += heightmap_container::get_textures_lod_size(static_cast<e_heigtmap_chunk_lod>(k)).x *
                    heightmap_container::get_textures_lod_size(static_cast<e_heigtmap_chunk_lod>(k)).y;
                }
            }
        }
    }
    
    void heightmap_container::mmap_normal_textures(const std::string& filename)
    {
        heightmap_container::erase_normal_textures();
        
        m_splatting_normal_textures_mmap_descriptor = std::make_shared<memory_map>();
        m_splatting_normal_textures_mmap_descriptor->allocate(heightmap_loader::get_splatting_normal_textures_mmap_filename(filename));
        
        ui32 offset = 0;
        for(ui32 i = 0; i < m_chunks_num.x; ++i)
        {
            for(ui32 j = 0; j < m_chunks_num.y; ++j)
            {
                for(ui32 k = 0; k < e_heigtmap_chunk_lod_max; ++k)
                {
                    m_splatting_normal_textures_mmap[i + j * m_chunks_num.x][k] = std::make_shared<heightmap_texture_memory_map_RGBA8>(m_splatting_normal_textures_mmap_descriptor);
                    m_splatting_normal_textures_mmap[i + j * m_chunks_num.x][k]->set_size(heightmap_container::get_textures_lod_size(static_cast<e_heigtmap_chunk_lod>(k)).x *
                                                                                heightmap_container::get_textures_lod_size(static_cast<e_heigtmap_chunk_lod>(k)).y *
                                                                                heightmap_texture_generator::k_splatting_texture_channels);
                    m_splatting_normal_textures_mmap[i + j * m_chunks_num.x][k]->set_offset(offset);
                    offset += heightmap_container::get_textures_lod_size(static_cast<e_heigtmap_chunk_lod>(k)).x *
                    heightmap_container::get_textures_lod_size(static_cast<e_heigtmap_chunk_lod>(k)).y * heightmap_texture_generator::k_splatting_texture_channels;
                }
            }
        }
    }
    
    void heightmap_container::mmap_displace_textures(const std::string& filename)
    {
        heightmap_container::erase_displace_textures();
        
        m_splatting_displace_textures_mmap_descriptor = std::make_shared<memory_map>();
        m_splatting_displace_textures_mmap_descriptor->allocate(heightmap_loader::get_splatting_displace_textures_mmap_filename(filename));
        
        ui32 offset = 0;
        for(ui32 i = 0; i < m_chunks_num.x; ++i)
        {
            for(ui32 j = 0; j < m_chunks_num.y; ++j)
            {
                for(ui32 k = 0; k < e_heigtmap_chunk_lod_max; ++k)
                {
                    m_splatting_displace_textures_mmap[i + j * m_chunks_num.x][k] = std::make_shared<heightmap_texture_memory_map_RGBA8>(m_splatting_displace_textures_mmap_descriptor);
                    m_splatting_displace_textures_mmap[i + j * m_chunks_num.x][k]->set_size(heightmap_container::get_textures_lod_size(static_cast<e_heigtmap_chunk_lod>(k)).x *
                                                                                heightmap_container::get_textures_lod_size(static_cast<e_heigtmap_chunk_lod>(k)).y *
                                                                                heightmap_texture_generator::k_splatting_texture_channels);
                    m_splatting_displace_textures_mmap[i + j * m_chunks_num.x][k]->set_offset(offset);
                    offset += heightmap_container::get_textures_lod_size(static_cast<e_heigtmap_chunk_lod>(k)).x *
                    heightmap_container::get_textures_lod_size(static_cast<e_heigtmap_chunk_lod>(k)).y * heightmap_texture_generator::k_splatting_texture_channels;
                }
            }
        }
    }
    
    void heightmap_container::create_deep_texture()
    {
        ui32 texture_id;
        gl_create_textures(1, &texture_id);
        m_deep_texture = texture::construct(k_deep_texture_name, texture_id, m_main_size.x, m_main_size.y);
        
        m_deep_texture->set_wrap_mode(GL_CLAMP_TO_EDGE);
        m_deep_texture->set_mag_filter(GL_LINEAR);
        m_deep_texture->set_min_filter(GL_LINEAR);
    }
}
