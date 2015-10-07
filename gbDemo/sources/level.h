//
//  level.h
//  gbDemo
//
//  Created by sergey.sergeev on 10/6/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef level_h
#define level_h

#include "main_headers.h"
#include "declarations.h"

namespace koth
{
    class level
    {
    private:
        
    protected:
        
        glm::ivec2 m_size;
        
        glm::vec2 m_boxes_size;
        f32 m_boxes_offset;
        
        gb::scene_fabricator_shared_ptr m_fabricator;
        gb::scene_graph_shared_ptr m_graph;
        
        std::vector<gb::instanced_models3d_static_shared_ptr> m_boxes;
        
    public:
        level(const gb::scene_fabricator_shared_ptr& fabricator,
              const gb::scene_graph_shared_ptr& graph);
        ~level();
        
        void construct(const std::string& filename);
    };
};


#endif
