//
//  ces_base_component.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/11/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "ces_base_component.h"

namespace gb
{
    e_ces_component_type ces_base_component::get_type(void) const
    {
        return e_ces_component_type_undefined;
    }
};