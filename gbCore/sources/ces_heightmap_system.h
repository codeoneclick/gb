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
        
        graphics_context_shared_ptr m_graphic_context;
        resource_accessor_shared_ptr m_resource_accessor;
        
    protected:
        
        void on_feed_start(f32 deltatime);
        void on_feed(const ces_entity_shared_ptr& entity, f32 deltatime);
        void on_feed_end(f32 deltatime);
        
    public:
        
        ces_heightmap_system(const graphics_context_shared_ptr& graphic_context,
                             const resource_accessor_shared_ptr& resource_accessor);
        ~ces_heightmap_system();
    };
};

#endif
