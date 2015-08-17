//
//  render_technique_ss.h
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef render_technique_ss_h
#define render_technique_ss_h

#include "render_technique_base.h"

namespace gb
{
    class texture;
    class material;
    class quad;
    class render_technique_ss : public render_technique_base
    {
    private:
        
    protected:
        
        std::shared_ptr<texture> m_color_attachment_texture;
        std::shared_ptr<material> m_material;
        std::shared_ptr<quad> m_quad;
        
    public:
        
        render_technique_ss(ui32 width, ui32 height, const std::string& name, const std::shared_ptr<material>& material);
        ~render_technique_ss(void);
        
        std::shared_ptr<texture> get_color_attachment_texture(void) const;
        std::shared_ptr<material> get_material(void) const;
        
        void bind(void);
        void unbind(void);
        void draw(void);
    };
};

#endif
