//
//  ces_global_light_component.h
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef ces_global_light_component_h
#define ces_global_light_component_h

#include "ces_base_component.h"

namespace gb
{
    class global_light;
    class ces_global_light_component : public ces_base_component
    {
    private:
        
    protected:
        
        std::shared_ptr<global_light> m_global_light;
        
    public:
        
        ces_global_light_component(void);
        ~ces_global_light_component(void);
        
        void set_global_light(const std::shared_ptr<global_light>& light);
        std::shared_ptr<global_light> get_global_light(void) const;
    };
};

#endif
