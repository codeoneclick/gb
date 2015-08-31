//
//  demo_game_scene.cpp
//  gbDemo
//
//  Created by sergey.sergeev on 8/27/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "demo_game_scene.h"
#include "game_transition.h"
#include "fabricator.h"
#include "scene_graph.h"
#include "camera.h"
#include "global_light.h"
#include "model3d_animated.h"

demo_game_scene::demo_game_scene(const gb::game_transition_shared_ptr& transition) :
gb::game_scene(transition)
{
    m_camera = game_scene::get_transition()->get_fabricator()->create_camera(45.f, .1f, 1024.f, glm::ivec4(0.f, 0.f, 1024.f, 768.f));
    gb::global_light_shared_ptr global_light = game_scene::get_transition()->get_fabricator()->create_global_light(45.f , .1f, 1024.f);
    
    m_models["human_02"] = game_scene::get_transition()->get_fabricator()->create_model3d_animated("gameobject.human_02.xml");
    m_models["orc_01"] = game_scene::get_transition()->get_fabricator()->create_model3d_animated("gameobject.orc_01.xml");
    m_models["orc_02"] = game_scene::get_transition()->get_fabricator()->create_model3d_animated("gameobject.orc_02.xml");
    
    game_scene::get_transition()->get_scene_graph()->set_camera(m_camera);
    game_scene::get_transition()->get_scene_graph()->set_global_light(global_light);
    game_scene::get_transition()->get_scene_graph()->add_game_object(m_models["human_02"]);
    game_scene::get_transition()->get_scene_graph()->add_game_object(m_models["orc_01"]);
    game_scene::get_transition()->get_scene_graph()->add_game_object(m_models["orc_02"]);
    
    m_camera->set_position(glm::vec3(0.f));
    m_camera->set_look_at(glm::vec3(0.f, 1.f, 0.f));
    m_camera->set_distance_to_look_at(glm::vec3(4.f));
    
    global_light->set_angle(3.f);
    global_light->set_distance_to_sun(512.f);
    global_light->set_distance_to_look_at(64.f);
    global_light->set_rotation_center(glm::vec3(256.f, .0f, 256.f));
    
    m_models["human_02"]->set_rotation(glm::vec3(0.f, -45.f, 0.f));
    m_models["orc_01"]->set_rotation(glm::vec3(0.f, -90.f, 0.f));
    m_models["orc_02"]->set_rotation(glm::vec3(0.f, -90.f, 0.f));
    
    m_models["orc_01"]->set_position(glm::vec3(0.f, 0.f, -2.f));
    m_models["orc_02"]->set_position(glm::vec3(0.f, 0.f, 2.f));
}

demo_game_scene::~demo_game_scene()
{
    
}

void demo_game_scene::update(f32 deltatime)
{
    /*static f32 angle = 0.f;
    angle += 1.f;
    m_model->set_rotation(glm::vec3(0.f, angle, 0.f));
    m_camera->set_distance_to_look_at(glm::vec3(6.f + sinf(angle * .01f)));*/
    m_models["human_02"]->set_animation("IDLE");
    m_models["orc_01"]->set_animation("IDLE");
    m_models["orc_02"]->set_animation("IDLE");
}