//
//  pathfinder.cpp
//  gbDemo
//
//  Created by sergey.sergeev on 10/15/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#include "pathfinder.h"

namespace koth
{
    static i32 g_dx[k_directions] = {1, 1, 0, -1, -1, -1, 0, 1};
    static i32 g_dy[k_directions] = {0, 1, 1, 1, 0, -1, -1, -1};
    
    pathfinder::node::node(const glm::ivec2& position, i32 distance, i32 priority) :
    m_position(position),
    m_distance(distance),
    m_priority(priority)
    {
        
    }
    
    pathfinder::node::~node()
    {
        
    }
    
    glm::ivec2 pathfinder::node::get_position() const
    {
        return m_position;
    }
    
    i32 pathfinder::node::get_distance() const
    {
        return m_distance;
    }
    
    i32 pathfinder::node::get_priority() const
    {
        return m_priority;
    }
    
    void pathfinder::node::update_priority(const glm::ivec2& destination_position)
    {
        m_priority = m_distance + pathfinder::node::estimate(destination_position) * 10;
    }
    
    void pathfinder::node::next_distance(i32 direction)
    {
        m_distance += (k_directions == 8 ? (direction % 2 == 0 ? 10 : 14) : 10);
    }
    
    const i32 pathfinder::node::estimate(const glm::ivec2& destination_position) const
    {
        static glm::ivec2 distance_vec;
        static i32 distance;
        
        distance_vec.x = destination_position.x - m_position.x;
        distance_vec.y = destination_position.y - m_position.y;
        
        
        distance = static_cast<i32>(sqrt(distance_vec.x * distance_vec.x +
                                         distance_vec.y * distance_vec.y));
        
        return distance;
    }
    
    pathfinder::pathfinder(const glm::ivec2& size) :
    m_size(size)
    {
        m_map = new i32[m_size.x * m_size.y];
        m_closed_nodes_map = new i32[m_size.x * m_size.y];
        m_open_nodes_map = new i32[m_size.x * m_size.y];
        m_directions_map = new i32[m_size.x * m_size.y];
        
        for(i32 x = 0; x < m_size.x; ++x)
        {
            for(i32 y = 0; y < m_size.y; ++y)
            {
                m_map[x + y * m_size.x] = 0;
            }
        }
    }
    
    pathfinder::~pathfinder()
    {
        
    }
    
    std::vector<glm::ivec2> pathfinder::get_path(const glm::ivec2& start_position,
                                                 const glm::ivec2& end_position)
    {
        static std::priority_queue<std::shared_ptr<node>> open_nodes_queue[2];
        static i32 open_nodes_queue_index;
        open_nodes_queue_index = 0;
        
        std::shared_ptr<node> parent_node = nullptr;
        std::shared_ptr<node> child_node = nullptr;
        
        for(i32 x = 0; x < m_size.x; ++x)
        {
            for(i32 y = 0; y < m_size.y; ++y)
            {
                m_closed_nodes_map[x + y * m_size.x] = 0;
                m_open_nodes_map[x + y * m_size.x] = 0;
            }
        }
        
        parent_node = std::make_shared<pathfinder::node>(start_position, 0, 0);
        parent_node->update_priority(end_position);
        
        open_nodes_queue[open_nodes_queue_index].push(parent_node);
        
        m_open_nodes_map[(m_size.x - 1) + (m_size.y - 1) * m_size.x] = parent_node->get_priority();
        
        i32 x, y = 0;
        i32 dx, dy = 0;
        
        while(!open_nodes_queue[open_nodes_queue_index].empty())
        {
            parent_node = std::make_shared<pathfinder::node>(open_nodes_queue[open_nodes_queue_index].top()->get_position(),
                                                             open_nodes_queue[open_nodes_queue_index].top()->get_distance(),
                                                             open_nodes_queue[open_nodes_queue_index].top()->get_priority());
            
            x = parent_node->get_position().x;
            y = parent_node->get_position().y;
            
            open_nodes_queue[open_nodes_queue_index].pop();
            m_open_nodes_map[x + y * m_size.x] = 0;
            m_closed_nodes_map[x + y * m_size.x] = 1;
            
            if(x == end_position.x && y == end_position.y)
            {
                std::vector<glm::ivec2> path;

                while(!(x == start_position.x && y == start_position.y))
                {
                    i32 direction = m_directions_map[x + y * m_size.x];
                    x += g_dx[direction];
                    y += g_dy[direction];
                    path.push_back(glm::ivec2(x, y));
                    
                    if(path.size() >= m_size.x + m_size.y)
                    {
                        return std::vector<glm::ivec2>();
                    }
                }
                return path;
            }
            
            for(i32 i = 0; i < k_directions; i++)
            {
                dx = x + g_dx[i];
                dy = y + g_dy[i];
                
                if(!(dx < 0 || dx > m_size.x - 1 || dy < 0 || dy > m_size.y - 1 || m_map[dx + dy * m_size.x] == 1
                     || m_closed_nodes_map[dx + dy * m_size.x] == 1))
                {
                    
                    child_node = std::make_shared<pathfinder::node>(glm::ivec2(dx, dy),
                                                                    parent_node->get_distance(),
                                                                    parent_node->get_priority());
                    child_node->next_distance(i);
                    child_node->update_priority(end_position);
                    
                    if(m_open_nodes_map[dx + dy * m_size.x] == 0)
                    {
                        m_open_nodes_map[dx + dy * m_size.x] = child_node->get_priority();
                        open_nodes_queue[open_nodes_queue_index].push(child_node);
                        m_directions_map[dx + dy * m_size.x] = (i + k_directions / 2) % k_directions;
                    }
                    else if(m_open_nodes_map[dx + dy * m_size.x] > child_node->get_priority())
                    {
                        m_open_nodes_map[dx + dy * m_size.x] = child_node->get_priority();
                        m_directions_map[dx + dy * m_size.x] = (i + k_directions / 2) % k_directions;
                        while(!(open_nodes_queue[open_nodes_queue_index].top()->get_position().x == dx &&
                                open_nodes_queue[open_nodes_queue_index].top()->get_position().y == dy))
                        {
                            open_nodes_queue[1 - open_nodes_queue_index].push(open_nodes_queue[open_nodes_queue_index].top());
                            open_nodes_queue[open_nodes_queue_index].pop();
                        }
                        open_nodes_queue[open_nodes_queue_index].pop();
                        
                        if(open_nodes_queue[open_nodes_queue_index].size() > open_nodes_queue[1 - open_nodes_queue_index].size())
                        {
                            open_nodes_queue_index = 1 - open_nodes_queue_index;
                        }
                        while(!open_nodes_queue[open_nodes_queue_index].empty())
                        {                
                            open_nodes_queue[1 - open_nodes_queue_index].push(open_nodes_queue[open_nodes_queue_index].top());
                            open_nodes_queue[open_nodes_queue_index].pop();
                        }
                        open_nodes_queue_index = 1 - open_nodes_queue_index;
                        open_nodes_queue[open_nodes_queue_index].push(child_node);
                    }
                }
            }

        }
        return std::vector<glm::ivec2>();
    }
}