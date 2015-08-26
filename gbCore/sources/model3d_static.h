//
//  model3d_static.h
//  gbCore
//
//  Created by sergey.sergeev on 8/26/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef model3d_static_h
#define model3d_static_h

#include "game_object.h"

namespace gb
{
    class model3d_static : public game_object
    {
    private:
        
    protected:
        
        ces_camera_component_shared_ptr m_camera_component;
        ces_frustum_culling_component_shared_ptr m_frustum_culling_component;
        ces_global_light_component_shared_ptr m_global_light_component;
        ces_geometry_component_shared_ptr m_geometry_component;
        ces_render_component_shared_ptr m_render_component;
        
    public:
        
        model3d_static();
        ~model3d_static();
        
        void add_material(const std::string& technique_name, const material_shared_ptr& material);
        void remove_material(const std::string& technique_name);
        material_shared_ptr get_material(const std::string& technique_name) const;
        
        void set_mesh(const mesh_shared_ptr& mesh);
    };
};

#endif
