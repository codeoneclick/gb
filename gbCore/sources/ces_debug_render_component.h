//
//  ces_debug_render_component.h
//  gbCore
//
//  Created by sergey.sergeev on 8/31/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef ces_debug_render_component_h
#define ces_debug_render_component_h

#include "ces_base_component.h"

namespace gb
{
    class ces_debug_render_component : public ces_base_component
    {
    private:
        
    protected:
        
        material_shared_ptr m_material;
        mesh_shared_ptr m_mesh;
        
    public:
        
        ces_debug_render_component(void);
        ~ces_debug_render_component(void);
        
        virtual void bind_transformation_uniforms(const std::string& technique_name,
                                                  const glm::mat4& matrix_m,
                                                  const glm::mat4& matrix_mvp,
                                                  const glm::mat4& matrix_imvp);
        
        virtual void bind_camera_uniforms(const std::string& technique_name,
                                          const std::shared_ptr<camera>& camera);
        
        virtual void on_bind(const std::string& technique_name);
        
        virtual void on_draw(const std::string& technique_name);
        
        virtual void on_unbind(const std::string& technique_name);
        
        void set_mesh(const mesh_shared_ptr& mesh);
    };
};

#endif
