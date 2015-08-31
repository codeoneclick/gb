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
    ces_entity::ces_entity()
    {
        ces_entity::remove_components();
    }
    
    ces_entity::~ces_entity()
    {
        ces_entity::remove_components();
    }
    
    void ces_entity::add_component(const std::shared_ptr<ces_base_component>& component)
    {
        assert(component != nullptr && component->get_type() != e_ces_component_type_undefined);
        m_components[component->get_type()] = component;
    }
    
    void ces_entity::remove_component(const std::shared_ptr<ces_base_component>& component)
    {
        assert(component != nullptr && component->get_type() != e_ces_component_type_undefined);
        m_components[component->get_type()] = nullptr;
    }
    
    void ces_entity::remove_components()
    {
        for(auto& component : m_components)
        {
            component = nullptr;
        }
    }
    
    bool ces_entity::is_component_exist(e_ces_component_type type) const
    {
        assert(type != e_ces_component_type_undefined);
        return m_components[type] != nullptr;
    }
    
    std::shared_ptr<ces_base_component> ces_entity::get_component(e_ces_component_type type) const
    {
        assert(type != e_ces_component_type_undefined);
        return m_components[type];
    }
};