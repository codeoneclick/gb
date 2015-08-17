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
        /*assert(m_renderPipeline != nullptr);
        assert(m_resourceAccessor != nullptr);
        
        std::shared_ptr<CConfigurationTransition> configurationTransition = std::static_pointer_cast<CConfigurationTransition>(configuration);
        assert(configurationTransition != nullptr);
        
        for(const auto& iterator : configurationTransition->getConfigurationWSTechnique())
        {
            std::shared_ptr<CConfigurationWSTechnique> configurationWSTechnique = std::static_pointer_cast<CConfigurationWSTechnique>(iterator);
            
            ui32 screenWidth = MIN_VALUE(configurationWSTechnique->getScreenWidth(), m_graphicsContext->getWidth());
            ui32 screenHeight = MIN_VALUE(configurationWSTechnique->getScreenHeight(), m_graphicsContext->getHeight());
            
            CSharedRenderTechniqueWorldSpace renderWSTechnique =
            std::make_shared<CRenderTechniqueWorldSpace>(screenWidth,
                                                         screenHeight,
                                                         configurationWSTechnique->getGUID(),
                                                         configurationWSTechnique->getIndex());
            glm::vec4 color = glm::vec4(configurationWSTechnique->getClearColorR(),
                                        configurationWSTechnique->getClearColorG(),
                                        configurationWSTechnique->getClearColorB(),
                                        configurationWSTechnique->getClearColorA());
            renderWSTechnique->setClearColor(color);
            renderWSTechnique->setAreDrawBoundingBoxes(configurationWSTechnique->getDrawBoundingBoxes());
            m_renderPipeline->addWorldSpaceRenderTechnique(configurationWSTechnique->getGUID(), renderWSTechnique);
        }
        
        for(const auto& iterator : configurationTransition->getConfigurationSSTechnique())
        {
            std::shared_ptr<CConfigurationSSTechnique> configurationSSTechnique = std::static_pointer_cast<CConfigurationSSTechnique>(iterator);
            assert(configurationSSTechnique != nullptr);
            std::shared_ptr<CConfigurationMaterial> configurationMaterial = configurationSSTechnique->getConfigurationMaterial();
            assert(configurationMaterial != nullptr);
            
            assert(m_resourceAccessor != nullptr);
            CSharedMaterial material =  CMaterial::constructCustomMaterial(configurationMaterial,
                                                                           m_resourceAccessor,
                                                                           m_renderPipeline);
            
            ui32 screenWidth = MIN_VALUE(configurationSSTechnique->getScreenWidth(), m_graphicsContext->getWidth());
            ui32 screenHeight = MIN_VALUE(configurationSSTechnique->getScreenHeight(), m_graphicsContext->getHeight());
            
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
        }*/
        

    }
    
    void game_transition::on_update(f32 deltatime)
    {
        
    }
    
    void game_transition::create_scene(void)
    {
        
    }
}