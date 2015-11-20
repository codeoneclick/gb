//
//  ces_base_component.h
//  gbCore
//
//  Created by sergey.sergeev on 8/11/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef ces_base_component_h
#define ces_base_component_h

#include "main_headers.h"
#include "declarations.h"
#include "ces_component_types.h"

namespace gb
{
    class ces_base_component : public std::enable_shared_from_this<ces_base_component>
    {
    private:
        
        std::weak_ptr<scene_graph> m_scene_graph;
        
    protected:
        
        e_ces_component_type m_type;
        ces_base_component();
        
    public:
        
        virtual ~ces_base_component() = default;
        e_ces_component_type get_type() const;
        
        void set_scene_graph(const scene_graph_shared_ptr& scene_graph);
        scene_graph_shared_ptr get_scene_graph() const;
    };

#define unsafe_get_render_component(entity) static_cast<ces_render_component*>(entity->get_component(e_ces_component_type_render).get())
#define unsafe_get_render_component_from_this static_cast<ces_render_component*>(ces_entity::get_component(e_ces_component_type_render).get())
    
#define unsafe_get_transformation_component(entity) static_cast<ces_transformation_component*>(entity->get_component(e_ces_component_type_transformation).get())
#define unsafe_get_transformation_component_from_this static_cast<ces_transformation_component*>(ces_entity::get_component(e_ces_component_type_transformation).get())
    
#define unsafe_get_geometry_component(entity) static_cast<ces_geometry_component*>(entity->get_component(e_ces_component_type_geometry).get())
#define unsafe_get_geometry_component_from_this static_cast<ces_geometry_component*>(ces_entity::get_component(e_ces_component_type_geometry).get())
    
#define unsafe_get_animation_component(entity) static_cast<ces_animation_component*>(entity->get_component(e_ces_component_type_animation).get())
#define unsafe_get_animation_component_from_this static_cast<ces_animation_component*>(ces_entity::get_component(e_ces_component_type_animation).get())
    
#define unsafe_get_debug_render_component(entity) static_cast<ces_debug_render_component*>(entity->get_component(e_ces_component_type_debug_render).get())
#define unsafe_get_debug_render_component_from_this static_cast<ces_debug_render_component*>(ces_entity::get_component(e_ces_component_type_debug_render).get())
    
#define unsafe_get_touch_component(entity) static_cast<ces_touch_component*>(entity->get_component(e_ces_component_type_touch).get())
#define unsafe_get_touch_component_from_this static_cast<ces_touch_component*>(ces_entity::get_component(e_ces_component_type_touch).get())
    
#define unsafe_get_particle_emitter_component(entity) static_cast<ces_particle_emitter_component*>(entity->get_component(e_ces_component_type_particle_emitter).get())
#define unsafe_get_particle_emitter_component_from_this static_cast<ces_particle_emitter_component*>(ces_entity::get_component(e_ces_component_type_particle_emitter).get())
    
#define unsafe_get_instanced_geometry_component(entity) static_cast<ces_instanced_geometry_component*>(entity->get_component(e_ces_component_type_geometry).get())
#define unsafe_get_instanced_geometry_component_from_this static_cast<ces_instanced_geometry_component*>(ces_entity::get_component(e_ces_component_type_geometry).get())
    
#define unsafe_get_ocean_component(entity) static_cast<ces_ocean_component*>(entity->get_component(e_ces_component_type_ocean).get())
#define unsafe_get_ocean_component_from_this static_cast<ces_ocean_component*>(ces_entity::get_component(e_ces_component_type_ocean).get())
    
#define unsafe_get_skybox_component(entity) static_cast<ces_skybox_component*>(entity->get_component(e_ces_component_type_skybox).get())
#define unsafe_get_skybox_component_from_this static_cast<ces_skybox_component*>(ces_entity::get_component(e_ces_component_type_skybox).get())
    
#define unsafe_get_batch_component(entity) static_cast<ces_batch_component*>(entity->get_component(e_ces_component_type_batch).get())
#define unsafe_get_batch_component_from_this static_cast<ces_batch_component*>(ces_entity::get_component(e_ces_component_type_batch).get())
    
#define unsafe_get_terrain_component(entity) static_cast<ces_terrain_component*>(entity->get_component(e_ces_component_type_terrain).get())
#define unsafe_get_terrain_component_from_this static_cast<ces_terrain_component*>(ces_entity::get_component(e_ces_component_type_terrain).get())
    
};

#endif
