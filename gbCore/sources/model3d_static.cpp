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
#include "ces_touch_component.h"
#include "mesh_constructor.h"
#include "mesh.h"
#include "ces_debug_render_component.h"
#include "input_context.h"

namespace gb
{
    model3d_static::model3d_static()
    {
        m_camera_component = std::make_shared<ces_camera_component>();
        ces_entity::add_component(m_camera_component);
        
        m_frustum_culling_component = std::make_shared<ces_frustum_culling_component>();
        ces_entity::add_component(m_frustum_culling_component);
                                  
        m_global_light_component = std::make_shared<ces_global_light_component>();
        ces_entity::add_component(m_global_light_component);
        
        m_geometry_component = std::make_shared<ces_geometry_component>();
        ces_entity::add_component(m_geometry_component);
        
        m_render_component = std::make_shared<ces_render_component>();
        ces_entity::add_component(m_render_component);
    }
    
    model3d_static::~model3d_static()
    {

    }
    
    void model3d_static::add_material(const std::string& technique_name, const material_shared_ptr& material)
    {
        unsafe_get_render_component_from_this()->add_material(technique_name, material);
    }
    
    void model3d_static::remove_material(const std::string& technique_name)
    {
        unsafe_get_render_component_from_this()->remove_material(technique_name);
    }
    
    material_shared_ptr model3d_static::get_material(const std::string& technique_name) const
    {
        return unsafe_get_render_component_from_this()->get_material(technique_name);
    }
    
    void model3d_static::set_mesh(const mesh_shared_ptr& mesh)
    {
        m_geometry_component->set_mesh(mesh);
        
        if(ces_entity::is_component_exist(e_ces_component_type_debug_render))
        {
            mesh_shared_ptr mesh_bounding_box = mesh_constructor::create_wireframe_box(mesh->get_min_bound(),
                                                                                       mesh->get_max_bound());
            m_debug_render_component->set_mesh(mesh_bounding_box);
        }
    }
    
    void model3d_static::set_touches_receives_enabled(bool value)
    {
        if(value)
        {
            m_touch_component = std::make_shared<ces_touch_component>();
            ces_entity::add_component(m_touch_component);
            
            m_touch_component->enable(e_input_state_pressed, true);
            m_touch_component->enable(e_input_state_dragged, true);
            m_touch_component->enable(e_input_state_released, true);
        }
        else
        {
            ces_entity::remove_component(e_ces_component_type_touch);
            m_touch_component = nullptr;
        }
    }
    
    void model3d_static::set_debug_draw_enabled(bool value)
    {
        if(value)
        {
            m_debug_render_component = std::make_shared<ces_debug_render_component>();
            ces_entity::add_component(m_debug_render_component);
        }
        else
        {
            ces_entity::remove_component(e_ces_component_type_debug_render);
            m_debug_render_component = nullptr;
        }
    }
}