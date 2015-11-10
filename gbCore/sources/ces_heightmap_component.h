//
//  ces_heightmap_component.h
//  gbCore
//
//  Created by sergey.sergeev on 11/9/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#ifndef ces_heightmap_component_h
#define ces_heightmap_component_h

#include "ces_base_component.h"
#include "heightmap_texture_generator.h"
#include "heightmap_container.h"

namespace gb
{
    class ces_heightmap_component : public ces_base_component
    {
    public:
        
        enum e_heightmap_generation_state
        {
            e_heightmap_generation_state_none = 0,
            e_heightmap_generation_state_generating,
            e_heightmap_generation_state_generated
        };
        
    private:
        
        e_heightmap_generation_state m_generation_state;
        
        std::vector<std::tuple<std::string, i32, material_shared_ptr>> m_materials;
        
        std::array<texture_shared_ptr, heightmap_texture_generator::e_splatting_texture_max> m_splatting_diffuse_textures;
        std::array<texture_shared_ptr, heightmap_texture_generator::e_splatting_texture_max> m_splatting_normal_textures;
        std::array<texture_shared_ptr, heightmap_texture_generator::e_splatting_texture_max> m_splatting_displace_textures;
        
        heightmap_accessor_shared_ptr m_heightmap_accessor;
        std::string m_filename;
        
        std::vector<heightmap_chunk_shared_ptr> m_chunks;
        
        heightmap_container::e_heigtmap_chunk_lod get_current_lod(const glm::vec3& position,
                                                                  const glm::vec3& min_bound,
                                                                  const glm::vec3& max_bound) const;
        
    protected:

    public:
        
        ces_heightmap_component(const std::string& filename);
        ~ces_heightmap_component();
        
        e_heightmap_generation_state get_generation_state() const;
        void generate(const graphics_context_shared_ptr& graphics_context);
        
        void update(f32 deltatime);
        
        void add_material(const std::string& technique_name, i32 technique_pass, const material_shared_ptr& material);
        
        void set_splatting_diffuse_textures(const std::array<texture_shared_ptr, heightmap_texture_generator::e_splatting_texture_max>& splatting_diffuse_textures);
        void set_splatting_normal_textures(const std::array<texture_shared_ptr, heightmap_texture_generator::e_splatting_texture_max>& splatting_normal_textures);
        void set_splatting_displace_textures(const std::array<texture_shared_ptr, heightmap_texture_generator::e_splatting_texture_max>& splatting_displace_textures);
    };
};


#endif
