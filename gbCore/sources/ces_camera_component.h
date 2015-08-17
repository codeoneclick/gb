//
//  ces_camera_component.h
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef ces_camera_component_h
#define ces_camera_component_h

#include "ces_base_component.h"

namespace gb
{
    class camera;
    class ces_camera_component : public ces_base_component
    {
    private:
    
    protected:
        
        std::shared_ptr<camera> m_camera;
        
    public:
        
        ces_camera_component(void);
        ~ces_camera_component(void);
        
        void set_camera(const std::shared_ptr<camera>& camera);
        std::shared_ptr<camera> get_camera(void) const;
    };
};

#endif
