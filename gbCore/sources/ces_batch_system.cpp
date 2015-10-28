//
//  ces_batch_system.cpp
//  gbCore
//
//  Created by sergey.sergeev on 10/28/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#include "ces_batch_system.h"
#include "ces_batch_component.h"
#include "ces_transformation_component.h"
#include "ces_render_component.h"
#include "ces_geometry_component.h"
#include "ces_transformation_component.h"
#include "material.h"
#include "shader.h"
#include "batch.h"

namespace gb
{
    ces_batch_system::ces_batch_system()
    {
        m_type = e_ces_system_type_ocean;
    }
    
    ces_batch_system::~ces_batch_system()
    {
        
    }
    
    void ces_batch_system::on_feed_start(f32 deltatime)
    {
        batch_shared_ptr batch = nullptr;
        for(const auto& iterator : m_batches)
        {
            batch = iterator.second;
            batch->cleanup();
        }
    }
    
    void ces_batch_system::on_feed(const ces_entity_shared_ptr& entity, f32 deltatime)
    {
        ces_batch_component_shared_ptr batch_component =
        std::static_pointer_cast<ces_batch_component>(entity->get_component(e_ces_component_type_batch));
        
        ces_render_component_shared_ptr render_component =
        std::static_pointer_cast<ces_render_component>(entity->get_component(e_ces_component_type_render));
        
        ces_geometry_component_shared_ptr geometry_component =
        std::static_pointer_cast<ces_geometry_component>(entity->get_component(e_ces_component_type_geometry));
        
        ces_transformation_component_shared_ptr transformation_component =
        std::static_pointer_cast<ces_transformation_component>(entity->get_component(e_ces_component_type_transformation));
        
        if(batch_component && render_component && geometry_component && transformation_component)
        {
            batch_component->cleanup();
            
            const std::unordered_map<std::string, std::unordered_map<i32, material_shared_ptr>>& materials = render_component->get_materials();
            
            material_shared_ptr material = nullptr;
            for(const auto& techniques_iterator : materials)
            {
                const auto& passes_container = techniques_iterator.second;
                for(const auto& pass_iterator : passes_container)
                {
                    material = pass_iterator.second;
                    if(material->get_shader()->is_commited())
                    {
                        batch_shared_ptr batch = nullptr;
                        
                        std::stringstream guid_string_stream;
                        guid_string_stream<<techniques_iterator.first<<pass_iterator.first<<material->get_shader()->get_guid();
                        std::string guid = guid_string_stream.str();
                        
                        auto batch_iterator = m_batches.find(guid);
                        if(batch_iterator != m_batches.end())
                        {
                            batch = batch_iterator->second;
                        }
                        else
                        {
                            batch = std::make_shared<gb::batch>();
                            m_batches[guid] = batch;
                        }
                        assert(batch);
                        
                        batch->add_data(geometry_component->get_mesh(), transformation_component->get_matrix_m());
                        batch_component->add_batch(batch, guid);
                    }
                }
            }
        }
    }
    
    void ces_batch_system::on_feed_end(f32 deltatime)
    {
        batch_shared_ptr batch = nullptr;
        for(const auto& iterator : m_batches)
        {
            batch = iterator.second;
            batch->generate();
        }
    }
}