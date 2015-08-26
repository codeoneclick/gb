//
//  model3d_static.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/26/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "model3d_static.h"
#include "ces_camera_component.h"
#include "ces_frustum_culling_component.h"
#include "ces_global_light_component.h"
#include "ces_geometry_component.h"
#include "ces_render_component.h"

namespace gb
{
    model3d_static::model3d_static()
    {
        m_camera_component = std::make_shared<ces_camera_component>();
        m_frustum_culling_component = std::make_shared<ces_frustum_culling_component>();
        m_global_light_component = std::make_shared<ces_global_light_component>();
        m_geometry_component = std::make_shared<ces_geometry_component>();
        m_render_component = std::make_shared<ces_render_component>();
    }
    
    model3d_static::~model3d_static()
    {
        ces_entity::remove_component(m_camera_component);
        ces_entity::remove_component(m_frustum_culling_component);
        ces_entity::remove_component(m_global_light_component);
        ces_entity::remove_component(m_geometry_component);
        ces_entity::remove_component(m_render_component);
    }
    
    void model3d_static::add_material(const std::string& technique_name, const material_shared_ptr& material)
    {
        m_render_component->add_material(technique_name, material);
    }
    
    void model3d_static::remove_material(const std::string& technique_name)
    {
        m_render_component->remove_material(technique_name);
    }
    
    material_shared_ptr model3d_static::get_material(const std::string& technique_name) const
    {
        return m_render_component->get_material(technique_name);
    }
    
    void model3d_static::set_mesh(const mesh_shared_ptr& mesh)
    {
        m_geometry_component->set_mesh(mesh);
    }
}