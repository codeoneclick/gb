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
    class ces_geometry_component : public ces_base_component
    {
    private:
        
    protected:

        std::string m_guid;
        mesh_shared_ptr m_mesh;
        
    public:
        
        ces_geometry_component();
        ~ces_geometry_component();
        
        virtual void set_mesh(const mesh_shared_ptr& mesh);
        virtual mesh_shared_ptr get_mesh() const;
        
        virtual const glm::vec3 get_min_bound() const;
        virtual const glm::vec3 get_max_bound() const;
        virtual const std::tuple<glm::vec3, glm::vec3> get_bounds() const;
        
        virtual const glm::vec3 get_min_bound(const glm::mat4& mat) const;
        virtual const glm::vec3 get_max_bound(const glm::mat4& mat) const;
        virtual const std::tuple<glm::vec3, glm::vec3> get_bounds(const glm::mat4& mat) const;
        
        std::string get_guid() const;
    };
};

#endif
