//
//  render_technique_base.h
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef render_technique_base_h
#define render_technique_base_h

#include "main_headers.h"

namespace gb
{
    class render_technique_base
    {
    private:
        
    protected:
        
        ui32 m_frame_width;
        ui32 m_frame_height;
        ui32 m_frame_buffer;
        glm::vec4 m_clear_color;
        std::string m_name;
        ui32 m_index;
        
    public:
        
        render_technique_base(ui32 width, ui32 height, const std::string& name, ui32 index);
        virtual ~render_technique_base(void);
        
        ui32 get_index(void) const;
        
        ui32 get_frame_width(void) const;
        ui32 get_frame_height(void) const;
        
        std::string get_name(void) const;
        
        void set_clear_color(const glm::vec4& color);
        
        virtual void bind(void) = 0;
        virtual void unbind(void) = 0;
        virtual void draw(void) = 0;
    };
};

#endif
