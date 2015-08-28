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

class demo_game_scene : public gb::game_scene
{
private:
    
    gb::camera_shared_ptr m_camera;
    gb::model3d_animated_shared_ptr m_model;
    
protected:
    
    void update(f32 deltatime);
    
public:
    
    demo_game_scene(const gb::game_transition_shared_ptr& transition);
    ~demo_game_scene();
};

#endif
