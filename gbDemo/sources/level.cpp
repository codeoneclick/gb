//
//  level.cpp
//  gbDemo
//
//  Created by sergey.sergeev on 10/6/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "level.h"
#include "scene_fabricator.h"
#include "scene_graph.h"
#include "instanced_models3d_static.h"

namespace koth
{
    level::level(const gb::scene_fabricator_shared_ptr& fabricator,
                 const gb::scene_graph_shared_ptr& graph) :
    m_fabricator(fabricator),
    m_graph(graph)
    {
        m_size = glm::ivec2(16);
        m_boxes_size = glm::vec2(1.f);
        m_boxes_offset = .005f;
    }
    
    level::~level()
    {
        
    }
    
    void level::construct(const std::string& filename)
    {
        m_graph->set_box2d_world(-(m_boxes_size * 2.f), glm::vec2(m_size.x * (m_boxes_size.x + m_boxes_offset) + m_boxes_size.x,
                                                                  m_size.y * (m_boxes_size.y + m_boxes_offset) + m_boxes_size.y));
        
        m_boxes.resize(m_size.x, nullptr);
        m_boxes_states.resize(m_size.x * m_size.y, e_level_box_state_none);
        
        for(i32 i = 0; i < m_size.x; ++i)
        {
            m_boxes[i] = m_fabricator->create_instanced_models3d_static("gameobject.instanced.boxes.xml", m_size.x);
            m_graph->add_game_object(m_boxes[i]);
            for (i32 j = 0; j < m_size.y; ++j)
            {
                m_boxes[i]->set_position(glm::vec3(i * (m_boxes_size.x + m_boxes_offset), 0.f, j * (m_boxes_size.y + m_boxes_offset)), j);
                m_boxes[i]->set_scale(glm::vec3(m_boxes_size.x), j);
            }
        }
    }
    
    void level::update(f32 deltatime)
    {
        for(i32 i = 0; i < m_size.x; ++i)
        {
            for(i32 j = 0; j < m_size.y; ++j)
            {
                switch (m_boxes_states[i + j * m_size.x])
                {
                    case e_level_box_state_fall_down:
                    {
                        glm::vec3 position = m_boxes[i]->get_position(j);
                        glm::vec3 rotation = m_boxes[i]->get_rotation(j);
                        
                        if(position.y > -24.f)
                        {
                            position.y -= 0.05;
                            rotation.x += 2.5;
                            rotation.z += 0.5;
                        }
                        else
                        {
                            m_boxes_states[i + j * m_size.x] = e_level_box_state_drop_down;
                            position.y = 24.f;
                            rotation = glm::vec3(0.f);
                        }
                        
                        m_boxes[i]->set_position(position, j);
                        m_boxes[i]->set_rotation(rotation, j);
                    }
                        break;
                        
                    case e_level_box_state_drop_down:
                    {
                        glm::vec3 position = m_boxes[i]->get_position(j);
                        glm::vec3 rotation = m_boxes[i]->get_rotation(j);
                        
                        if(position.y > 0.f)
                        {
                            position.y -= 0.1;
                        }
                        else
                        {
                            m_boxes_states[i + j * m_size.x] = e_level_box_state_none;
                        }
                        
                        m_boxes[i]->set_position(position, j);
                        m_boxes[i]->set_rotation(rotation, j);
                    }
                        break;
                        
                    default:
                        break;
                }
            }
        }
    }
    
    void level::set_box_state(i32 x, i32 z)
    {
        if(x < m_size.x && z < m_size.y &&
           x >= 0 && z >= 0 &&
           m_boxes_states[x + z * m_size.x] == e_level_box_state_none)
        {
            m_boxes_states[x + z * m_size.x] = e_level_box_state_fall_down;
        }
    }
}