//
//  resource_loader.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/24/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "resource_loader.h"
#include "resource_loading_operation.h"
#include "shader_loading_operation.h"
#include "texture_loading_operation.h"
#include "resource.h"
#include "shader.h"
#include "texture.h"

namespace gb
{
    resource_loader::resource_loader()
    {
        m_thread_executed = true;
        m_thread = std::thread(&resource_loader::on_thread_update, this);
    }
    
    resource_loader::~resource_loader()
    {
        m_thread_executed = false;
        m_thread.join();
        m_operationsQueue.clear();
        m_resources.clear();
    }
    
    void resource_loader::on_thread_update()
    {
        while (m_thread_executed)
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            for(const auto& iterator : m_operationsQueue)
            {
                resource_loading_operation_shared_ptr operation = iterator.second;
                if(operation->get_status() == e_resource_loading_operation_status_undefined)
                {
                    operation->serialize();
                }
            }
            if(m_operationsQueue.empty())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
    }
    
    void resource_loader::on_update(f32 deltatime)
    {
        auto iterator = m_operationsQueue.begin();
        while(iterator != m_operationsQueue.end())
        {
            resource_loading_operation_shared_ptr operation = iterator->second;
            if(operation->get_status() == e_resource_loading_operation_status_waiting)
            {
                operation->commit();
            }
            else if(operation->get_status() == e_resource_loading_operation_status_failure ||
                    operation->get_status() == e_resource_loading_operation_status_success)
            {
                std::lock_guard<std::mutex> guard(m_mutex);
                m_operationsQueue.erase(iterator++);
                
                const auto& resource = m_resources.find(operation->get_guid());
                assert(resource != m_resources.end());
                if(resource != m_resources.end())
                {
                    resource->second->on_resource_loaded(operation->get_status() == e_resource_loading_operation_status_success);
                }
            }
            else
            {
                ++iterator;
            }
        }
    }
    
    shader_shared_ptr resource_loader::get_shader(const std::string &vs_filename, const std::string &fs_filename, bool sync)
    {
        std::string guid = std::string().append(vs_filename).append(fs_filename);
        shader_shared_ptr resource = nullptr;
        if(m_resources.find(guid) != m_resources.end())
        {
            resource = std::static_pointer_cast<shader>(m_resources.find(guid)->second);
        }
        else
        {
            resource = std::make_shared<shader>(guid);
            shader_loading_operation_shared_ptr operation = std::make_shared<shader_loading_operation>(vs_filename,
                                                                                                       fs_filename,
                                                                                                       resource);
            m_resources.insert(std::make_pair(guid, resource));
            if(!sync)
            {
                m_operationsQueue.insert(std::make_pair(guid, operation));
            }
            else
            {
                operation->serialize();
                operation->commit();
                resource->on_resource_loaded(operation->get_status() == e_resource_loading_operation_status_success);
            }
        }
        return resource;
    }
    
    
    texture_shared_ptr resource_loader::get_texture(const std::string &filename, bool sync)
    {
        std::string guid = filename;
        texture_shared_ptr resource = nullptr;
        if(m_resources.find(guid) != m_resources.end())
        {
            resource = std::static_pointer_cast<texture>(m_resources.find(guid)->second);
        }
        else
        {
            resource = std::make_shared<texture>(guid);
            texture_loading_operation_shared_ptr operation = std::make_shared<texture_loading_operation>(filename,
                                                                                                         resource);
            m_resources.insert(std::make_pair(guid, resource));
            if(!sync)
            {
                m_operationsQueue.insert(std::make_pair(guid, operation));
            }
            else
            {
                operation->serialize();
                operation->commit();
                resource->on_resource_loaded(operation->get_status() == e_resource_loading_operation_status_success);
            }
        }
        return resource;
    }
    
    
}