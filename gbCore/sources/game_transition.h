//
//  game_transition.h
//  gbCore
//
//  Created by sergey.sergeev on 8/12/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef game_transition_h
#define game_transition_h

#include "main_headers.h"
#include "declarations.h"
#include "game_loop.h"
#include "ces_entity.h"
#include "ces_configuration_component.h"

namespace gb
{
    class ces_systems_feeder;
    class graphics_context;
    class input_context;

    class game_transition : public game_loop_interface
    {
    private:
        
        std::string m_guid;
        bool m_offscreen;
        std::shared_ptr<ces_systems_feeder> m_system_feeder;
        
    protected:
        
        void on_update(f32 deltatime);
        
    public:
        
        game_transition(const std::string& guid, bool is_offscreen);
        virtual ~game_transition(void);
        
        void on_activated(const std::shared_ptr<graphics_context>& graphics_context,
                          const std::shared_ptr<input_context>& input_context,
                          const configuration_accessor_shared_ptr& configurations_accessor,
                          const resource_accessor_shared_ptr& resource_accessor);
        
        void on_deactivated(void);
        
        virtual void create_scene(void);
        std::string get_guid(void) const;
    };
};

#endif
