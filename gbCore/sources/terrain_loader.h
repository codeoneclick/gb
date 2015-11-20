//
//  terrain_loader.h
//  gbCore
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef terrain_loader_h
#define terrain_loader_h

#include "main_headers.h"
#include "declarations.h"

namespace gb
{
    class terrain_loader
    {
    private:
        
    protected:
        
    public:
        
        terrain_loader() = default;
        ~terrain_loader() = default;
        
        static std::tuple<glm::ivec2, std::vector<f32>> get_heights_data(const std::string& filename);
        
        static std::string get_uncompressed_vertices_mmap_filename(const std::string& filename);
        static std::string get_compressed_vertices_mmap_filename(const std::string& filename);
        static std::string get_faces_mmap_filename(const std::string& filename);
        
        static std::string get_vbos_mmap_filename(const std::string& filename);
        static std::string get_ibos_mmap_filename(const std::string& filename);
        
        static std::string get_splatting_mask_textures_mmap_filename(const std::string& filename);
        static std::string get_splatting_diffuse_textures_mmap_filename(const std::string& filename);
        static std::string get_splatting_normal_textures_mmap_filename(const std::string& filename);
        static std::string get_splatting_displace_textures_mmap_filename(const std::string& filename);
        
        
        static std::string get_tangent_space_mmap_filename(const std::string& filename);
        static std::string get_attaches_to_vbo_mmap_filename(const std::string& filename);
        
        static bool is_uncompressed_vertices_mmap_exist(const std::string& filename);
        static bool is_compressed_vertices_mmap_exist(const std::string& filename);
        static bool is_faces_mmap_exist(const std::string& filename);
        
        static bool is_vbos_mmap_exist(const std::string& filename);
        static bool is_ibos_mmap_exist(const std::string& filename);
        
        static bool is_splatting_mask_textures_mmap_exist(const std::string& filename);
        static bool is_splatting_diffuse_textures_mmap_exist(const std::string& filename);
        static bool is_splatting_normal_textures_mmap_exist(const std::string& filename);
        static bool is_splatting_displace_textures_mmap_exist(const std::string& filename);
        
        static bool is_tangent_space_mmap_exist(const std::string& filename);
        static bool is_attaches_to_vbo_mmap_exist(const std::string& filename);
    };
};

#endif
