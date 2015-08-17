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
#include "ces_configurations_system.h"
#include "ces_render_system.h"
#include "transition_configuration.h"
#include "render_pipeline.h"
#include "graphics_context.h"
#include "render_technique_ws.h"
#include "render_technique_ss.h"
#include "render_technique_main.h"
#include "material.h"

namespace gb
{
    game_transition::game_transition(const std::string& guid, bool is_offscreen) :
    m_guid(guid),
    m_offscreen(is_offscreen)
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
    
    void game_transition::on_activated(const std::shared_ptr<graphics_context>& graphics_context,
                                       const std::shared_ptr<input_context>& input_context,
                                       const std::shared_ptr<resources_accessor>& resource_accessor,
                                       const std::shared_ptr<configuration_accessor>& configurations_accessor)
    {
        std::shared_ptr<ces_configurations_system> configurations_system = std::make_shared<ces_configurations_system>(configurations_accessor);
        std::shared_ptr<ces_render_system> render_system = std::make_shared<ces_render_system>(graphics_context, m_offscreen);
        
        m_system_feeder->add_system(configurations_system, e_ces_system_type_configuration);
        m_system_feeder->add_system(render_system, e_ces_system_type_render);
        
        m_system_feeder->add_entity(shared_from_this());
        add_listener_to_game_loop(m_system_feeder);
    }
    
    void game_transition::on_deactivated(void)
    {
        
    }
    
    void game_transition::on_configuration_loaded(const std::shared_ptr<configuration>& configuration, bool success)
    {
        
        std::shared_ptr<ces_render_system> render_system =
        std::static_pointer_cast<ces_render_system>(m_system_feeder->get_system(e_ces_system_type_render));
        std::shared_ptr<render_pipeline> render_pipeline = render_system->get_render_pipeline();
        
        std::shared_ptr<transition_configuration> transition_configuration = std::static_pointer_cast<gb::transition_configuration>(configuration);
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
        }
        
        for(const auto& iterator : transition_configuration->get_ss_technique_configuration())
        {
            std::shared_ptr<ss_technique_configuration> ss_technique_configuration = std::static_pointer_cast<gb::ss_technique_configuration>(iterator);
            assert(ss_technique_configuration != nullptr);
            std::shared_ptr<material_configuration> material_configuration = ss_technique_configuration->get_ConfigurationMaterial();
            assert(material_configuration);
            
            std::shared_ptr<material> material =  material::construct(material_configuration);
            
            ui32 screen_width = std::min(ss_technique_configuration->get_screen_width(), render_pipeline->get_graphics_context()->get_width());
            ui32 screen_height = std::min(ss_technique_configuration->get_screen_height(), render_pipeline->get_graphics_context()->get_height());
            
            CSharedRenderTechniqueScreenSpace renderSSTechnique =
            std::make_shared<CRenderTechniqueScreenSpace>(screenWidth,
                                                          screenHeight,
                                                          configurationSSTechnique->getGUID(),
                                                          material);
            m_renderPipeline->addScreenSpaceRenderTechnique(configurationSSTechnique->getGUID(), renderSSTechnique);
        }
        
        if(!m_isOffscreen)
        {
            std::shared_ptr<CConfigurationOutputTechnique> configurationOutputTechnique = configurationTransition->getConfigurationOutputTechnique();
            assert(configurationOutputTechnique != nullptr);
            std::shared_ptr<CConfigurationMaterial> configurationMaterial = configurationOutputTechnique->getConfigurationMaterial();
            assert(configurationMaterial != nullptr);
            
            assert(m_resourceAccessor != nullptr);
            CSharedMaterial material =  CMaterial::constructCustomMaterial(configurationMaterial,
                                                                           m_resourceAccessor,
                                                                           m_renderPipeline);
            m_renderPipeline->setMainRenderTechnique(material);
        }
        

    }
    
    void game_transition::on_update(f32 deltatime)
    {
        
    }
    
    void game_transition::create_scene(void)
    {
        
    }
}