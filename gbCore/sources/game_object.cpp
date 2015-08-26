//
//  game_object.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/26/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "game_object.h"
#include "ces_transformation_component.h"

namespace gb
{
    game_object::game_object(void)
    {
        m_transformation_component = std::make_shared<ces_transformation_component>();
        ces_entity::add_component(m_transformation_component);
    }
    
    game_object::~game_object(void)
    {
        ces_entity::remove_component(m_transformation_component);
    }
    
    void game_object::set_position(const glm::vec3& position)
    {
        m_transformation_component->set_position(position);
    }
    
    glm::vec3 game_object::get_position(void) const
    {
        return m_transformation_component->get_position();
    }
    
    void game_object::set_rotation(const glm::vec3& rotation)
    {
        m_transformation_component->set_rotation(rotation);
    }
    
    glm::vec3 game_object::get_rotation(void) const
    {
        return m_transformation_component->get_rotation();
    }
    
    void game_object::set_scale(const glm::vec3& scale)
    {
        m_transformation_component->set_scale(scale);
    }
    
    glm::vec3 game_object::get_scale(void) const
    {
        return m_transformation_component->get_scale();
    }
    
    glm::vec3 game_object::get_forward(void) const
    {
        return m_transformation_component->get_forward();
    }
    
    glm::vec3 game_object::get_up(void) const
    {
        return m_transformation_component->get_up();
    }
    
    glm::vec3 game_object::get_right(void) const
    {
        return m_transformation_component->get_right();
    }
    
    void game_object::on_added_to_scene()
    {
        
    }
    
    void game_object::on_removed_from_scene()
    {
        
    }
}