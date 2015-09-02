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
    class render_technique_ws : public render_technique_base
    {
    private:
        
    protected:
        
        texture_shared_ptr m_color_attachment_texture;
        texture_shared_ptr m_depth_attachment_texture;
        
        std::vector<std::queue<ces_entity_shared_ptr>> m_entities;
        
    public:
        
        render_technique_ws(ui32 width, ui32 height, const std::string& name, ui32 index);
        ~render_technique_ws();
        
        std::shared_ptr<texture> get_color_attachment_texture() const;
        std::shared_ptr<texture> get_depth_attachment_texture() const;
        
        void bind();
        void unbind();
        void draw();
        
        void add_entity(const ces_entity_shared_ptr& entity);
    };
};

#endif
