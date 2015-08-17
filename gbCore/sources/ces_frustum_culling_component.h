//
//  ces_frustum_culling_component.h
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef ces_frustum_culling_component_h
#define ces_frustum_culling_component_h

#include "ces_base_component.h"

namespace gb
{
    class frustum;
    class ces_frustum_culling_component : public ces_base_component
    {
    private:
        
    protected:
        
        std::shared_ptr<frustum> m_frustum;
        
    public:
        
        ces_frustum_culling_component(void);
        ~ces_frustum_culling_component(void);
        
        void set_frustum(const std::shared_ptr<frustum>& frustum);
        std::shared_ptr<frustum> get_frustum(void) const;
    };
};

#endif
