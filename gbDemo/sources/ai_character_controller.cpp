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

namespace koth
{
    ai_character_controller::ai_character_controller(const gb::game_object_shared_ptr& game_object) :
    koth::character_controller(game_object, nullptr),
    m_target_position(glm::ivec2(0))
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
    }
    
    ai_character_controller::~ai_character_controller()
    {
        
    }
    
    void ai_character_controller::update(f32 deltatime)
    {
        std::vector<std::shared_ptr<astar_node>> path;
        
        glm::ivec2 current_position = glm::ivec2(m_game_object->get_position().x,
                                                 m_game_object->get_position().z);
        
        m_pathfinder->set_start(m_map[current_position.x][current_position.y]);
        m_pathfinder->set_goal(m_map[m_target_position.x][m_target_position.y]);
        
        bool is_found = m_pathfinder->find_path(path);
        if(is_found)
        {
            if(path.size() > 1)
            {
                path.resize(path.size() - 1);
                std::reverse(path.begin(), path.end());
                
                character_controller::set_position(glm::mix(m_game_object->get_position(), glm::vec3(path.at(0)->get_x(),
                                                                                                     m_game_object->get_position().y,
                                                                                                     path.at(0)->get_y()),
                                                            .51f));
            }
            
        }
        /*
         if(current_position != m_target_position)
         {
            current_position = glm::ivec2(1, 1);
            m_target_position = glm::ivec2(1, 7);
            std::vector<glm::ivec2> path = m_pathfinder->get_path(current_position,
                                                                  m_target_position);
            if(path.size() > 1)
            {
                path.resize(path.size() - 1);
                std::reverse(path.begin(), path.end());
                
                character_controller::set_position(glm::mix(m_game_object->get_position(), glm::vec3(path.at(0).x, m_game_object->get_position().y, path.at(0).y),
                                                            .05f));
            }
        }*/
        character_controller::update(deltatime);
    }
    
    void ai_character_controller::set_target_position(const glm::vec3& position)
    {
        m_target_position.x = std::max(0, std::min(static_cast<i32>(position.x), 15));
        m_target_position.y = std::max(0, std::min(static_cast<i32>(position.z), 15));
    }
}