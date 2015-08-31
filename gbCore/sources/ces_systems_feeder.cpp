//
//  ces_systems_feeder.cpp
//  gbCore
//
//  Created by Sergey Sergeev on 8/13/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "ces_systems_feeder.h"
#include "ces_system.h"

namespace gb
{
    ces_systems_feeder::ces_systems_feeder(void)
    {
        
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
        for(const auto& system : m_systems)
        {
            system.second->on_feed_start(deltatime);
        }
        
        for(const auto& entity : m_entities)
        {
            for(const auto& system : m_systems)
            {
                system.second->on_feed(entity, deltatime);
            }
        }
        
        for(const auto& system : m_systems)
        {
            system.second->on_feed_end(deltatime);
        }
    }
    
    void ces_systems_feeder::add_system(const std::shared_ptr<ces_system>& system, e_ces_system_type type)
    {
        m_systems[type] = system;
    }
    
    void ces_systems_feeder::remove_system(e_ces_system_type type)
    {
        const auto& iterator = m_systems.find(type);
        if(iterator != m_systems.end())
        {
            m_systems.erase(iterator);
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