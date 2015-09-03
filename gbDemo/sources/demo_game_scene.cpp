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
#include "particle_emitter.h"
#include "model3d_animated.h"

demo_game_scene::demo_game_scene(const gb::game_transition_shared_ptr& transition) :
gb::game_scene(transition)
{
    m_camera = game_scene::get_transition()->get_fabricator()->create_camera(45.f, .1f, 128.f, glm::ivec4(0.f, 0.f,
                                                                                                           game_scene::get_transition()->get_width(),
                                                                                                           game_scene::get_transition()->get_height()));
    gb::global_light_shared_ptr global_light = game_scene::get_transition()->get_fabricator()->create_global_light(45.f , .1f, 128.f);
    
    m_models["human_02"] = game_scene::get_transition()->get_fabricator()->create_model3d_animated("gameobject.human_02.xml");
    m_models["orc_01"] = game_scene::get_transition()->get_fabricator()->create_model3d_animated("gameobject.orc_01.xml");
    m_models["orc_02"] = game_scene::get_transition()->get_fabricator()->create_model3d_animated("gameobject.orc_02.xml");
    
    gb::particle_emitter_shared_ptr particle_emitter_fire = game_scene::get_transition()->get_fabricator()->create_particle_emitter("gameobject.particle.emitter.fire.xml");
    gb::particle_emitter_shared_ptr particle_emitter_smoke = game_scene::get_transition()->get_fabricator()->create_particle_emitter("gameobject.particle.emitter.smoke.xml");
    
    gb::model3d_static_shared_ptr plane = game_scene::get_transition()->get_fabricator()->create_model3d_static("gameobject.plane.xml");
    plane->set_scale(glm::vec3(8.f, 1.f, 8.f));
    plane->set_position(glm::vec3(-4.f, 0.f, -4.f));
    
    game_scene::get_transition()->get_scene_graph()->set_camera(m_camera);
    game_scene::get_transition()->get_scene_graph()->set_global_light(global_light);
    game_scene::get_transition()->get_scene_graph()->add_game_object(m_models["human_02"]);
    game_scene::get_transition()->get_scene_graph()->add_game_object(m_models["orc_01"]);
    game_scene::get_transition()->get_scene_graph()->add_game_object(m_models["orc_02"]);
    
    game_scene::get_transition()->get_scene_graph()->add_game_object(particle_emitter_fire);
    game_scene::get_transition()->get_scene_graph()->add_game_object(particle_emitter_smoke);
    
    game_scene::get_transition()->get_scene_graph()->add_game_object(plane);
    
    m_camera->set_position(glm::vec3(0.f));
    m_camera->set_look_at(glm::vec3(0.f, 1.f, 0.f));
    m_camera->set_distance_to_look_at(glm::vec3(4.f));
    
    particle_emitter_fire->set_position(glm::vec3(2.f, 0.f, 2.f));
    particle_emitter_smoke->set_position(glm::vec3(2.f, 0.f, -2.f));
    
    //global_light->set_angle(0.f);
    //global_light->set_distance_to_sun(4.f);
    //global_light->set_distance_to_look_at(4.f);
    //global_light->set_rotation_center(glm::vec3(0.f, .0f, 0.f));
    global_light->set_position(glm::vec3(-4.f, 8.f, 4.f));
    global_light->set_look_at(glm::vec3(0.f, 1.f, 0.f));
    
    m_models["human_02"]->set_rotation(glm::vec3(0.f, -45.f, 0.f));
    m_models["orc_01"]->set_rotation(glm::vec3(0.f, -90.f, 0.f));
    m_models["orc_02"]->set_rotation(glm::vec3(0.f, -90.f, 0.f));
    
    m_models["orc_01"]->set_position(glm::vec3(0.f, 0.f, -2.f));
    m_models["orc_02"]->set_position(glm::vec3(0.f, 0.f, 2.f));

    m_models["human_02"]->set_touches_receives_enabled(true);
    m_models["human_02"]->set_debug_draw_enabled(true);
}

demo_game_scene::~demo_game_scene()
{
    
}

void demo_game_scene::update(f32 deltatime)
{
    m_models["human_02"]->set_animation("IDLE");
    m_models["orc_01"]->set_animation("IDLE");
    m_models["orc_02"]->set_animation("IDLE");
}

void demo_game_scene::on_touch(const glm::vec3 &point, const gb::ces_entity_shared_ptr &listener,
                               gb::e_input_element input_element, gb::e_input_state input_state)
{
    if(input_state == gb::e_input_state_pressed)
    {
        std::cout<<"on_touch"<<std::endl;
    }
}