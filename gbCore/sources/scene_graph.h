//
//  scene_graph.h
//  gbCore
//
//  Created by sergey.sergeev on 8/26/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef scene_graph_h
#define scene_graph_h

#include "main_headers.h"
#include "declarations.h"
#include "game_loop.h"

namespace gb
{
    class scene_graph : public game_loop_interface, public std::enable_shared_from_this<scene_graph>
    {
    private:
        
    protected:
        
        ces_systems_feeder_shared_ptr m_systems_feeder;
        
        camera_shared_ptr m_camera;
        global_light_shared_ptr m_global_light;
        std::set<game_object_shared_ptr> m_game_objects_container;
        std::set<omni_light_shared_ptr> m_omni_lights_container;
        
        void on_update(f32 deltatime);
        
    public:
        
        scene_graph(const ces_systems_feeder_shared_ptr& systems_feeder);
        ~scene_graph();
        
        void set_camera(const camera_shared_ptr& camera);
        camera_shared_ptr get_camera(void) const;
        
        void set_global_light(const global_light_shared_ptr& global_light);
        global_light_shared_ptr get_global_light(void) const;
        
        void add_game_object(const game_object_shared_ptr& game_object);
        void remove_game_object(const game_object_shared_ptr& game_object);
        
        void add_omni_light(const omni_light_shared_ptr& omni_light);
        void remove_omni_light(const omni_light_shared_ptr& omni_light);
    };
};

#endif
