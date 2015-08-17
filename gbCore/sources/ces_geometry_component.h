//
//  ces_geometry_component.h
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef ces_geometry_component_h
#define ces_geometry_component_h

#include "ces_base_component.h"

namespace gb
{
    class mesh;
    class ces_geometry_component : public ces_base_component
    {
    private:
        
    protected:
        
        std::shared_ptr<mesh> m_mesh;
        
    public:
        
        ces_geometry_component(void);
        ~ces_geometry_component(void);
        
        void set_mesh(const std::shared_ptr<mesh>& mesh);
        std::shared_ptr<mesh> get_mesh(void) const;
    };
};

#endif
