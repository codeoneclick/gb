//
//  ai_character_controller.cpp
//  gbDemo
//
//  Created by sergey.sergeev on 10/15/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#include "ai_character_controller.h"
#include "game_object.h"
#include "level.h"
#include "ai_actions_processor.h"
#include "ai_move_action.h"
#include "model3d_animated.h"
#include "glm_extensions.h"

namespace koth
{
    ai_character_controller::ai_character_controller(const gb::game_object_shared_ptr& game_object) :
    koth::character_controller(game_object, nullptr),
    m_goal_position_index(glm::ivec2(-1))
    {
        m_pathfinder = std::make_shared<pathfinder>();
        
        m_map = new std::shared_ptr<level_node>*[16];
        for(ui32 x = 0; x < 16; ++x)
        {
            m_map[x] = new std::shared_ptr<level_node>[16];
            for(ui32 y = 0; y < 16; ++y)
            {
                m_map[x][y] = std::make_shared<level_node>();
                m_map[x][y]->set_position(x, y);
                m_map[x][y]->set_type(true);
            }
        }
        
        int newX, newY;
        std::shared_ptr<level_node> child;
        for(int x = 0; x < 16; ++x)
        {
            for(int y = 0; y < 16; ++y)
            {
                for(int i = -1; i < 2; ++i)
                {
                    newX = m_map[x][y]->get_x() + i;
                    for(int j = -1; j < 2; ++j)
                    {
                        newY = m_map[x][y]->get_y() + j;
                        if( newX > -1 && newX < 16 && newY > -1 && newY < 16)
                        {
                            child = m_map[newX][newY];
                            if(child->get_type() && (newX != x || newY != y) )
                            {
                                m_map[x][y]->add_child(child, m_map[x][y]->distance_to_local(child));
                            }
                        }
                    }
                }
            }
        }
        
        m_actions_processor = std::make_shared<ai_actions_processor>();
    }
    
    ai_character_controller::~ai_character_controller()
    {
        
    }
    
    void ai_character_controller::update(f32 deltatime)
    {
        m_actions_processor->update(deltatime);
        
        gb::model3d_animated_shared_ptr animated_model = std::static_pointer_cast<gb::model3d_animated>(m_game_object);
        
        if(!m_actions_processor->is_actions_exist() &&
           m_goal_position_index.x != -1 && m_goal_position_index.y != -1)
        {
            std::vector<std::shared_ptr<astar_node>> path;
            
            glm::ivec2 current_position_index = glm::ivec2(ceilf(m_game_object->get_position().x),
                                                           ceilf(m_game_object->get_position().z));
            
            m_pathfinder->set_start(m_map[current_position_index.x][current_position_index.y]);
            m_pathfinder->set_goal(m_map[m_goal_position_index.x][m_goal_position_index.y]);
            
            bool is_found = m_pathfinder->find_path(path);
            if(is_found)
            {
                if(path.size() > 1)
                {
                    path.resize(path.size() - 1);
                    std::reverse(path.begin(), path.end());
                    
                    glm::vec3 previous_position = m_game_object->get_position();
                    for(const auto& point : path)
                    {
                        ai_move_action_shared_ptr move_action = std::make_shared<ai_move_action>();
                        move_action->set_parameters(previous_position, glm::vec3(point->get_x(),
                                                                                 m_game_object->get_position().y,
                                                                                 point->get_y()));
                        
                        previous_position = glm::vec3(point->get_x(),
                                                      m_game_object->get_position().y,
                                                      point->get_y());
                        
                        move_action->set_in_progress_callback([this, animated_model](const ai_action_shared_ptr& action) {
                            
                            ai_move_action_shared_ptr move_action = std::static_pointer_cast<ai_move_action>(action);
                            character_controller::set_position(move_action->get_position());
                            character_controller::set_rotation(glm::mix(m_game_object->get_rotation(),
                                                                        glm::vec3(.0f, glm::degrees(glm::wrap_angle(move_action->get_rotation())), .0f), .1f));
                            
                            animated_model->set_animation("RUN");
                        });
                        
                        m_actions_processor->add_action(move_action);
                    }
                }
                else
                {
                    animated_model->set_animation("IDLE");
                }
            }
        }
        character_controller::update(deltatime);
    }
    
    void ai_character_controller::set_goal_position(const glm::vec3& position)
    {
        m_goal_position_index.x = std::max(0, std::min(static_cast<i32>(position.x), 16));
        m_goal_position_index.y = std::max(0, std::min(static_cast<i32>(position.z), 16));
    }
    
    void ai_character_controller::set_position(const glm::vec3& position)
    {
        character_controller::set_position(position);
    }
}