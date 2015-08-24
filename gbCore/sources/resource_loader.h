//
//  resource_loader.h
//  gbCore
//
//  Created by sergey.sergeev on 8/24/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef resource_loader_h
#define resource_loader_h

#include "main_headers.h"
#include "declarations.h"
#include "game_loop.h"

namespace gb
{
    class resource_loader : public game_loop_interface
    {
    private:
        
    protected:
        
        std::unordered_map<std::string, resource_loading_operation_shared_ptr> m_operationsQueue;
        std::unordered_map<std::string, resource_shared_ptr> m_resources;
        
        std::mutex m_mutex;
        std::thread m_thread;
        bool m_thread_executed;
        
        void on_thread_update();
        void on_update(f32 deltatime);
        
    public:
        
        resource_loader();
        ~resource_loader();
        
        shader_shared_ptr get_shader(const std::string& vs_filename,
                                     const std::string& fs_filename, bool sync = false);
        
        texture_shared_ptr get_texture(const std::string& filename, bool sync = false);
        
    };
};

#endif
