//
//  ces_instanced_geometry_component.h
//  gbCore
//
//  Created by sergey.sergeev on 10/1/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef ces_instanced_geometry_component_h
#define ces_instanced_geometry_component_h

#include "ces_geometry_component.h"

namespace gb
{
    class ces_instanced_geometry_component : public ces_geometry_component
    {
    private:
        
        instanced_mesh_shared_ptr m_instanced_mesh;
        
    protected:
        
    public:
        
        ces_instanced_geometry_component();
        ~ces_instanced_geometry_component();
        
        void set_mesh(const mesh_shared_ptr& mesh);
        mesh_shared_ptr get_mesh() const;
    };
};

#endif
