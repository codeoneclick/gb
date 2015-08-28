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

namespace gb
{
    class graphics_context;
    class input_context;

    class game_transition : public game_loop_interface, public std::enable_shared_from_this<game_transition>
    {
    private:
        
        std::string m_guid;
        bool m_offscreen;
        ces_systems_feeder_shared_ptr m_system_feeder;
        fabricator_shared_ptr m_fabricator;
        scene_graph_shared_ptr m_scene_graph;
        
    protected:
        
        void on_update(f32 deltatime);
        
        virtual void create_scene() = 0;
        virtual void destroy_scene() = 0;
        
        game_scene_shared_ptr m_scene;
        
    public:
        
        game_transition(const std::string& guid, bool is_offscreen);
        virtual ~game_transition();
        
        void on_activated(const std::shared_ptr<graphics_context>& graphics_context,
                          const std::shared_ptr<input_context>& input_context,
                          const configuration_accessor_shared_ptr& configurations_accessor,
                          const resource_accessor_shared_ptr& resource_accessor);
        
        void on_deactivated(void);
        
        std::string get_guid() const;
        
        fabricator_shared_ptr get_fabricator() const;
        scene_graph_shared_ptr get_scene_graph() const;
    };
};

#endif
