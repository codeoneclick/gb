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
        
        static void create_splatting_mask_textures(const std::shared_ptr<heightmap_container>& container, const std::string& filename);
        
        static void create_splatting_diffuse_textures(const std::shared_ptr<graphics_context>& graphics_context,
                                                      const std::shared_ptr<heightmap_container>& container, const std::string& filename,
                                                      const std::array<texture_shared_ptr, e_splatting_texture_max>& splatting_diffuse_textures);
        
        static void create_splatting_normal_textures(const std::shared_ptr<graphics_context>& graphics_context,
                                                     const std::shared_ptr<heightmap_container>& container, const std::string& filename,
                                                     const std::array<texture_shared_ptr, e_splatting_texture_max>& splatting_normal_textures);
        
        static void create_splatting_displace_textures(const std::shared_ptr<graphics_context>& graphics_context,
                                                       const std::shared_ptr<heightmap_container>& container, const std::string& filename,
                                                       const std::array<texture_shared_ptr, e_splatting_texture_max>& splatting_displace_textures);
        
        static void generate_splatting_texture(const std::shared_ptr<graphics_context>& graphics_context,
                                               const std::shared_ptr<heightmap_container>& container,
                                               const std::array<texture_shared_ptr, e_splatting_texture_max>& splatting_textures,
                                               ui32 i, ui32 j, const std::function<void(ui8 *, ui32, heightmap_container::e_heigtmap_chunk_lod lod)>& callback);
        
        static material_shared_ptr get_splatting_textures_material(const std::array<texture_shared_ptr, e_splatting_texture_max>& splatting_textures);
        
    protected:
        
    public:
        
        static const f32 k_min_splatting_texture_height;
        static const f32 k_max_splatting_texture_height;
        static const ui8 k_splatting_texture_channels;
        static const glm::ivec2 k_splatting_texture_mask_size;
        static const glm::ivec2 k_splatting_texture_size_lod_01;
        static const glm::ivec2 k_splatting_texture_size_lod_02;
        static const glm::ivec2 k_splatting_texture_size_lod_03;
        static const glm::ivec2 k_splatting_texture_size_lod_04;
        
        heightmap_texture_generator() = default;
        ~heightmap_texture_generator() = default;
        
        static void generate_splatting_mask_textures(const std::shared_ptr<heightmap_container>& container, const std::string& filename);
        static void generate_splatting_mask_texture(const std::shared_ptr<heightmap_container>& container,
                                                    ui32 i, ui32 j, const std::shared_ptr<std::ofstream> stream = nullptr);
        
        static void generate_splatting_diffuse_textures(const std::shared_ptr<graphics_context>& graphics_context,
                                                        const std::shared_ptr<heightmap_container>& container, const std::string& filename,
                                                        const std::array<texture_shared_ptr, e_splatting_texture_max>& splatting_diffuse_textures);
        static void generate_splatting_diffuse_texture(const std::shared_ptr<graphics_context>& graphics_context,
                                                       const std::shared_ptr<heightmap_container>& container,
                                                       const std::array<texture_shared_ptr, e_splatting_texture_max>& splatting_diffuse_textures,
                                                       ui32 i, ui32 j, const std::shared_ptr<std::ofstream> stream = nullptr);
        
        static void generate_splatting_normal_textures(const std::shared_ptr<graphics_context>& graphics_context,
                                                       const std::shared_ptr<heightmap_container>& container, const std::string& filename,
                                                       const std::array<texture_shared_ptr, e_splatting_texture_max>& splatting_normal_textures);
        
        static void generate_splatting_normal_texture(const std::shared_ptr<graphics_context>& graphics_context,
                                                      const std::shared_ptr<heightmap_container>& container,
                                                      const std::array<texture_shared_ptr, e_splatting_texture_max>& splatting_normal_textures,
                                                      ui32 i, ui32 j, const std::shared_ptr<std::ofstream> stream = nullptr);
        
        static void generate_splatting_displace_textures(const std::shared_ptr<graphics_context>& graphics_context,
                                                         const std::shared_ptr<heightmap_container>& container, const std::string& filename,
                                                         const std::array<texture_shared_ptr, e_splatting_texture_max>& splatting_displace_textures);
        
        static void generate_splatting_displace_texture(const std::shared_ptr<graphics_context>& graphics_context,
                                                        const std::shared_ptr<heightmap_container>& container,
                                                        const std::array<texture_shared_ptr, e_splatting_texture_max>& splatting_displace_textures,
                                                        ui32 i, ui32 j, const std::shared_ptr<std::ofstream> stream = nullptr);
        
        static void generate_deep_texture(const std::shared_ptr<heightmap_container>& container, bool create,
                                          ui32 offset_x, ui32 offset_y,
                                          ui32 sub_width, ui32 sub_height);
        
    };
};

#endif
