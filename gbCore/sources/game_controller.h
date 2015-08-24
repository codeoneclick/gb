//
//  game_controller.h
//  gbCore
//
//  Created by sergey.sergeev on 8/12/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef game_controller_h
#define game_controller_h

#include "main_headers.h"
#include "declarations.h"

namespace gb
{
    class ogl_window;
    class graphics_context;
    class game_transition;
    
    class game_controller
    {
    private:
        
        std::shared_ptr<graphics_context> m_graphics_context;
        configuration_accessor_shared_ptr m_configuration_accessor;
        resource_accessor_shared_ptr m_resource_accessor;
        
    protected:
        
        std::map<std::string, std::shared_ptr<game_transition>> m_transitions;
        std::shared_ptr<game_transition> m_current_transition;
        
    public:
        
        game_controller(const std::shared_ptr<ogl_window>& window);
        virtual ~game_controller(void);
        
        void add_transition(const std::shared_ptr<game_transition>& transition);
        void remove_transition(const std::shared_ptr<game_transition>& transition);
        void goto_transition(const std::string& guid);
    };
};

#endif
