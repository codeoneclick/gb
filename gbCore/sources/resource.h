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
    struct resource_transfering_data
    {
    protected:
        
        e_resource_transfering_data_type m_type;
        resource_transfering_data(e_resource_transfering_data_type type);
        
    public:
        
        virtual ~resource_transfering_data(void) = default;
        
        e_resource_transfering_data_type get_type(void) const;
    };
    
    class resource : public std::enable_shared_from_this<resource>
    {
    private:
        
        friend class resource_serializer;
        std::string m_guid;
        
    protected:
        
        ui8 m_status;
        e_resource_type m_type;
        
        resource(e_resource_type type,
                 const std::string& guid);
        
        virtual void on_transfering_data_serialized(const std::shared_ptr<resource_transfering_data>& data) = 0;
        virtual void on_transfering_data_commited(const std::shared_ptr<resource_transfering_data>& data) = 0;
        
    public:
        
        virtual ~resource(void) = default;
        
        const std::string& get_guid(void) const;
        e_resource_type get_type(void) const;
        
        virtual bool is_loaded(void) const;
        virtual bool is_commited(void) const;
    };
};

#endif
