//
//  ces_configurations_system.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/14/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "ces_configurations_system.h"
#include "ces_system_types.h"
#include "configuration_accessor.h"
#include "ces_configuration_component.h"

namespace gb
{
    ces_configurations_system::ces_configurations_system(const std::shared_ptr<configuration_accessor>& configuration_accessor)
    {
        m_type = e_ces_system_type_configuration;
        m_configuration_accessor = configuration_accessor;
    }
    
    ces_configurations_system::~ces_configurations_system(void)
    {
        
    }
    
    void ces_configurations_system::on_feed(const std::shared_ptr<ces_entity> &entity)
    {
        if(entity->is_component_exist(e_ces_component_type_configuration) &&
           entity->get_component(e_ces_component_type_configuration)->is_dirty())
        {
            std::shared_ptr<ces_configuration_component> configuration_component =
            std::static_pointer_cast<ces_configuration_component>(entity->get_component(e_ces_component_type_configuration));
            switch (configuration_component->get_configuration_type())
            {
                case e_configuration_type_model:
                {
                    
                }
                    break;
                    
                case e_configuration_type_particle_emitter:
                {
                    
                }
                    break;
                    
                case e_configuration_type_transition:
                {
                    std::shared_ptr<configuration> configuration = m_configuration_accessor->get_transition_configuration(configuration_component->get_configuration_filename());
                    configuration_component->on_configuration_loaded(configuration, configuration != nullptr);
                    
                }
                    break;
                    
                case e_configuration_type_ws_technique:
                {
                    
                }
                    break;
                    
                case e_configuration_type_ss_technique:
                {
                    
                }
                    break;
                    
                default:
                {
                    assert(false);
                }
                    break;
            }
        }
    }
}