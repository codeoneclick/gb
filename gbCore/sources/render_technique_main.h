//
//  render_technique_main.h
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef render_technique_main_h
#define render_technique_main_h

#include "render_technique_base.h"

namespace gb
{
    class material;
    class quad;
    class render_technique_main : render_technique_base
    {
    private:
        
    protected:
        
        std::shared_ptr<material> m_material;
        std::shared_ptr<quad> m_quad;
        ui32 m_render_buffer;
        
        
    public:
        
        render_technique_main(ui32 width, ui32 height, const std::shared_ptr<material> material, ui32 frame_buffer, ui32 render_buffer);
        ~render_technique_main(void);
        
        void bind(void);
        void unbind(void);
        void draw(void);
    };
};

#endif
