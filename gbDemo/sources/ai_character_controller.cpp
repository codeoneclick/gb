//
//  ai_character_controller.cpp
//  gbDemo
//
//  Created by sergey.sergeev on 10/15/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#include "ai_character_controller.h"
#include "pathfinder.h"
#include "game_object.h"

namespace koth
{
    ai_character_controller::ai_character_controller(const gb::game_object_shared_ptr& game_object) :
    koth::character_controller(game_object, nullptr),
    m_target_position(glm::ivec2(0))
    {
        m_pathfinder = std::make_shared<pathfinder>(glm::ivec2(16));
    }
    
    ai_character_controller::~ai_character_controller()
    {
        
    }
    
    void ai_character_controller::update(f32 deltatime)
    {
        glm::ivec2 current_position = glm::ivec2(m_game_object->get_position().x,
                                                 m_game_object->get_position().z);
        if(current_position != m_target_position)
        {
            std::vector<glm::ivec2> path = m_pathfinder->get_path(current_position,
                                                                  m_target_position);
            if(path.size() > 1)
            {
                path.resize(path.size() - 1);
                std::reverse(path.begin(), path.end());
                
                character_controller::set_position(glm::mix(m_game_object->get_position(), glm::vec3(path.at(0).x, m_game_object->get_position().y, path.at(0).y),
                                                            .05f));
            }
        }
        character_controller::update(deltatime);
    }
    
    void ai_character_controller::set_target_position(const glm::vec3& position)
    {
        m_target_position.x = std::max(1, std::min(static_cast<i32>(position.x), 15));
        m_target_position.y = std::max(1, std::min(static_cast<i32>(position.z), 15));
    }
}