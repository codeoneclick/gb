//
//  game_fabricator_interface.h
//  gbCore
//
//  Created by sergey.sergeev on 10/5/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef game_fabricator_interface_h
#define game_fabricator_interface_h

#include "main_headers.h"
#include "declarations.h"

namespace gb
{
    class game_fabricator_interface
    {
    private:
        
    protected:
        
        configuration_accessor_shared_ptr m_configuration_accessor;
        resource_accessor_shared_ptr m_resource_accessor;
        
    public:
        
        game_fabricator_interface();
        ~game_fabricator_interface() = default;
        
        void set_configuration_accessor(const configuration_accessor_shared_ptr& configuration_accessor);
        void set_resource_accessor(const resource_accessor_shared_ptr& resource_accessor);
    };
};

#endif
