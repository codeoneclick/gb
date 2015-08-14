//
//  ces_systems_feeder.cpp
//  gbCore
//
//  Created by Sergey Sergeev on 8/13/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "ces_systems_feeder.h"
#include "ces_system.h"
#include "ces_configurations_system.h"

namespace gb
{
    ces_systems_feeder::ces_systems_feeder(void)
    {
        m_systems.insert(std::make_pair(e_ces_system_type_configuration, std::make_shared<ces_configurations_system>()));
    }
    
    ces_systems_feeder::~ces_systems_feeder(void)
    {
        
    }
    
    std::shared_ptr<ces_system> ces_systems_feeder::get_system(e_ces_system_type type) const
    {
        auto system = m_systems.find(type);
        if(system != m_systems.end())
        {
            return system->second;
        }
        return nullptr;
    }
    
    void ces_systems_feeder::on_update(f32 deltatime)
    {
        for(const auto& entity : m_entities)
        {
            for(const auto& system : m_systems)
            {
                system.second->on_feed(entity);
            }
        }
    }
    
    void ces_systems_feeder::add_entity(const std::shared_ptr<ces_entity>& entity)
    {
        m_entities.insert(entity);
    }
    
    void ces_systems_feeder::remove_entity(const std::shared_ptr<ces_entity>& entity)
    {
        m_entities.erase(entity);
    }
}