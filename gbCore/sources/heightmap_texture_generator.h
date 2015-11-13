//
//  heightmap_texture_generator.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef heightmap_texture_generator_h
#define heightmap_texture_generator_h

#include "main_headers.h"
#include "declarations.h"
#include "heightmap_container.h"

namespace gb
{
    class heightmap_texture_generator
    {
    public:
        
        enum e_splatting_texture
        {
            e_splatting_texture_01 = 0,
            e_splatting_texture_02,
            e_splatting_texture_03,
            e_splatting_texture_max
        };
        
    private:
        
        static void create_splatting_rgb565_mask(const heightmap_container_shared_ptr& container, const std::string& filename);
        
        static void create_splatting_rgb565_diffuse_textures(const graphics_context_shared_ptr& graphics_context,
                                                             const heightmap_container_shared_ptr& container, const std::string& filename,
                                                             const std::array<texture_shared_ptr, e_splatting_texture_max>& diffuse_textures);
        
        static void create_splatting_rgb888_normal_a8_displace_textures(const graphics_context_shared_ptr& graphics_context,
                                                                        const heightmap_container_shared_ptr& container, const std::string& filename,
                                                                        const std::array<texture_shared_ptr, e_splatting_texture_max>& normal_textures,
                                                                        const std::array<texture_shared_ptr, e_splatting_texture_max>& displace_textures);
        
        static void create_splatting_texture(const graphics_context_shared_ptr& graphics_context,
                                             const heightmap_container_shared_ptr& container,
                                             const std::vector<texture_shared_ptr>& textures,
                                             ui32 offset_x, ui32 offset_y, const std::function<void(ui8 *, ui32, heightmap_container::e_heigtmap_chunk_lod lod)>& callback);
        
        static material_shared_ptr get_splatting_material(const std::vector<texture_shared_ptr>& textures);
        static material_shared_ptr get_splatting_material_rgb565(const std::vector<texture_shared_ptr>& textures);
        static material_shared_ptr get_splatting_material_rgba8888(const std::vector<texture_shared_ptr>& textures);
        
    protected:
        
    public:
        
        static const ui8 k_splatting_texture_channels;
        
        static const glm::ivec2 k_splatting_texture_mask_size;
        
        static const glm::ivec2 k_splatting_texture_size_lod_01;
        static const glm::ivec2 k_splatting_texture_size_lod_02;
        static const glm::ivec2 k_splatting_texture_size_lod_03;
        static const glm::ivec2 k_splatting_texture_size_lod_04;
        
        heightmap_texture_generator() = default;
        ~heightmap_texture_generator() = default;
        
        static void splatting_masks(const heightmap_container_shared_ptr& container, const std::string& filename);
        static void splatting_mask(const heightmap_container_shared_ptr& container,
                                   ui32 offset_x, ui32 offset_y, const std::shared_ptr<std::ofstream> stream = nullptr);
        
        static void splatting_diffuse_textures(const graphics_context_shared_ptr& graphics_context,
                                               const heightmap_container_shared_ptr& container, const std::string& filename,
                                               const std::array<texture_shared_ptr, e_splatting_texture_max>& textures);
        
        static void splatting_diffuse_texture(const graphics_context_shared_ptr& graphics_context,
                                              const heightmap_container_shared_ptr& container,
                                              const std::array<texture_shared_ptr, e_splatting_texture_max>& textures,
                                              ui32 offset_x, ui32 offset_y, const std::shared_ptr<std::ofstream> stream = nullptr);
        
        static void splatting_normal_displace_textures(const graphics_context_shared_ptr& graphics_context,
                                                       const heightmap_container_shared_ptr& container, const std::string& filename,
                                                       const std::array<texture_shared_ptr, e_splatting_texture_max>& normal_textures,
                                                       const std::array<texture_shared_ptr, e_splatting_texture_max>& displace_textures);
        
        static void splatting_normal_displace_texture(const graphics_context_shared_ptr& graphics_context,
                                                      const heightmap_container_shared_ptr& container,
                                                      const std::array<texture_shared_ptr, e_splatting_texture_max>& normal_textures,
                                                      const std::array<texture_shared_ptr, e_splatting_texture_max>& displace_textures,
                                                      ui32 offset_x, ui32 offset_y, const std::shared_ptr<std::ofstream> stream = nullptr);
        
        static void generate_deep_texture(const heightmap_container_shared_ptr& container, bool create,
                                          ui32 offset_x, ui32 offset_y,
                                          ui32 sub_width, ui32 sub_height);
        
    };
};

#endif
