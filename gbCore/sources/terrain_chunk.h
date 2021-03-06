//
//  terrain_chunk.h
//  gbCore
//
//  Created by sergey.sergeev on 11/9/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#ifndef terrain_chunk_h
#define terrain_chunk_h

#include "game_object.h"
#include "terrain_container.h"

namespace gb
{
    class terrain_chunk : public game_object
    {
    private:
        
    protected:
        
        terrain_container::e_heigtmap_chunk_lod m_current_lod;
        terrain_container::e_heigtmap_chunk_lod m_inprogress_lod;
        
        texture_shared_ptr m_diffuse_texture;
        texture_shared_ptr m_normal_displace_texture;
        
        void on_bind_material_imposer_callback(const material_shared_ptr& material);
        
    public:
        
        terrain_chunk();
        ~terrain_chunk();
        
        void set_mesh(const mesh_shared_ptr& mesh);
        void set_debug_tbn_mesh(const mesh_shared_ptr& mesh);
        
        void set_debug_tbn_enabled(bool value);
        bool is_debug_tbn_enabled() const;
        
        terrain_container::e_heigtmap_chunk_lod get_current_lod() const;
        terrain_container::e_heigtmap_chunk_lod get_inprogress_lod() const;
        
        void set_current_lod(terrain_container::e_heigtmap_chunk_lod lod);
        void set_inprogress_lod(terrain_container::e_heigtmap_chunk_lod lod);
        
        void set_diffuse_texture(const texture_shared_ptr& texture);
        void set_normal_displace_texture(const texture_shared_ptr& texture);
        
        void create_bind_material_imposer_link();
    };
};


#endif
