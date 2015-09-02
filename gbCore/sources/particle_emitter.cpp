//
//  particle_emitter.cpp
//  gbCore
//
//  Created by sergey.sergeev on 9/2/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "particle_emitter.h"
#include "ces_particle_emitter_component.h"
#include "ces_render_component.h"

namespace gb
{
    particle_emitter::particle_emitter()
    {
        ces_particle_emitter_component_shared_ptr particle_emitter_component = std::make_shared<ces_particle_emitter_component>();
        ces_entity::add_component(particle_emitter_component);
        
        ces_render_component_shared_ptr render_component = std::make_shared<ces_render_component>();
        ces_entity::add_component(render_component);
        render_component->set_z_order(1);
    }
    
    particle_emitter::~particle_emitter()
    {
        
    }
    
    void particle_emitter::add_material(const std::string& technique_name, const material_shared_ptr& material)
    {
        unsafe_get_render_component_from_this->add_material(technique_name, material);
    }
    
    void particle_emitter::remove_material(const std::string& technique_name)
    {
        unsafe_get_render_component_from_this->remove_material(technique_name);
    }
    
    material_shared_ptr particle_emitter::get_material(const std::string& technique_name) const
    {
        return unsafe_get_render_component_from_this->get_material(technique_name);
    }
    
    void particle_emitter::set_settings(const std::shared_ptr<particle_emitter_configuration>& settings)
    {
        unsafe_get_particle_emitter_component_from_this->set_settings(settings);
    }
}