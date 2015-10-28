//
//  ces_batch_component.cpp
//  gbCore
//
//  Created by sergey.sergeev on 10/28/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#include "ces_batch_component.h"

namespace gb
{
    ces_batch_component::ces_batch_component()
    {
        m_type = e_ces_component_type_batch;
    }
    
    ces_batch_component::~ces_batch_component()
    {
        
    }
    
    void ces_batch_component::add_batch(const batch_shared_ptr& batch, const std::string& guid)
    {
        m_batches[guid] = batch;
    }
    
    batch_shared_ptr ces_batch_component::get_batch(const std::string& technique_name, i32 technique_pass, const std::string& material_guid)
    {
        batch_shared_ptr batch = nullptr;
        
        std::stringstream guid_string_stream;
        guid_string_stream<<technique_name<<technique_pass<<material_guid;
        std::string guid = guid_string_stream.str();
        
        const auto& iterator = m_batches.find(guid);
        if(iterator != m_batches.end())
        {
            batch = iterator->second;
        }
        return batch;
    }
    
    void ces_batch_component::cleanup()
    {
        m_batches.clear();
    }
}