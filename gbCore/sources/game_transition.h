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
#include "game_loop.h"
#include "ces_entity.h"
#include "ces_configuration_component.h"

namespace gb
{
    class ces_systems_feeder;
    class graphics_context;
    class input_context;
    class resources_accessor;
    class configuration_accessor;
    class game_transition : public game_loop_interface, public ces_entity, public ces_configuration_component_interface
    {
    private:
        
        std::string m_guid;
        bool m_offscreen;
        std::shared_ptr<ces_systems_feeder> m_system_feeder;
        
    protected:
        
        void on_update(f32 deltatime);
        
        
        
        virtual void on_configuration_loaded(const std::shared_ptr<configuration>& configuration, bool success);
        
    public:
        
        game_transition(const std::string& guid, bool is_offscreen);
        virtual ~game_transition(void);
        
        void on_activated(const std::shared_ptr<graphics_context>& graphics_context,
                          const std::shared_ptr<input_context>& input_context,
                          const std::shared_ptr<resources_accessor>& resource_accessor,
                          const std::shared_ptr<configuration_accessor>& configurations_accessor);
        
        void on_deactivated(void);
        
        virtual void create_scene(void);
        std::string get_guid(void) const;
    };
};

#endif
