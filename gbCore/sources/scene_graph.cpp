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
#include "shadow_cast_light.h"
#include "game_object.h"
#include "omni_light.h"
#include "instanced_omni_lights.h"
#include "direction_light.h"
#include "ces_systems_feeder.h"

namespace gb
{
    scene_graph::scene_graph() :
    m_camera(nullptr),
    m_shadow_cast_light(nullptr)
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
        if(m_shadow_cast_light)
        {
            m_shadow_cast_light->update(deltatime);
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
    
    void scene_graph::set_shadow_cast_light(const shadow_cast_light_shared_ptr &shadow_cast_light)
    {
        m_shadow_cast_light = shadow_cast_light;
    }
    
    shadow_cast_light_shared_ptr scene_graph::get_shadow_cast_light() const
    {
        return m_shadow_cast_light;
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
    
    void scene_graph::add_instanced_omni_lights(const instanced_omni_lights_shared_ptr& instanced_omni_lights)
    {
        instanced_omni_lights->on_added_to_scene(shared_from_this());
        m_instanced_omni_lights_container.insert(instanced_omni_lights);
        m_systems_feeder->add_entity(instanced_omni_lights);
    }
    
    void scene_graph::remove_instanced_omni_lights(const instanced_omni_lights_shared_ptr& instanced_omni_lights)
    {
        instanced_omni_lights->on_removed_from_scene();
        m_instanced_omni_lights_container.erase(instanced_omni_lights);
        m_systems_feeder->remove_entity(instanced_omni_lights);
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