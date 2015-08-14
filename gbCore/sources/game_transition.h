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
    class game_transition : public game_loop_interface, public ces_entity, public ces_configuration_component_interface
    {
    private:
        
        std::string m_guid;
        std::shared_ptr<ces_systems_feeder> m_system_feeder;
        
    protected:
        
        void on_update(f32 deltatime);
        
        friend class game_controller;
        void on_activated(void);
        void on_deactivated(void);
        
        virtual void on_configuration_loaded(const std::shared_ptr<configuration>& configuration, bool success);
        
    public:
        
        game_transition(const std::string& guid, bool is_offscreen);
        virtual ~game_transition(void);
        
        virtual void create_scene(void);
        std::string get_guid(void) const;
    };
};

#endif
