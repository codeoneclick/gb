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
#include "game_graph_interface.h"

#define scene_graph_id 0
#define scene_graph_inst std::static_pointer_cast<gb::scene_graph>(game_scene::get_transition()->get_graph(scene_graph_id))

namespace gb
{
    class scene_graph : public game_graph_interface,
    public std::enable_shared_from_this<scene_graph>
    {
    private:
        
    protected:
        
        camera_shared_ptr m_camera;
        shadow_cast_light_shared_ptr m_shadow_cast_light;
        sky_box_shared_ptr m_sky_box;
        std::set<game_object_shared_ptr> m_game_objects_container;
        std::set<omni_light_shared_ptr> m_omni_lights_container;
        std::set<instanced_omni_lights_shared_ptr> m_instanced_omni_lights_container;
        std::set<direction_light_shared_ptr> m_direction_lights_container;
        
        void on_update(f32 deltatime);
        
    public:
        
        scene_graph();
        ~scene_graph();
        
        void set_camera(const camera_shared_ptr& camera);
        camera_shared_ptr get_camera() const;
        
        void set_shadow_cast_light(const shadow_cast_light_shared_ptr& shadow_cast_light);
        shadow_cast_light_shared_ptr get_shadow_cast_light() const;
        
        void set_sky_box(const sky_box_shared_ptr& sky_box);
        
        void add_game_object(const game_object_shared_ptr& game_object);
        void remove_game_object(const game_object_shared_ptr& game_object);
        
        void add_omni_light(const omni_light_shared_ptr& omni_light);
        void remove_omni_light(const omni_light_shared_ptr& omni_light);
        
        void add_instanced_omni_lights(const instanced_omni_lights_shared_ptr& instanced_omni_lights);
        void remove_instanced_omni_lights(const instanced_omni_lights_shared_ptr& instanced_omni_lights);
        
        void add_direction_light(const direction_light_shared_ptr& direction_light);
        void remove_direction_light(const direction_light_shared_ptr& direction_light);
        
        void set_box2d_world(const glm::vec2 &min_bound, const glm::vec2 &max_bound);
        
        b2Body* add_box2d_body(const std::shared_ptr<b2BodyDef> box2d_body_definition);
        void remove_box2d_body(b2Body* box2d_body);
    };
};

#endif
