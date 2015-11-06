//
//  heightmap_container.hpp
//  gbCore
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//


namespace gb
{
    inline glm::ivec2 heightmap_container::get_main_size() const
    {
        return m_main_size;
    };
    
    inline glm::ivec2 heightmap_container::get_chunks_num() const
    {
        return m_chunks_num;
    };
    
    inline glm::ivec2 heightmap_container::get_chunk_size() const
    {
        return m_chunk_size;
    };
    
    inline glm::ivec2 heightmap_container::get_chunk_lod_size(e_heigtmap_chunk_lod lod) const
    {
        assert(lod >= 0 && lod < e_heigtmap_chunk_lod_max);
        return m_chunk_lods_sizes[lod];
    }
    
    inline glm::ivec2 heightmap_container::get_textures_lod_size(e_heigtmap_chunk_lod lod) const
    {
        assert(lod >= 0 && lod < e_heigtmap_chunk_lod_max);
        return m_textures_lods_sizes[lod];
    }
    
    inline f32 heightmap_container::get_max_height() const
    {
        auto max = std::max_element(m_compressed_vertices, m_compressed_vertices + m_main_size.x * m_main_size.y, [](compressed_vertex const& value_01,
                                                                                                                     compressed_vertex const& value_02) {
            return value_01.m_position.y > value_02.m_position.y;
        });
        return max->m_position.y;
    }
    
    inline f32 heightmap_container::get_min_height() const
    {
        auto min = std::max_element(m_compressed_vertices, m_compressed_vertices + m_main_size.x * m_main_size.y, [] (compressed_vertex const& value_01,
                                                                                                                      compressed_vertex const& value_02) {
            return value_01.m_position.y < value_02.m_position.y;
        });
        return min->m_position.y;
    }
    
    inline heightmap_container::uncompressed_vertex* heightmap_container::get_uncopressed_vertices() const
    {
        assert(m_uncompressed_vertices != nullptr);
        return m_uncompressed_vertices;
    };
    
    inline heightmap_container::compressed_vertex* heightmap_container::get_compressed_vertices() const
    {
        assert(m_compressed_vertices != nullptr);
        return m_compressed_vertices;
    };
    
    inline heightmap_container::face* heightmap_container::get_faces() const
    {
        assert(m_faces != nullptr);
        return m_faces;
    };
    
    inline void heightmap_container::attach_uncompressed_vertex_to_vbo(i32 i, i32 j, ui32 vbo_index, ui32 vbo_vertex_index)
    {
        i32 index = i + j * m_main_size.x;
        assert(m_uncompressed_vertices[index].m_contains_in_vbo_size <= k_max_contains_in_vbo);
        m_uncompressed_vertices[index].m_contains_in_vbo[m_uncompressed_vertices[index].m_contains_in_vbo_size++] = glm::ivec2(vbo_index, vbo_vertex_index);
    }
    
    inline glm::ivec2* heightmap_container::attached_vertices_to_vbo(i32 i, i32 j, ui8 *size) const
    {
        i32 index = i + j * m_main_size.x;
        if(m_uncompressed_vertices[index].m_contains_in_vbo_size == 0 || m_uncompressed_vertices[index].m_contains_in_vbo_size > k_max_contains_in_vbo)
        {
            assert(m_uncompressed_vertices[index].m_contains_in_vbo_size != 0 && m_uncompressed_vertices[index].m_contains_in_vbo_size <= k_max_contains_in_vbo);
            *size = 0;
            return nullptr;
        }
        *size = m_uncompressed_vertices[index].m_contains_in_vbo_size;
        return m_uncompressed_vertices[index].m_contains_in_vbo;
    }
    
    inline void heightmap_container::attach_uncompressed_vertex_to_face(i32 i, i32 j, ui32 face_index)
    {
        i32 index = i + j * m_main_size.x;
        assert(m_uncompressed_vertices[index].m_contains_in_face_size <= k_max_contains_in_face);
        m_uncompressed_vertices[index].m_contains_in_face[m_uncompressed_vertices[index].m_contains_in_face_size++] = face_index;
    }
    
    inline ui32* heightmap_container::attached_vertices_to_face(i32 i, i32 j, ui8 *size) const
    {
        i32 index = i + j * m_main_size.x;
        assert(m_uncompressed_vertices[index].m_contains_in_face_size != 0 && m_uncompressed_vertices[index].m_contains_in_face_size <= k_max_contains_in_face);
        *size = m_uncompressed_vertices[index].m_contains_in_face_size;
        return m_uncompressed_vertices[index].m_contains_in_face;
    }
    
    inline glm::vec3 heightmap_container::get_vertex_position(ui32 i, ui32 j) const
    {
        return m_compressed_vertices[i + j * m_main_size.x].m_position;
    };
    
    inline glm::uint32 heightmap_container::get_compressed_vertex_texcoord(ui32 i, ui32 j) const
    {
        return m_compressed_vertices[i + j * m_main_size.x].m_texcoord;
    };
    
    inline glm::vec2 heightmap_container::get_uncompressed_vertex_texcoord(ui32 i, ui32 j) const
    {
        return glm::unpackUnorm2x16(m_compressed_vertices[i + j * m_main_size.x].m_texcoord);
    };
    
    inline glm::uint32 heightmap_container::get_compressed_vertex_normal(ui32 i, ui32 j) const
    {
        return m_compressed_vertices[i + j * m_main_size.x].m_normal;
    };
    
    inline glm::vec3 heightmap_container::get_uncompressed_vertex_normal(ui32 i, ui32 j) const
    {
        glm::vec4 normal = glm::unpackSnorm4x8(m_compressed_vertices[i + j * m_main_size.x].m_normal);
        return glm::vec3(normal.x, normal.y, normal.z);
    };
    
    inline std::shared_ptr<heightmap_container::heightmap_vbo_memory_map> heightmap_container::get_vbo_mmap(i32 index) const
    {
        assert(index >=0 && index < m_vbos_mmap.size());
        return m_vbos_mmap[index];
    };
    
    inline std::shared_ptr<heightmap_container::heightmap_ibo_memory_map> heightmap_container::get_ibo_mmap(i32 index, e_heigtmap_chunk_lod lod) const
    {
        assert(lod >= 0 && lod < e_heigtmap_chunk_lod_max);
        assert(index >=0 && index < m_ibos_mmap.size());
        return m_ibos_mmap[index][lod];
    };
    
    inline std::shared_ptr<heightmap_container::heightmap_texture_memory_map_RGB565> heightmap_container::get_splatting_mask_textures_mmap(i32 index) const
    {
        assert(index >=0 && index < m_splatting_mask_textures_mmap.size());
        return m_splatting_mask_textures_mmap[index];
    };
    
    inline std::shared_ptr<heightmap_container::heightmap_texture_memory_map_RGB565> heightmap_container::get_splatting_diffuse_textures_mmap(i32 index, e_heigtmap_chunk_lod lod) const
    {
        assert(lod >= 0 && lod < e_heigtmap_chunk_lod_max);
        assert(index >=0 && index < m_splatting_diffuse_textures_mmap.size());
        return m_splatting_diffuse_textures_mmap[index][lod];
    };
    
    inline std::shared_ptr<heightmap_container::heightmap_texture_memory_map_RGBA8> heightmap_container::get_splatting_normal_textures_mmap(i32 index, e_heigtmap_chunk_lod lod) const
    {
        assert(lod >= 0 && lod < e_heigtmap_chunk_lod_max);
        assert(index >=0 && index < m_splatting_normal_textures_mmap.size());
        return m_splatting_normal_textures_mmap[index][lod];
    };
    
    inline std::shared_ptr<heightmap_container::heightmap_texture_memory_map_RGBA8> heightmap_container::get_splatting_displace_textures_mmap(i32 index, e_heigtmap_chunk_lod lod) const
    {
        assert(lod >= 0 && lod < e_heigtmap_chunk_lod_max);
        assert(index >=0 && index < m_splatting_displace_textures_mmap.size());
        return m_splatting_displace_textures_mmap[index][lod];
    };
    
    inline texture_shared_ptr heightmap_container::get_deep_texture() const
    {
        assert(m_deep_texture != nullptr);
        return m_deep_texture;
    };
};
