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
#include "declarations.h"
#include "ces_component_types.h"

namespace gb
{
    class ces_base_component : public std::enable_shared_from_this<ces_base_component>
    {
    private:
        
    protected:
        
        e_ces_component_type m_type;

        ces_base_component();
        
    public:
        
        virtual ~ces_base_component() = default;
        
        e_ces_component_type get_type() const;
    };
};

#endif
