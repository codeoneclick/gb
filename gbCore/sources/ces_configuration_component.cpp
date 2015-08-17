//
//  ces_configuration_component.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/14/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "ces_configuration_component.h"

namespace gb
{
    ces_configuration_component_interface::ces_configuration_component_interface(void) :
    gb::ces_base_component_interface()
    {
        m_type = e_ces_component_type_configuration;
    }
    
    void ces_configuration_component_interface::on_configuration_loaded(const std::shared_ptr<configuration>& configuration, bool success)
    {
        
    }
    
    ces_configuration_component::ces_configuration_component(const std::string& configuration_filename, e_configuration_type configuration_type) :
    gb::ces_base_component(),
    m_configuration_filename(configuration_filename),
    m_configuration_type(configuration_type),
    m_configuration(nullptr)
    {
        m_type = e_ces_component_type_configuration;
    }
    
    ces_configuration_component::~ces_configuration_component(void)
    {
        
    }
    
    std::string ces_configuration_component::get_configuration_filename(void) const
    {
        return m_configuration_filename;
    }
    
    e_configuration_type ces_configuration_component::get_configuration_type() const
    {
        return m_configuration_type;
    }
    
    std::shared_ptr<configuration> ces_configuration_component::get_configuration(void) const
    {
        return m_configuration;
    }
    
    void ces_configuration_component::on_configuration_loaded(const std::shared_ptr<configuration>& configuration, bool success)
    {
        if(success)
        {
            m_configuration = configuration;
            for(const auto& listener : m_listeners)
            {
                if(listener->get_type() == e_ces_component_type_configuration)
                {
                    std::static_pointer_cast<ces_configuration_component_interface>(listener)->on_configuration_loaded(m_configuration,
                                                                                                                       success);
                }
            }
        }
        else
        {
            assert(false);
        }
    }
}
