//
//  pathfinder.h
//  gbDemo
//
//  Created by sergey.sergeev on 10/15/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#ifndef pathfinder_h
#define pathfinder_h

#include "main_headers.h"
#include "declarations.h"

#define k_directions 8

namespace koth
{
    class pathfinder
    {
    public:
        
        class node
        {
        private:
            
            glm::ivec2 m_position;
            i32 m_distance;
            i32 m_priority;
            
        public:
            
            node(const glm::ivec2& position, i32 distance, i32 priority);
            ~node();
            
            glm::ivec2 get_position() const;
            i32 get_distance() const;
            i32 get_priority() const;
            
            void update_priority(const glm::ivec2& destination_position);
            void next_distance(i32 direction);
            const i32 estimate(const glm::ivec2& destination_position) const;
        };
        
    private:
        
    protected:
        
        glm::ivec2 m_size;
        
        i32* m_map;
        i32* m_closed_nodes_map;
        i32* m_open_nodes_map;
        i32* m_directions_map;
        
    public:
        
        pathfinder(const glm::ivec2& size);
        ~pathfinder();
        
        std::vector<glm::ivec2> get_path(const glm::ivec2& start_position,
                                         const glm::ivec2& end_position);
    };
};

#endif
