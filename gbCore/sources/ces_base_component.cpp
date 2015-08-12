//
//  ces_base_component.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/11/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "ces_base_component.h"

namespace gb
{
    void ces_base_component_interface::on_component_added(const std::shared_ptr<ces_base_component>& component)
    {
        
    }
    
    void ces_base_component_interface::on_component_removed(const std::shared_ptr<ces_base_component>& component)
    {
        
    }
    
    e_ces_component_type ces_base_component::get_type(void) const
    {
        return e_ces_component_type_undefined;
    }
    
    void ces_base_component::add_listener(const std::shared_ptr<ces_base_component_interface>& listener)
    {
        m_listeners.insert(listener);
    }
    
    void ces_base_component::remove_listener(const std::shared_ptr<ces_base_component_interface>& listener)
    {
        m_listeners.erase(listener);
    }
};