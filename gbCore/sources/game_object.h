//
//  game_object.h
//  gbCore
//
//  Created by sergey.sergeev on 8/26/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef game_object_h
#define game_object_h

#include "ces_entity.h"

namespace gb
{
    class game_object : public ces_entity
    {
    private:
        
    protected:
        
    public:
        
        game_object();
        virtual ~game_object();
        
        void set_position(const glm::vec3& position);
        glm::vec3 get_position() const;
        
        void set_rotation(const glm::vec3& rotation);
        glm::vec3 get_rotation() const;
        
        void set_scale(const glm::vec3& scale);
        glm::vec3 get_scale() const;
        
        glm::vec3 get_forward() const;
        glm::vec3 get_up() const;
        glm::vec3 get_right() const;
        
        virtual void on_added_to_scene(const scene_graph_shared_ptr& scene_graph);
        virtual void on_removed_from_scene();
    };
};

#endif
