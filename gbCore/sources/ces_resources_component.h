//
//  ces_resources_component.h
//  gbCore
//
//  Created by Sergey Sergeev on 8/12/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef ces_resources_component_h
#define ces_resources_component_h

#include "ces_base_component.h"

namespace gb
{
    class resource;
    class ces_resources_component_interface : public ces_base_component_interface
    {
    private:
        
    protected:
        
    public:
        
        ces_resources_component_interface(void) = default;
        ~ces_resources_component_interface(void) = default;
        
        virtual void on_resource_loaded(const std::shared_ptr<resource>& resource, bool success);
    };
    
    class ces_resources_component : public ces_base_component
    {
    private:
        
        std::set<std::shared_ptr<resource>> m_resources;
        
    protected:
        
        void on_resource_loaded(const std::shared_ptr<resource>& resource, bool success);
        
    public:
        
        ces_resources_component(void);
        ~ces_resources_component(void);
    };
};

#endif
