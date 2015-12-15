//
//  omni_light.h
//  gbCore
//
//  Created by sergey.sergeev on 9/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef omni_light_h
#define omni_light_h

#include "renderable_interface.h"

namespace gb
{
    class omni_light : public renderable_interface
    {
    private:
        
        std::vector<glm::vec4>* m_transform_parameters_ref;
        std::vector<glm::vec4>* m_colors_ref;
        i32 m_instance_id;
        
    protected:
        
    public:
        
        omni_light();
        ~omni_light();
        
        void set_instance_id(i32 instance_id);
        void set_parameters_ref(std::vector<glm::vec4>* transform_parameters_ref,
                                std::vector<glm::vec4>* colors_ref);
        
        void set_position(const glm::vec3& position);
        glm::vec3 get_position() const;
        
        void set_radius(f32 radius);
        f32 get_radius() const;
        
        void set_color(const glm::vec4& color);
        glm::vec4 get_color() const;
        
        void add_material(const std::string& technique_name, i32 technique_pass, const material_shared_ptr& material);
        
        void set_mesh(const instanced_mesh_shared_ptr& mesh);
    };
};


#endif
