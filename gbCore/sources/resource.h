//
//  resource.h
//  gbCore
//
//  Created by sergey.sergeev on 8/12/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef resource_h
#define resource_h

#include "main_headers.h"
#include "resource_types.h"

namespace gb
{
    class resource;
    class resource_interface
    {
    private:
        
        std::set<std::shared_ptr<resource_interface>> m_listeners;
        std::set<std::shared_ptr<resource>> m_resources;
        
    protected:
        
        virtual void on_resource_loaded(const std::shared_ptr<resource>& resource, bool success);
        
    public:
        
        resource_interface(void) = default;
        virtual ~resource_interface(void);
        
        void add_listener(const std::shared_ptr<resource_interface>& listener);
        void remove_listener(const std::shared_ptr<resource_interface>& listener);
    };
    
    class resource : public std::enable_shared_from_this<resource>
    {
    private:
        
        std::string m_guid;
        
    protected:
        
        ui8 m_status;
        e_resource_type m_resource_type;
        std::set<std::shared_ptr<resource_interface>> m_listeners;
        
        resource(e_resource_type resource_type,
                 const std::string& guid);
        
    public:
        
        virtual ~resource(void);
        
        const std::string& get_guid(void) const;
        e_resource_type get_resource_type(void) const;
        
        virtual bool is_loaded(void) const;
        virtual bool is_commited(void) const;
        
        void add_listener(const std::shared_ptr<resource_interface>& listener);
        void remove_listener(const std::shared_ptr<resource_interface>& listener);
    };
};

#endif
