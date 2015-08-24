//
//  render_technique_ws.h
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef render_technique_ws_h
#define render_technique_ws_h

#include "render_technique_base.h"

namespace gb
{
    class texture;
    class ces_entity;
    class render_technique_ws : public render_technique_base
    {
    private:
        
    protected:
        
        std::shared_ptr<texture> m_color_attachment_texture;
        std::shared_ptr<texture> m_depth_attachment_texture;
        
        std::queue<std::shared_ptr<ces_entity>> m_entities;
        
    public:
        
        render_technique_ws(ui32 width, ui32 height, const std::string& name, ui32 index);
        ~render_technique_ws(void);
        
        std::shared_ptr<texture> get_color_attachment_texture(void) const;
        std::shared_ptr<texture> get_depth_attachment_texture(void) const;
        
        void bind(void);
        void unbind(void);
        void draw(void);
        
        void add_entity(const std::shared_ptr<ces_entity>& entity);
    };
};

#endif
