//
//  scene_graph.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/26/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "scene_graph.h"
#include "camera.h"
#include "frustum.h"
#include "global_light.h"
#include "game_object.h"
#include "omni_light.h"
#include "direction_light.h"
#include "ces_systems_feeder.h"

namespace gb
{
    scene_graph::scene_graph(const ces_systems_feeder_shared_ptr& systems_feeder) :
    m_systems_feeder(systems_feeder),
    m_camera(nullptr),
    m_global_light(nullptr)
    {

    }
    
    scene_graph::~scene_graph()
    {
        
    }
    
    void scene_graph::on_update(f32 deltatime)
    {
        if(m_camera)
        {
            m_camera->update(deltatime);
        }
        if(m_global_light)
        {
            m_global_light->update(deltatime);
        }
    }
    
    void scene_graph::set_camera(const camera_shared_ptr &camera)
    {
        m_camera = camera;
    }
    
    camera_shared_ptr scene_graph::get_camera() const
    {
        return m_camera;
    }
    
    void scene_graph::set_global_light(const global_light_shared_ptr &global_light)
    {
        m_global_light = global_light;
    }
    
    global_light_shared_ptr scene_graph::get_global_light() const
    {
        return m_global_light;
    }
    
    void scene_graph::add_game_object(const game_object_shared_ptr& game_object)
    {
        game_object->on_added_to_scene(shared_from_this());
        m_game_objects_container.insert(game_object);
        m_systems_feeder->add_entity(game_object);
    }
    
    void scene_graph::remove_game_object(const game_object_shared_ptr& game_object)
    {
        game_object->on_removed_from_scene();
        m_game_objects_container.erase(game_object);
        m_systems_feeder->remove_entity(game_object);
    }
    
    void scene_graph::add_omni_light(const omni_light_shared_ptr& omni_light)
    {
        omni_light->on_added_to_scene(shared_from_this());
        m_omni_lights_container.insert(omni_light);
        m_systems_feeder->add_entity(omni_light);
    }
    
    void scene_graph::remove_omni_light(const omni_light_shared_ptr& omni_light)
    {
        omni_light->on_removed_from_scene();
        m_omni_lights_container.erase(omni_light);
        m_systems_feeder->remove_entity(omni_light);
    }
    
    void scene_graph::add_direction_light(const direction_light_shared_ptr& direction_light)
    {
        direction_light->on_added_to_scene(shared_from_this());
        m_direction_lights_container.insert(direction_light);
        m_systems_feeder->add_entity(direction_light);
    }
    
    void scene_graph::remove_direction_light(const direction_light_shared_ptr& direction_light)
    {
        direction_light->on_removed_from_scene();
        m_direction_lights_container.erase(direction_light);
        m_systems_feeder->remove_entity(direction_light);
    }
}