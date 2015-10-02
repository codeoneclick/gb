//
//  ces_geometry_component.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "ces_geometry_component.h"
#include "mesh.h"

namespace gb
{
    ces_geometry_component::ces_geometry_component() :
    m_mesh(nullptr)
    {
        m_type = e_ces_component_type_geometry;
    }
    
    ces_geometry_component::~ces_geometry_component()
    {
        
    }
    
    void ces_geometry_component::set_mesh(const std::shared_ptr<mesh>& mesh)
    {
        m_mesh = mesh;
    }
    
    mesh_shared_ptr ces_geometry_component::get_mesh() const
    {
        return m_mesh;
    }
    
    const glm::vec3 ces_geometry_component::get_min_bound() const
    {
        if(!m_mesh)
        {
            std::cout<<"warning! mesh doesn't exist - bounds is wrong."<<std::endl;
            return glm::vec3(0.f);
        }
        return m_mesh->get_min_bound();
    }
    
    const glm::vec3 ces_geometry_component::get_max_bound() const
    {
        if(!m_mesh)
        {
            std::cout<<"warning! mesh doesn't exist - bounds is wrong."<<std::endl;
            return glm::vec3(0.f);
        }
        return m_mesh->get_max_bound();
    }
    
    const std::tuple<glm::vec3, glm::vec3> ces_geometry_component::get_bounds() const
    {
        if(!m_mesh)
        {
            std::cout<<"warning! mesh doesn't exist - bounds is wrong."<<std::endl;
            return std::make_tuple(glm::vec3(0.f), glm::vec3(0.f));
        }
        return m_mesh->get_bounds();
    }
    
    const glm::vec3 ces_geometry_component::get_min_bound(const glm::mat4& mat) const
    {
        if(!m_mesh)
        {
            std::cout<<"warning! mesh doesn't exist - bounds is wrong."<<std::endl;
            return glm::vec3(0.f);
        }
        return m_mesh->get_min_bound(mat);
    }
    
    const glm::vec3 ces_geometry_component::get_max_bound(const glm::mat4& mat) const
    {
        if(!m_mesh)
        {
            std::cout<<"warning! mesh doesn't exist - bounds is wrong."<<std::endl;
            return glm::vec3(0.f);
        }
        return m_mesh->get_max_bound(mat);
    }
    
    const std::tuple<glm::vec3, glm::vec3> ces_geometry_component::get_bounds(const glm::mat4& mat) const
    {
        if(!m_mesh)
        {
            std::cout<<"warning! mesh doesn't exist - bounds is wrong."<<std::endl;
            return std::make_tuple(glm::vec3(0.f), glm::vec3(0.f));
        }
        return m_mesh->get_bounds(mat);
    }
}