//
//  model3d_static.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/26/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "model3d_static.h"
#include "ces_geometry_component.h"
#include "ces_render_component.h"
#include "ces_touch_component.h"
#include "ces_debug_render_component.h"
#include "ces_box2d_component.h"
#include "mesh_constructor.h"
#include "mesh.h"
#include "scene_graph.h"
#include "input_context.h"

namespace gb
{
    model3d_static::model3d_static()
    {
        ces_geometry_component_shared_ptr geometry_component = std::make_shared<ces_geometry_component>();
        ces_entity::add_component(geometry_component);
        
        ces_render_component_shared_ptr render_component = std::make_shared<ces_render_component>();
        ces_entity::add_component(render_component);
        render_component->set_z_order(0);
    }
    
    model3d_static::~model3d_static()
    {

    }
    
    void model3d_static::add_material(const std::string& technique_name, const material_shared_ptr& material)
    {
        unsafe_get_render_component_from_this->add_material(technique_name, material);
    }
    
    void model3d_static::remove_material(const std::string& technique_name)
    {
        unsafe_get_render_component_from_this->remove_material(technique_name);
    }
    
    material_shared_ptr model3d_static::get_material(const std::string& technique_name) const
    {
        return unsafe_get_render_component_from_this->get_material(technique_name);
    }
    
    void model3d_static::set_mesh(const mesh_shared_ptr& mesh)
    {
        unsafe_get_geometry_component_from_this->set_mesh(mesh);
        
        if(ces_entity::is_component_exist(e_ces_component_type_debug_render))
        {
            mesh_shared_ptr mesh_bounding_box = mesh_constructor::create_wireframe_box(mesh->get_min_bound(),
                                                                                       mesh->get_max_bound());
            unsafe_get_debug_render_component_from_this->set_mesh(mesh_bounding_box);
        }
    }
    
    void model3d_static::set_touches_receives_enabled(bool value)
    {
        if(value)
        {
            ces_touch_component_shared_ptr touch_component = std::make_shared<ces_touch_component>();
            ces_entity::add_component(touch_component);
            touch_component->set_scene_graph(game_object::get_scene_graph());
            
            touch_component->enable(e_input_state_pressed, true);
            touch_component->enable(e_input_state_dragged, true);
            touch_component->enable(e_input_state_released, true);
        }
        else
        {
            ces_entity::remove_component(e_ces_component_type_touch);
        }
    }
    
    void model3d_static::set_debug_draw_enabled(bool value)
    {
        if(value)
        {
            ces_debug_render_component_shared_ptr debug_render_component = std::make_shared<ces_debug_render_component>();
            ces_entity::add_component(debug_render_component);
            debug_render_component->set_scene_graph(game_object::get_scene_graph());
        }
        else
        {
            ces_entity::remove_component(e_ces_component_type_debug_render);
        }
    }
    
    void model3d_static::set_enable_box2d_physics(bool value, bool is_static)
    {
        if(value)
        {
            if(unsafe_get_geometry_component_from_this->get_mesh() && unsafe_get_geometry_component_from_this->get_mesh()->is_loaded())
            {
                ces_box2d_component_shared_ptr box2d_component = std::make_shared<ces_box2d_component>();
                ces_entity::add_component(box2d_component);
                std::shared_ptr<b2BodyDef> body_definition = box2d_component->get_box2d_body_definition();
                
                body_definition->type = is_static ? b2_staticBody : b2_dynamicBody;
                body_definition->position.Set(game_object::get_position().x, game_object::get_position().z);
                body_definition->userData = this;
                
                b2PolygonShape box2d_shape;
                glm::vec2 min_bound = glm::vec2(unsafe_get_geometry_component_from_this->get_mesh()->get_min_bound().x,
                                                unsafe_get_geometry_component_from_this->get_mesh()->get_min_bound().z);
                glm::vec2 max_bound = glm::vec2(unsafe_get_geometry_component_from_this->get_mesh()->get_max_bound().x,
                                                unsafe_get_geometry_component_from_this->get_mesh()->get_max_bound().z);
                
                box2d_shape.SetAsBox((max_bound.x - min_bound.x) / 2.f,
                                     (max_bound.y - min_bound.y) / 2.f);
                
                b2Body* box2d_body = game_object::get_scene_graph()->add_box2d_body(body_definition);
                assert(box2d_body);
                box2d_body->CreateFixture(&box2d_shape, 1);
                box2d_component->set_box2d_body(box2d_body);
            }
            else if(unsafe_get_geometry_component_from_this->get_mesh())
            {
                assert(false);
            }
            else
            {
                assert(false);
            }
        }
        else
        {
            ces_entity::remove_component(e_ces_component_type_box2d);
        }
    }
}