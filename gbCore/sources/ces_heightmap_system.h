//
//  ces_heightmap_system.h
//  gbCore
//
//  Created by sergey.sergeev on 11/9/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#ifndef ces_heightmap_system_h
#define ces_heightmap_system_h

#include "ces_system.h"

namespace gb
{
    class ces_heightmap_system : public ces_system
    {
    private:
        
        std::shared_ptr<graphics_context> m_graphic_context;
        
    protected:
        
        void on_feed_start(f32 deltatime);
        void on_feed(const ces_entity_shared_ptr& entity, f32 deltatime);
        void on_feed_end(f32 deltatime);
        
    public:
        
        ces_heightmap_system(const std::shared_ptr<graphics_context>& graphic_context);
        ~ces_heightmap_system();
    };
};

#endif
