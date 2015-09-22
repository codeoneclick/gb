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
#include "omni_light.h"
#include "direction_light.h"
#include "particle_emitter.h"
#include "model3d_animated.h"

demo_game_scene::demo_game_scene(const gb::game_transition_shared_ptr& transition) :
gb::game_scene(transition)
{
    m_camera = game_scene::get_transition()->get_fabricator()->create_camera(45.f, .1f, 128.f, glm::ivec4(0.f, 0.f,
                                                                                                          game_scene::get_transition()->get_width(),
                                                                                                          game_scene::get_transition()->get_height()));
    m_global_light = game_scene::get_transition()->get_fabricator()->create_global_light(45.f , .1f, 128.f);
    
    m_models["human_02"] = game_scene::get_transition()->get_fabricator()->create_model3d_animated("gameobject.human_02.xml");
    m_models["orc_01"] = game_scene::get_transition()->get_fabricator()->create_model3d_animated("gameobject.orc_01.xml");
    m_models["orc_02"] = game_scene::get_transition()->get_fabricator()->create_model3d_animated("gameobject.orc_02.xml");
    
    gb::particle_emitter_shared_ptr particle_emitter_fire = game_scene::get_transition()->get_fabricator()->create_particle_emitter("gameobject.particle.emitter.fire.xml");
    gb::particle_emitter_shared_ptr particle_emitter_smoke = game_scene::get_transition()->get_fabricator()->create_particle_emitter("gameobject.particle.emitter.smoke.xml");
    
    gb::model3d_static_shared_ptr plane = game_scene::get_transition()->get_fabricator()->create_model3d_static("gameobject.plane.xml");
    plane->set_scale(glm::vec3(32.f, 1.f, 32.f));
    plane->set_position(glm::vec3(-8.f, 0.f, -8.f));
    
    game_scene::get_transition()->get_scene_graph()->set_camera(m_camera);
    game_scene::get_transition()->get_scene_graph()->set_global_light(m_global_light);
    game_scene::get_transition()->get_scene_graph()->add_game_object(m_models["human_02"]);
    game_scene::get_transition()->get_scene_graph()->add_game_object(m_models["orc_01"]);
    game_scene::get_transition()->get_scene_graph()->add_game_object(m_models["orc_02"]);
    
    //game_scene::get_transition()->get_scene_graph()->add_game_object(particle_emitter_fire);
    //game_scene::get_transition()->get_scene_graph()->add_game_object(particle_emitter_smoke);
    
    game_scene::get_transition()->get_scene_graph()->add_game_object(plane);
    
    m_camera->set_position(glm::vec3(0.f));
    m_camera->set_look_at(glm::vec3(16.f, 1.f, 16.f));
    m_camera->set_distance_to_look_at(glm::vec3(16.f));
    
    //particle_emitter_fire->set_position(glm::vec3(2.f, 0.f, 2.f));
    //particle_emitter_smoke->set_position(glm::vec3(2.f, 0.f, -2.f));
    
    m_global_light->set_position(glm::vec3(-4.f, 8.f, 4.f));
    m_global_light->set_look_at(glm::vec3(0.f, 1.f, 0.f));
    
    m_models["human_02"]->set_rotation(glm::vec3(0.f, -90.f, 0.f));
    m_models["orc_01"]->set_rotation(glm::vec3(0.f, -90.f, 0.f));
    m_models["orc_02"]->set_rotation(glm::vec3(0.f, -90.f, 0.f));
    
    m_models["human_02"]->set_position(glm::vec3(16.f, 0.f, 16.f));
    m_models["orc_01"]->set_position(glm::vec3(16.f, 0.f, 14.f));
    m_models["orc_02"]->set_position(glm::vec3(16.f, 0.f, 18.f));
    
    m_models["human_02"]->set_touches_receives_enabled(true);
    m_models["human_02"]->set_debug_draw_enabled(true);
    
    m_models["orc_01"]->set_debug_draw_enabled(true);
    m_models["orc_02"]->set_debug_draw_enabled(true);
    
    m_omni_lights["omni_light_01"] = game_scene::get_transition()->get_fabricator()->create_omni_light();
    game_scene::get_transition()->get_scene_graph()->add_omni_light(m_omni_lights["omni_light_01"]);
    m_omni_lights["omni_light_01"]->set_radius(4.f);
    m_omni_lights["omni_light_01"]->set_position(glm::vec3(16.f, 1.f, 16.f));
    
    m_omni_lights["omni_light_02"] = game_scene::get_transition()->get_fabricator()->create_omni_light();
    game_scene::get_transition()->get_scene_graph()->add_omni_light(m_omni_lights["omni_light_02"]);
    m_omni_lights["omni_light_02"]->set_radius(8.f);
    m_omni_lights["omni_light_02"]->set_position(glm::vec3(16.f, 3.f, 16.f));
    
    m_direction_light = game_scene::get_transition()->get_fabricator()->create_direction_light();
    game_scene::get_transition()->get_scene_graph()->add_direction_light(m_direction_light);
    m_direction_light->set_direction(glm::vec3(-1.f, 1.f, 0.f));
}

demo_game_scene::~demo_game_scene()
{
    
}

void demo_game_scene::update(f32 deltatime)
{
    m_models["human_02"]->set_animation("IDLE");
    m_models["orc_01"]->set_animation("IDLE");
    m_models["orc_02"]->set_animation("IDLE");
    
    /*static f32 angle = 0.f;
    angle += .01f;
    m_camera->set_rotation(angle);
    m_models["human_02"]->set_rotation(glm::vec3(0.f, angle, 0.f));*/
    
    static f32 angle = 0.f;
    angle += 0.05f;
    glm::vec2 light_xz_position = glm::vec2(0.f);
    light_xz_position.x = 16.f + m_global_light->get_look_at().x + cosf(angle) * -4.f;
    light_xz_position.y = 16.f + m_global_light->get_look_at().z + sinf(angle) * -4.f;
    m_omni_lights["omni_light_01"]->set_position(glm::vec3(light_xz_position.x, 1.f, light_xz_position.y));
    //m_camera->set_rotation(angle * .1f);
}

void demo_game_scene::on_touch(const glm::vec3 &point, const gb::ces_entity_shared_ptr &listener,
                               gb::e_input_element input_element, gb::e_input_state input_state)
{
    if(input_state == gb::e_input_state_pressed)
    {
        std::cout<<"on_touch"<<std::endl;
    }
}