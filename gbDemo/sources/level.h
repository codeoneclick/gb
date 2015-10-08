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
    enum e_level_box_state
    {
        e_level_box_state_none = 0,
        e_level_box_state_fall_down,
        e_level_box_state_drop_down
    };
    
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
        std::vector<i32> m_boxes_states;
        
    public:
        level(const gb::scene_fabricator_shared_ptr& fabricator,
              const gb::scene_graph_shared_ptr& graph);
        ~level();
        
        void construct(const std::string& filename);
        
        void update(f32 deltatime);
        
        void set_box_state(i32 x, i32 z);
    };
};


#endif
