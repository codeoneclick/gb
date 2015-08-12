//
//  resource.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/12/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "resource.h"

namespace gb
{
    resource_interface::~resource_interface(void)
    {
        m_resources.clear();
        m_listeners.clear();
    }
    
    void resource_interface::on_resource_loaded(const std::shared_ptr<resource> &resource, bool success)
    {
        if(success)
        {
            m_resources.insert(resource);
        }
        std::for_each(m_listeners.begin(), m_listeners.end(), [resource, success](std::shared_ptr<gb::resource_interface> listener){
            listener->on_resource_loaded(resource, success);
        });
    }
    
    void resource_interface::add_listener(const std::shared_ptr<resource_interface> &listener)
    {
        m_listeners.insert(listener);
        std::for_each(m_resources.begin(), m_resources.end(), [listener](std::shared_ptr<resource> resource){
            listener->on_resource_loaded(resource, true);
        });
    }
    
    void IResourceLoadingHandler::removeResourceLoadingCommand(const RESOURCE_LOADING_COMMAND& command)
    {
    }
    
    IResourceData::IResourceData(E_RESOURCE_DATA_CLASS resourceDataClass) :
    m_resourceDataClass(resourceDataClass)
    {
        
    }
    
    IResourceData::~IResourceData(void)
    {
        
    }
    
    E_RESOURCE_DATA_CLASS IResourceData::getResourceDataClass(void) const
    {
        return m_resourceDataClass;
    }
    
    IResource::IResource(E_RESOURCE_CLASS resourceClass, const std::string& guid) :
    m_resourceClass(resourceClass),
    m_guid(guid),
    m_status(E_RESOURCE_STATUS_UNLOADED)
    {
        
    }
    
    IResource::~IResource(void)
    {
        m_handlers.clear();
    }
    
    const std::string& IResource::getGUID(void) const
    {
        return m_guid;
    }
    
    E_RESOURCE_CLASS IResource::getResourceClass(void) const
    {
        return m_resourceClass;
    }
    
    bool IResource::isLoaded(void) const
    {
        const bool value = 0 != (m_status & E_RESOURCE_STATUS_LOADED);
        return value;
    };
    
    bool IResource::isCommited(void) const
    {
        const bool value = 0 != (m_status & E_RESOURCE_STATUS_COMMITED);
        return value;
    };
    
    void IResource::addLoadingHandler(ISharedResourceLoadingHandlerRef handler)
    {
        assert(handler != nullptr);
        if(IResource::isLoaded() && IResource::isCommited())
        {
            handler->onResourceLoaded(shared_from_this(), true);
        }
        m_handlers.insert(handler);
    }
    
    void IResource::removeLoadingHandler(ISharedResourceLoadingHandlerRef handler)
    {
        assert(handler != nullptr);
        m_handlers.erase(handler);
    }
    
    void IResource::onResourceLoaded(void)
    {
        for(const auto& handler : m_handlers)
        {
            handler->onResourceLoaded(shared_from_this(), true);
        }
        
        m_handlers.clear();
        std::set<ISharedResourceLoadingHandler> deleter;
        m_handlers.swap(deleter);
    }
}