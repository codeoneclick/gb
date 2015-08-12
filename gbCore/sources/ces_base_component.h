//
//  ces_base_component.h
//  gbCore
//
//  Created by sergey.sergeev on 8/11/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef ces_base_component_h
#define ces_base_component_h

#include "main_headers.h"
#include "ces_component_types.h"

namespace gb
{
    class ces_base_component;
    class ces_base_component_interface
    {
    private:
        
    protected:
        
        virtual void on_component_added(const std::shared_ptr<ces_base_component>& component);
        virtual void on_component_removed(const std::shared_ptr<ces_base_component>& component);
        
    public:
        
        ces_base_component_interface(void) = default;
        virtual ~ces_base_component_interface(void) = default;
    };
    
    class ces_base_component : public std::enable_shared_from_this<ces_base_component>
    {
    private:
        
    protected:
        
        std::set<std::shared_ptr<ces_base_component_interface>> m_listeners;
        
    public:
        ces_base_component(void) = default;
        virtual ~ces_base_component(void) = default;
        
        virtual e_ces_component_type get_type(void) const;
        
        void add_listener(const std::shared_ptr<ces_base_component_interface>& listener);
        void remove_listener(const std::shared_ptr<ces_base_component_interface>& listener);
    };
};

#endif
