//
//  terrain_geometry_generator.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef terrain_geometry_generator_h
#define terrain_geometry_generator_h

#include "main_headers.h"
#include "declarations.h"

namespace gb
{
    class terrain_container;
    class terrain_geometry_generator
    {
    private:
        
        static void create_vertices_data(const std::shared_ptr<terrain_container>& container, const glm::ivec2& size, const std::vector<f32>& heights,
                                         const std::string& filename);
        
        static void create_vbos_data(const std::shared_ptr<terrain_container>& container, const std::string& filename);
        static void create_ibos_data(const std::shared_ptr<terrain_container>& container, const std::string& filename);
        static void create_tangent_space(const std::shared_ptr<terrain_container>& container, const std::string& filename);
        static void create_smooth_texcoord(const std::shared_ptr<terrain_container>& container, const std::string& filename);
        static void create_attaches_to_vbo(const std::shared_ptr<terrain_container>& container, const std::string& filename);
        
        static glm::vec3 generate_tangent(const glm::vec3& point_01, const glm::vec3& point_02, const glm::vec3& point_03,
                                          const glm::vec2& texcoord_01, const glm::vec2& texcoord_02, const glm::vec2& texcoord_03);
        
        static glm::vec3 get_closest_point_on_line(const glm::vec3& a, const glm::vec3& b, const glm::vec3& p);
        static glm::vec3 ortogonalize(const glm::vec3& v1, const glm::vec3& v2);
        
    protected:
        
    public:
        
        terrain_geometry_generator() = default;
        ~terrain_geometry_generator() = default;
        
        static void generate(const std::shared_ptr<terrain_container>& container, const std::string& filename,
                             const glm::ivec2& size, const std::vector<f32>& heights);
        
        static void generate_smooth_texcoord(const std::shared_ptr<terrain_container>& container, const std::string& filename);
        static void generate_smooth_texcoord(const std::shared_ptr<terrain_container>& container, ui32 index);
        
        static void generate_tangent_space(const std::shared_ptr<terrain_container>& container, const std::string& filename);
        static void generate_tangent_space(const std::shared_ptr<terrain_container>& container, ui32 index);
        
        static void generate_attaches_to_vbo(const std::shared_ptr<terrain_container>& container, const std::string& filename);
        
        static void update_vertices(const std::shared_ptr<terrain_container>& container, const std::vector<glm::vec3>& vertices);
    };
};

#endif
