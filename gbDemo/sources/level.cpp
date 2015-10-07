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
        m_boxes_size = glm::vec2(2.f);
        m_boxes_offset = .05f;
    }
    
    level::~level()
    {
        
    }
    
    void level::construct(const std::string& filename)
    {
        m_graph->set_box2d_world(glm::vec2(0.f), glm::vec2(m_size.x * (m_boxes_size.x + m_boxes_offset),
                                                           m_size.y * (m_boxes_size.y + m_boxes_offset)));
        
        m_boxes.resize(m_size.x, nullptr);
        
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
}