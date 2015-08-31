//
//  game_transition.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/12/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "game_transition.h"
#include "ces_systems_feeder.h"
#include "game_loop.h"
#include "configuration_accessor.h"
#include "resource_accessor.h"
#include "ces_render_system.h"
#include "ces_animation_system.h"
#include "ces_input_system.h"
#include "transition_configuration.h"
#include "render_pipeline.h"
#include "graphics_context.h"
#include "render_technique_ws.h"
#include "render_technique_ss.h"
#include "render_technique_main.h"
#include "material.h"
#include "scene_graph.h"
#include "fabricator.h"
#include "game_scene.h"

namespace gb
{
    game_transition::game_transition(const std::string& guid, bool is_offscreen) :
    m_guid(guid),
    m_offscreen(is_offscreen),
    m_scene_graph(nullptr),
    m_fabricator(nullptr),
    m_scene(nullptr)
    {
        m_system_feeder = std::make_shared<ces_systems_feeder>();
    }
    
    game_transition::~game_transition(void)
    {
        
    }
    
    std::string game_transition::get_guid(void) const
    {
        return m_guid;
    }
    
    void game_transition::on_activated(const graphics_context_shared_ptr& graphics_context,
                                       const input_context_shared_ptr& input_context,
                                       const configuration_accessor_shared_ptr& configurations_accessor,
                                       const resource_accessor_shared_ptr& resource_accessor)
    {
        std::shared_ptr<ces_render_system> render_system = std::make_shared<ces_render_system>(graphics_context, m_offscreen);
        std::shared_ptr<render_pipeline> render_pipeline = render_system->get_render_pipeline();
        
        std::shared_ptr<transition_configuration> transition_configuration =
        std::static_pointer_cast<gb::transition_configuration>(configurations_accessor->get_transition_configuration(m_guid));
        assert(transition_configuration);
        
        for(const auto& iterator : transition_configuration->get_ws_technique_configuration())
        {
            std::shared_ptr<ws_technique_configuration> ws_technique_configuration = std::static_pointer_cast<gb::ws_technique_configuration>(iterator);
            
            ui32 screen_width = std::min(ws_technique_configuration->get_screen_width(), render_pipeline->get_graphics_context()->get_width());
            ui32 screen_height = std::min(ws_technique_configuration->get_screen_height(), render_pipeline->get_graphics_context()->get_height());
            
            std::shared_ptr<render_technique_ws> render_technique_ws =
            std::make_shared<gb::render_technique_ws>(screen_width,
                                                      screen_height,
                                                      ws_technique_configuration->get_guid(),
                                                      ws_technique_configuration->get_index());
            glm::vec4 color = glm::vec4(ws_technique_configuration->get_clear_color_r(),
                                        ws_technique_configuration->get_clear_color_g(),
                                        ws_technique_configuration->get_clear_color_b(),
                                        ws_technique_configuration->get_clear_color_a());
            render_technique_ws->set_clear_color(color);
            render_pipeline->add_ws_render_technique(ws_technique_configuration->get_guid(), render_technique_ws);
            
            resource_accessor->add_custom_resource(ws_technique_configuration->get_guid() + ".color",
                                                   render_technique_ws->get_color_attachment_texture());
            resource_accessor->add_custom_resource(ws_technique_configuration->get_guid() + ".depth",
                                                   render_technique_ws->get_depth_attachment_texture());
        }
        
        for(const auto& iterator : transition_configuration->get_ss_technique_configuration())
        {
            std::shared_ptr<ss_technique_configuration> ss_technique_configuration = std::static_pointer_cast<gb::ss_technique_configuration>(iterator);
            assert(ss_technique_configuration != nullptr);
            std::shared_ptr<material_configuration> material_configuration = ss_technique_configuration->get_ConfigurationMaterial();
            assert(material_configuration);
            
            std::shared_ptr<material> material = material::construct(material_configuration);
            gb::material::set_shader(material, material_configuration, resource_accessor);
            gb::material::set_textures(material, material_configuration, resource_accessor);
            
            ui32 screen_width = std::min(ss_technique_configuration->get_screen_width(), render_pipeline->get_graphics_context()->get_width());
            ui32 screen_height = std::min(ss_technique_configuration->get_screen_height(), render_pipeline->get_graphics_context()->get_height());
            
            std::shared_ptr<render_technique_ss> render_technique_ss =
            std::make_shared<gb::render_technique_ss>(screen_width,
                                                      screen_height,
                                                      ss_technique_configuration->get_guid(),
                                                      material);
            render_pipeline->add_ss_render_technique(ss_technique_configuration->get_guid(), render_technique_ss);
            
            resource_accessor->add_custom_resource(ss_technique_configuration->get_guid() + ".color",
                                                   render_technique_ss->get_color_attachment_texture());
        }
        
        if(!m_offscreen)
        {
            std::shared_ptr<output_technique_configuration> main_technique_configuration = transition_configuration->get_output_technique_configuration();
            assert(main_technique_configuration);
            std::shared_ptr<material_configuration> material_configuration = main_technique_configuration->get_material_configuration();
            assert(material_configuration);
            
            std::shared_ptr<material> material =  material::construct(material_configuration);
            gb::material::set_shader(material, material_configuration, resource_accessor);
            gb::material::set_textures(material, material_configuration, resource_accessor);
            
            render_pipeline->create_main_render_technique(material);
        }
        
        m_fabricator = std::make_shared<fabricator>(configurations_accessor, resource_accessor);
        m_scene_graph = std::make_shared<scene_graph>(m_system_feeder);
        
        m_system_feeder->add_system(render_system, e_ces_system_type_render);
        
        std::shared_ptr<ces_animation_system> animation_system = std::make_shared<ces_animation_system>();
        m_system_feeder->add_system(animation_system, e_ces_system_type_animation);
        
        std::shared_ptr<ces_input_system> input_system = std::make_shared<ces_input_system>();
        m_system_feeder->add_system(input_system, e_ces_system_type_input);
        input_context->add_listener(input_system);
        
        add_listener_to_game_loop(m_system_feeder);
        add_listener_to_game_loop(m_scene_graph);
        
        create_scene();
    }
    
    void game_transition::on_deactivated(void)
    {
        remove_listener_from_game_loop(m_system_feeder);
        remove_listener_from_game_loop(m_scene_graph);
        
        destroy_scene();
    }
    
    void game_transition::on_update(f32 deltatime)
    {
        if(m_scene)
        {
            m_scene->update(deltatime);
        }
    }
    
    fabricator_shared_ptr game_transition::get_fabricator() const
    {
        return m_fabricator;
    }
    
    scene_graph_shared_ptr game_transition::get_scene_graph() const
    {
        return m_scene_graph;
    }
    
    ces_system_shared_ptr game_transition::get_system(e_ces_system_type type)
    {
        return m_system_feeder->get_system(type);
    }
}