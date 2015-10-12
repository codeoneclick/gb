//
//  instanced_omni_lights.h
//  gbCore
//
//  Created by sergey.sergeev on 10/1/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef instanced_omni_lights_h
#define instanced_omni_lights_h

#include "renderable_interface.h"

namespace gb
{
    class instanced_omni_lights : public renderable_interface
    {
    private:
        
        std::vector<glm::vec4> m_transform_parameters;
        std::vector<glm::vec4> m_colors;
        i32 m_num_instances;
        
    protected:
        
    public:
        
        instanced_omni_lights(i32 num_instances);
        ~instanced_omni_lights();
        
        void set_position(const glm::vec3& position, i32 instance_id);
        glm::vec3 get_position(i32 instance_id) const;
        
        void set_radius(f32 radius, i32 instance_id);
        f32 get_radius(i32 instance_id) const;
        
        void set_color(const glm::vec4& color, i32 instance_id);
        glm::vec4 get_color(i32 instance_id) const;
        
        void add_material(const std::string& technique_name, i32 technique_pass, const material_shared_ptr& material);
        
        void set_mesh(const instanced_mesh_shared_ptr& mesh);
    };
};

#endif
