//
//  ces_terrain_component.h
//  gbCore
//
//  Created by sergey.sergeev on 11/9/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#ifndef ces_terrain_component_h
#define ces_terrain_component_h

#include "ces_base_component.h"
#include "terrain_texture_generator.h"
#include "terrain_container.h"

namespace gb
{
    class ces_terrain_component : public ces_base_component
    {
    public:
        
        enum e_terrain_generation_state
        {
            e_terrain_generation_state_none = 0,
            e_terrain_generation_state_generating,
            e_terrain_generation_state_generated
        };
        
    private:
        
        e_terrain_generation_state m_generation_state;
        
        std::vector<std::tuple<std::string, i32, material_shared_ptr>> m_materials;
        
        std::array<texture_shared_ptr, terrain_texture_generator::e_splatting_texture_max> m_splatting_diffuse_textures;
        std::array<texture_shared_ptr, terrain_texture_generator::e_splatting_texture_max> m_splatting_normal_textures;
        std::array<texture_shared_ptr, terrain_texture_generator::e_splatting_texture_max> m_splatting_displace_textures;
        
        terrain_accessor_shared_ptr m_terrain_accessor;
        std::string m_filename;
        
        std::vector<terrain_chunk_shared_ptr> m_chunks;
        
        terrain_container::e_heigtmap_chunk_lod get_current_lod(const glm::vec3& position,
                                                                  const glm::vec3& min_bound,
                                                                  const glm::vec3& max_bound) const;
        
    protected:

    public:
        
        ces_terrain_component(const std::string& filename);
        ~ces_terrain_component();
        
        e_terrain_generation_state get_generation_state() const;
        void generate(const graphics_context_shared_ptr& graphics_context,
                      const resource_accessor_shared_ptr& resource_accessor);
        
        void update(f32 deltatime);
        
        void add_material(const std::string& technique_name, i32 technique_pass, const material_shared_ptr& material);
        
        void set_position(const glm::vec3& position);
        glm::vec3 get_position() const;
        
        void set_splatting_diffuse_textures(const std::array<texture_shared_ptr, terrain_texture_generator::e_splatting_texture_max>& splatting_diffuse_textures);
        void set_splatting_normal_textures(const std::array<texture_shared_ptr, terrain_texture_generator::e_splatting_texture_max>& splatting_normal_textures);
        void set_splatting_displace_textures(const std::array<texture_shared_ptr, terrain_texture_generator::e_splatting_texture_max>& splatting_displace_textures);
        
        f32 get_height(const glm::vec3& position) const;
        glm::vec2 get_angles_xz(const glm::vec3& position) const;
    };
};


#endif
