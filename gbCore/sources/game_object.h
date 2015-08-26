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
        
        ces_transformation_component_shared_ptr m_transformation_component;
        
    public:
        
        game_object(void);
        ~game_object(void);
        
        void set_position(const glm::vec3& position);
        glm::vec3 get_position(void) const;
        
        void set_rotation(const glm::vec3& rotation);
        glm::vec3 get_rotation(void) const;
        
        void set_scale(const glm::vec3& scale);
        glm::vec3 get_scale(void) const;
        
        glm::vec3 get_forward(void) const;
        glm::vec3 get_up(void) const;
        glm::vec3 get_right(void) const;
        
        virtual void on_added_to_scene();
        virtual void on_removed_from_scene();
    };
};

#endif
