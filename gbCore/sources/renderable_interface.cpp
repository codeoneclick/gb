//
//  renderable_interface.cpp
//  gbCore
//
//  Created by sergey.sergeev on 10/12/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#include "renderable_interface.h"
#include "ces_render_component.h"

namespace gb
{
    renderable_interface::renderable_interface()
    {
        ces_render_component_shared_ptr render_component = std::make_shared<ces_render_component>();
        ces_entity::add_component(render_component);
    }
    
    renderable_interface::~renderable_interface()
    {
        
    }
    
    void renderable_interface::set_scene_graph(const scene_graph_shared_ptr& scene_graph)
    {
        m_scene_graph = scene_graph;
    }
    
    scene_graph_shared_ptr renderable_interface::get_scene_graph() const
    {
        return m_scene_graph.lock();
    }
    
    void renderable_interface::on_added_to_scene(const scene_graph_shared_ptr& scene_graph)
    {
        renderable_interface::set_scene_graph(scene_graph);
        for(const auto& component : ces_entity::get_components())
        {
            if(component != nullptr)
            {
                component->set_scene_graph(scene_graph);
            }
        }
    }
    
    void renderable_interface::on_removed_from_scene()
    {
        renderable_interface::set_scene_graph(nullptr);
        for(const auto& component : ces_entity::get_components())
        {
            component->set_scene_graph(nullptr);
        }
    }
    
    void renderable_interface::add_material(const std::string& technique_name, i32 technique_pass, const material_shared_ptr& material)
    {
        unsafe_get_render_component_from_this->add_material(technique_name, technique_pass, material);
    }
    
    void renderable_interface::remove_material(const std::string& technique_name, i32 technique_pass)
    {
        unsafe_get_render_component_from_this->remove_material(technique_name, technique_pass);
    }
    
    material_shared_ptr renderable_interface::get_material(const std::string& technique_name, i32 technique_pass) const
    {
        return unsafe_get_render_component_from_this->get_material(technique_name, technique_pass);
    }
}