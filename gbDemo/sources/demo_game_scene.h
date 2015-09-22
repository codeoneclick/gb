//
//  demo_game_scene.h
//  gbDemo
//
//  Created by sergey.sergeev on 8/27/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef demo_game_scene_h
#define demo_game_scene_h

#include "game_scene.h"
#include "declarations.h"
#include "ces_input_system.h"

class demo_game_scene : public gb::game_scene, public gb::touch_listener
{
private:
    
    gb::camera_shared_ptr m_camera;
    gb::global_light_shared_ptr m_global_light;
    gb::direction_light_shared_ptr m_direction_light;
    std::map<std::string, gb::model3d_animated_shared_ptr> m_models;
    std::map<std::string, gb::omni_light_shared_ptr> m_omni_lights;
    
protected:
    
    void update(f32 deltatime);
    
    void on_touch(const glm::vec3& point, const gb::ces_entity_shared_ptr& listener,
                  gb::e_input_element input_element, gb::e_input_state input_state);
    
public:
    
    demo_game_scene(const gb::game_transition_shared_ptr& transition);
    ~demo_game_scene();
};

#endif
