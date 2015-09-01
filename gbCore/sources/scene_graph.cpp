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
#include "ces_systems_feeder.h"
#include "ces_camera_component.h"
#include "ces_frustum_culling_component.h"
#include "ces_global_light_component.h"

namespace gb
{
    scene_graph::scene_graph(const ces_systems_feeder_shared_ptr& systems_feeder) :
    m_systems_feeder(systems_feeder),
    m_camera(nullptr),
    m_global_light(nullptr)
    {
        m_frustum = std::make_shared<frustum>();
    }
    
    scene_graph::~scene_graph()
    {
        
    }
    
    void scene_graph::on_update(f32 deltatime)
    {
        if(m_camera)
        {
            m_camera->update(deltatime);
            m_frustum->update(m_camera->get_fov(), m_camera->get_aspect(), m_camera->get_near(), m_camera->get_far(),
                              m_camera->get_position(), m_camera->get_up(), m_camera->get_look_at());
        }
        if(m_global_light)
        {
            m_global_light->update(deltatime);
        }
    }
    
    void scene_graph::set_camera(const camera_shared_ptr &camera)
    {
        m_camera = camera;
        for(const auto& iterator : m_game_objects_container)
        {
            if(iterator->is_component_exist(e_ces_component_type_camera))
            {
                assert(m_camera);
                ces_camera_component_shared_ptr camera_component = std::static_pointer_cast<ces_camera_component>(iterator->get_component(e_ces_component_type_camera));
                camera_component->set_camera(m_camera);
            }
        }
    }
    
    camera_shared_ptr scene_graph::get_camera() const
    {
        return m_camera;
    }
    
    void scene_graph::set_global_light(const global_light_shared_ptr &global_light)
    {
        m_global_light = global_light;
        for(const auto& iterator : m_game_objects_container)
        {
            if(iterator->is_component_exist(e_ces_component_type_global_light))
            {
                assert(m_global_light);
                ces_global_light_component_shared_ptr global_light_component = std::static_pointer_cast<ces_global_light_component>(iterator->get_component(e_ces_component_type_global_light));
                global_light_component->set_global_light(m_global_light);
            }
        }
    }
    
    global_light_shared_ptr scene_graph::get_global_light() const
    {
        return m_global_light;
    }
    
    void scene_graph::add_game_object(const game_object_shared_ptr& game_object)
    {
        if(game_object->is_component_exist(e_ces_component_type_camera))
        {
            assert(m_camera);
            ces_camera_component_shared_ptr camera_component = std::static_pointer_cast<ces_camera_component>(game_object->get_component(e_ces_component_type_camera));
            camera_component->set_camera(m_camera);
        }
        if(game_object->is_component_exist(e_ces_component_type_frustum_culling))
        {
            assert(m_frustum);
            ces_frustum_culling_component_shared_ptr frustum_culling_component =
            std::static_pointer_cast<ces_frustum_culling_component>(game_object->get_component(e_ces_component_type_frustum_culling));
            frustum_culling_component->set_frustum(m_frustum);
        }
        if(game_object->is_component_exist(e_ces_component_type_global_light))
        {
            assert(m_global_light);
            ces_global_light_component_shared_ptr global_light_component = std::static_pointer_cast<ces_global_light_component>(game_object->get_component(e_ces_component_type_global_light));
            global_light_component->set_global_light(m_global_light);
        }
        
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
}