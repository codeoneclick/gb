//
//  heightmap_accessor.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef heightmap_accessor_h
#define heightmap_accessor_h

#include "main_headers.h"
#include "declarations.h"
#include "heightmap_container.h"
#include "heightmap_texture_generator.h"

namespace gb
{
    class heightmap_accessor
    {
    private:
        
        std::shared_ptr<heightmap_container> m_container;
        bool m_isGenerated;
        graphics_context_shared_ptr m_graphics_context;
        
        std::array<std::queue<texture_shared_ptr>, heightmap_container::e_heigtmap_chunk_lod_max> m_splatting_diffuse_textures_cache;
        std::array<std::queue<texture_shared_ptr>, heightmap_container::e_heigtmap_chunk_lod_max> m_splatting_normal_textures_cache;
        std::array<std::queue<texture_shared_ptr>, heightmap_container::e_heigtmap_chunk_lod_max> m_splatting_displace_textures_cache;
        
        std::vector<thread_operation_shared_ptr> m_executed_operations;
        
        std::vector<std::tuple<std::function<void(const mesh_shared_ptr&)>, std::function<void(const texture_shared_ptr&, const texture_shared_ptr&)>>> m_callbacks;
        std::vector<std::tuple<mesh_shared_ptr, texture_shared_ptr, texture_shared_ptr, heightmap_container::e_heigtmap_chunk_lod, std::shared_ptr<std::mutex>>> m_chunks_data;
        std::vector<std::tuple<glm::vec3, glm::vec3>> m_chunks_bounds;
        
        std::array<texture_shared_ptr, heightmap_texture_generator::e_splatting_texture_max> m_diffuse_textures;
        std::array<texture_shared_ptr, heightmap_texture_generator::e_splatting_texture_max> m_normal_textures;
        std::array<texture_shared_ptr, heightmap_texture_generator::e_splatting_texture_max> m_displace_textures;
        
        std::queue<thread_operation_shared_ptr> m_update_heightmap_operations;
        
        void create_loading_operations();
        void erase_loading_operations();
        
        void create_bounding_boxes();
        void create_bounding_box(ui32 i, ui32 j);
        void erase_bounding_boxes();
        
        void create_data_containers();
        void erase_data_containers();
        
        void erase_chunk_data(i32 index);
        
        void generate_mesh(i32 index, heightmap_container::e_heigtmap_chunk_lod lod);
        void generate_splatting_textures(i32 index, heightmap_container::e_heigtmap_chunk_lod lod);
        void update_splatting_textures(i32 index);
        
        static f32 get_angle(const glm::vec3& point_01,
                             const glm::vec3& point_02,
                             const glm::vec3& point_03);
        
    protected:
        
    public:
        
        heightmap_accessor();
        ~heightmap_accessor();
        
        inline glm::ivec2 get_main_size() const;
        inline glm::ivec2 get_chunks_num() const;
        inline glm::ivec2 get_chunk_size() const;
        inline const std::tuple<glm::vec3, glm::vec3> get_chunk_bounds(ui32 i, ui32 j) const;
        
        f32 get_height(const glm::vec3& position) const;
        glm::vec3 get_normal(const glm::vec3& position) const;
        glm::vec2 get_angles_xz(const glm::vec3& position) const;
        
        static f32 get_height(const heightmap_container_shared_ptr& container, const glm::vec3& position);
        static glm::vec3 get_normal(const heightmap_container_shared_ptr& container, const glm::vec3& position);
        static glm::vec2 get_angles_xz(const heightmap_container_shared_ptr& container, const glm::vec3& position);
        
        void generate(const std::string& filename, const graphics_context_shared_ptr& graphics_context,
                      const std::array<texture_shared_ptr, 3>& splatting_diffuse_textures,
                      const std::array<texture_shared_ptr, 3>& splatting_normal_textures,
                      const std::array<texture_shared_ptr, 3>& splatting_displace_textures,
                      const std::function<void(void)>& callback);
        
        void start_chunk_loading(i32 i, i32 j, heightmap_container::e_heigtmap_chunk_lod lod,
                                 const std::function<void(const mesh_shared_ptr&)>& callback_mesh_loaded,
                                 const std::function<void(const texture_shared_ptr&, const texture_shared_ptr&)>& callback_textures_loaded);
        void start_chunk_unloading(i32 i, i32 j);
        
        void update();
        void update_vertices(const std::vector<glm::vec3>& vertices,
                             const glm::ivec2& min_bound, const glm::ivec2& max_bound);
        
        inline texture_shared_ptr get_deep_texture() const;
    };
};

#include "heightmap_accessor.hpp"

#endif
