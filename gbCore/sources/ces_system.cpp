//
//  ces_system.cpp
//  gbCore
//
//  Created by Sergey Sergeev on 8/13/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "ces_system.h"

namespace gb
{
    ces_system::ces_system(void) :
    m_type(e_ces_system_type_undefined)
    {
        
    }
    
    ces_system::~ces_system(void)
    {
        
    }
    
    e_ces_system_type ces_system::get_type(void) const
    {
        return m_type;
    }
}