//
//  heightmap_container.h
//  gbCore
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef heightmap_container_h
#define heightmap_container_h

#include "main_headers.h"
#include "declarations.h"
#include "memory_map.h"
#include "vbo.h"

namespace gb
{
    
#define k_max_contains_in_face 32
#define k_max_contains_in_vbo 4
    
    class heightmap_container
    {
    public:
        
        enum e_heigtmap_chunk_lod
        {
            e_heigtmap_chunk_lod_unknown = -1,
            e_heigtmap_chunk_lod_01 = 0,
            e_heigtmap_chunk_lod_02,
            e_heigtmap_chunk_lod_03,
            e_heigtmap_chunk_lod_04,
            e_heigtmap_chunk_lod_max
        };
        
        struct uncompressed_vertex
        {
            glm::vec3 m_position;
            glm::vec3 m_normal;
            glm::vec2 m_texcoord;
            
            ui32 m_contains_in_face[k_max_contains_in_face];
            ui8 m_contains_in_face_size;
            
            glm::ivec2 m_contains_in_vbo[k_max_contains_in_vbo];
            ui8 m_contains_in_vbo_size;
            
            uncompressed_vertex()
            {
                m_contains_in_face_size = 0;
                m_contains_in_vbo_size = 0;
                
                memset(m_contains_in_face, 0x0, sizeof(ui32) * k_max_contains_in_face);
                memset(m_contains_in_vbo, 0x0, sizeof(glm::ivec2) * k_max_contains_in_vbo);
            };
            
            ~uncompressed_vertex() = default;
            
            uncompressed_vertex(const uncompressed_vertex& copy) = delete;
            uncompressed_vertex(uncompressed_vertex&& copy) = delete;
            uncompressed_vertex& operator = (const uncompressed_vertex& copy) = delete;
            uncompressed_vertex& operator = (uncompressed_vertex&& copy) = delete;
        };
        
        struct compressed_vertex
        {
            glm::vec3 m_position;
            glm::uint32 m_normal;
            glm::uint32 m_texcoord;
            
            compressed_vertex() = default;
            ~compressed_vertex() = default;
        };
        
        struct face
        {
            glm::vec3 m_normal;
            ui32 m_indexes[3];
            
            face()
            {
                memset(m_indexes, 0x0, sizeof(ui32) * 3);
            };
            
            ~face() = default;
            
            face(const face& copy) = delete;
            face(face&& copy) = delete;
            face& operator = (const face& copy) = delete;
            face& operator = (face&& copy) = delete;
        };
        
        class heightmap_memory_map
        {
        private:
            
        protected:
            
            std::shared_ptr<memory_map> m_descriptor;
            
            ui32 m_size;
            ui32 m_offset;
            
        public:
            
            heightmap_memory_map(const std::shared_ptr<memory_map>& descriptor) :
            m_descriptor(descriptor),
            m_size(0),
            m_offset(0)
            {
                
            }
            virtual ~heightmap_memory_map() = default;
            
            inline void set_size(ui32 size) { m_size = size; };
            inline void set_offset(ui32 offset) { m_offset = offset; }
            inline ui32 get_size() const { return m_size; };
        };
        
        class heightmap_vbo_memory_map : public heightmap_memory_map
        {
        private:
            
        protected:
            
        public:
            
            heightmap_vbo_memory_map(const std::shared_ptr<memory_map>& descriptor) : heightmap_memory_map(descriptor) { };
            ~heightmap_vbo_memory_map() = default;
            
            inline vbo::vertex_attribute *get_pointer() const
            {
                vbo::vertex_attribute* pointer = (vbo::vertex_attribute*)m_descriptor->pointer();
                assert(pointer != nullptr);
                return pointer + m_offset;
            };
        };
        
        class heightmap_ibo_memory_map : public heightmap_memory_map
        {
        private:
            
            
        protected:
            
        public:
            
            heightmap_ibo_memory_map(const std::shared_ptr<memory_map>& descriptor) : heightmap_memory_map(descriptor) { };
            ~heightmap_ibo_memory_map() = default;
            
            inline ui16* get_source_pointer() const
            {
                ui16* pointer = (ui16* )m_descriptor->pointer();
                assert(pointer != nullptr);
                return pointer + m_offset;
            };
            
            inline ui16* get_origin_pointer() const
            {
                ui16* pointer = (ui16* )m_descriptor->pointer();
                assert(pointer != nullptr);
                
                return pointer + m_offset + m_size;
            };
            
            inline void update_source_pointer()
            {
                ui16* source_pointer = heightmap_ibo_memory_map::get_source_pointer();
                ui16* origin_pointer = heightmap_ibo_memory_map::get_origin_pointer();
                memcpy(source_pointer, origin_pointer, m_size * sizeof(ui16));
            };
        };
        
        class heightmap_texture_memory_map_RGB565 : public heightmap_memory_map
        {
        private:
            
        protected:
            
        public:
            
            heightmap_texture_memory_map_RGB565(const std::shared_ptr<memory_map>& descriptor) : heightmap_memory_map(descriptor) { };
            ~heightmap_texture_memory_map_RGB565() = default;
            
            inline ui16* get_pointer() const
            {
                ui16* pointer = (ui16* )m_descriptor->pointer();
                assert(pointer != nullptr);
                return pointer + m_offset;
            };
        };
        
        class heightmap_texture_memory_map_RGBA8 : public heightmap_memory_map
        {
        private:
            
        protected:
            
        public:
            
            heightmap_texture_memory_map_RGBA8(const std::shared_ptr<memory_map>& descriptor) : heightmap_memory_map(descriptor) { };
            ~heightmap_texture_memory_map_RGBA8() = default;
            
            inline ui8* get_pointer() const
            {
                ui8* pointer = (ui8* )m_descriptor->pointer();
                assert(pointer != nullptr);
                return pointer + m_offset;
            };
        };
        
    private:
        
        uncompressed_vertex* m_uncompressed_vertices;
        face* m_faces;
        compressed_vertex* m_compressed_vertices;
        
        std::shared_ptr<memory_map> m_uncompressed_vertices_mmap_descriptor;
        std::shared_ptr<memory_map> m_faces_mmap_descriptor;
        std::shared_ptr<memory_map> m_compressed_vertices_mmap_descriptor;
        
        std::shared_ptr<memory_map> m_vbos_mmap_descriptor;
        std::shared_ptr<memory_map> m_ibos_mmap_descriptor;
        
        std::shared_ptr<memory_map> m_splatting_mask_textures_mmap_descriptor;
        std::shared_ptr<memory_map> m_splatting_diffuse_textures_mmap_descriptor;
        std::shared_ptr<memory_map> m_splatting_normal_textures_mmap_descriptor;
        std::shared_ptr<memory_map> m_splatting_displace_textures_mmap_descriptor;
        
        std::vector<std::shared_ptr<heightmap_vbo_memory_map>> m_vbos_mmap;
        std::vector<std::array<std::shared_ptr<heightmap_ibo_memory_map>, e_heigtmap_chunk_lod_max>> m_ibos_mmap;
        
        std::vector<std::shared_ptr<heightmap_texture_memory_map_RGB565>> m_splatting_mask_textures_mmap;
        std::vector<std::array<std::shared_ptr<heightmap_texture_memory_map_RGB565>, e_heigtmap_chunk_lod_max>> m_splatting_diffuse_textures_mmap;
        std::vector<std::array<std::shared_ptr<heightmap_texture_memory_map_RGBA8>, e_heigtmap_chunk_lod_max>> m_splatting_normal_textures_mmap;
        std::vector<std::array<std::shared_ptr<heightmap_texture_memory_map_RGBA8>, e_heigtmap_chunk_lod_max>> m_splatting_displace_textures_mmap;
        
        glm::ivec2 m_main_size;
        glm::ivec2 m_chunks_num;
        glm::ivec2 m_chunk_size;
        std::array<glm::ivec2, e_heigtmap_chunk_lod_max> m_chunk_lods_sizes;
        std::array<glm::ivec2, e_heigtmap_chunk_lod_max> m_textures_lods_sizes;
        
        texture_shared_ptr m_deep_texture;
        
        void erase_geometry();
        void erase_mask_textures();
        void erase_diffuse_textures();
        void erase_normal_textures();
        void erase_displace_textures();
        
    protected:
        
    public:
        
        heightmap_container();
        ~heightmap_container();
        
        static const f32 k_raise;
        static const f32 k_deep;
        
        static const f32 k_layer_section_01;
        static const f32 k_layer_section_02;
        static const f32 k_layer_section_offset;
        
        static const i32 k_max_chunk_size;
        
        void init(const glm::ivec2& size);
        void mmap_geometry(const std::string& filename);
        void mmap_mask_textures(const std::string& filename);
        void mmap_diffuse_textures(const std::string& filename);
        void mmap_normal_textures(const std::string& filename);
        void mmap_displace_textures(const std::string& filename);
        
        inline glm::ivec2 get_main_size() const;
        inline glm::ivec2 get_chunks_num() const;
        inline glm::ivec2 get_chunk_size() const;
        inline glm::ivec2 get_chunk_lod_size(e_heigtmap_chunk_lod lod) const;
        inline glm::ivec2 get_textures_lod_size(e_heigtmap_chunk_lod lod) const;
        
        inline f32 get_max_height() const;
        inline f32 get_min_height() const;
        
        inline uncompressed_vertex* get_uncopressed_vertices() const;
        inline compressed_vertex* get_compressed_vertices() const;
        inline face* get_faces() const;
        
        inline void attach_uncompressed_vertex_to_vbo(i32 i, i32 j, ui32 vbo_index, ui32 vbo_vertex_index);
        inline glm::ivec2* attached_vertices_to_vbo(i32 i, i32 j, ui8 *size) const;
        
        inline void attach_uncompressed_vertex_to_face(i32 i, i32 j, ui32 face_index);
        inline ui32* attached_vertices_to_face(i32 i, i32 j, ui8 *size) const;
        
        inline glm::vec3 get_vertex_position(ui32 i, ui32 j) const;
        inline glm::uint32 get_compressed_vertex_texcoord(ui32 i, ui32 j) const;
        inline glm::vec2 get_uncompressed_vertex_texcoord(ui32 i, ui32 j) const;
        inline glm::uint32 get_compressed_vertex_normal(ui32 i, ui32 j) const;
        inline glm::vec3 get_uncompressed_vertex_normal(ui32 i, ui32 j) const;
        
        inline std::shared_ptr<heightmap_vbo_memory_map> get_vbo_mmap(i32 index) const;
        inline std::shared_ptr<heightmap_ibo_memory_map> get_ibo_mmap(i32 index, e_heigtmap_chunk_lod lod) const;
        inline std::shared_ptr<heightmap_texture_memory_map_RGB565> get_splatting_mask_textures_mmap(i32 index) const;
        inline std::shared_ptr<heightmap_texture_memory_map_RGB565> get_splatting_diffuse_textures_mmap(i32 index, e_heigtmap_chunk_lod lod) const;
        inline std::shared_ptr<heightmap_texture_memory_map_RGBA8> get_splatting_normal_textures_mmap(i32 index, e_heigtmap_chunk_lod lod) const;
        inline std::shared_ptr<heightmap_texture_memory_map_RGBA8> get_splatting_displace_textures_mmap(i32 index, e_heigtmap_chunk_lod lod) const;
        
        void create_deep_texture();
        inline texture_shared_ptr get_deep_texture() const;
    };
};

#include "heightmap_container.hpp"

#endif
