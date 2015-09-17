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
        
        static material_shared_ptr m_material;
        mesh_shared_ptr m_mesh;
        
        void bind_main_shader_uniforms(const material_shared_ptr& material);
        
    public:
        
        ces_debug_render_component(void);
        ~ces_debug_render_component(void);
        
        virtual material_shared_ptr on_bind(const std::string& technique_name);
        virtual void on_draw(const std::string& technique_name);
        virtual void on_unbind(const std::string& technique_name);
        
        void set_mesh(const mesh_shared_ptr& mesh);
    };
};

#endif
