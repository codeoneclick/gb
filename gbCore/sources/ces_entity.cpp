//
//  ces_entity.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/11/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "ces_entity.h"

namespace gb
{
    ces_entity::ces_entity(void)
    {
        
    }
    
    ces_entity::~ces_entity(void)
    {
        ces_entity::remove_components();
    }
    
    void ces_entity::add_component(const std::shared_ptr<ces_base_component>& component)
    {
        assert(component != nullptr && component->get_type() != e_ces_component_type_undefined);
        m_components.insert(std::make_pair(component->get_type(), component));
    }
    
    void ces_entity::remove_component(const std::shared_ptr<ces_base_component>& component)
    {
        assert(component != nullptr && component->get_type() != e_ces_component_type_undefined);
        auto iterator = m_components.find(component->get_type());
        assert(iterator != m_components.end());
        m_components.erase(iterator);
    }
    
    void ces_entity::remove_components(void)
    {
        m_components.clear();
    }
    
    bool ces_entity::is_component_exist(e_ces_component_type type) const
    {
        assert(type != e_ces_component_type_undefined);
        auto iterator = m_components.find(type);
        return iterator != m_components.end();
    }
    
    std::shared_ptr<ces_base_component> ces_entity::get_component(e_ces_component_type type) const
    {
        assert(type != e_ces_component_type_undefined);
        auto iterator = m_components.find(type);
        return iterator != m_components.end() ? iterator->second : nullptr;
    }
};