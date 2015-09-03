//
//  fabricator.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/26/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "fabricator.h"
#include "resource_accessor.h"
#include "mesh.h"
#include "material.h"
#include "sequence.h"
#include "animation_mixer.h"
#include "camera.h"
#include "global_light.h"
#include "model3d_static.h"
#include "model3d_animated.h"
#include "particle_emitter.h"
#include "mesh_constructor.h"

namespace gb
{
    fabricator::fabricator(const configuration_accessor_shared_ptr& configuration_accessor,
                           const resource_accessor_shared_ptr& resource_accessor) :
    m_configuration_accessor(configuration_accessor),
    m_resource_accessor(resource_accessor)
    {
        
    }
    
    fabricator::~fabricator()
    {
        
    }
    
    camera_shared_ptr fabricator::create_camera(f32 fov, f32 near, f32 far,const glm::ivec4& viewport)
    {
        camera_shared_ptr camera = std::make_shared<gb::camera>(fov, near, far, viewport);
        m_cameras_container.insert(camera);
        return camera;
    }
    
    void fabricator::destroy_camera(const camera_shared_ptr& camera)
    {
        m_cameras_container.erase(camera);
    }
    
    global_light_shared_ptr fabricator::create_global_light(f32 fov, f32 near, f32 far)
    {
        global_light_shared_ptr global_light = std::make_shared<gb::global_light>(fov, near, far);
        m_global_lights_container.insert(global_light);
        return global_light;
    }
    
    void fabricator::destroy_global_light(const global_light_shared_ptr& global_light)
    {
        m_global_lights_container.erase(global_light);
    }
    
    model3d_static_shared_ptr fabricator::create_model3d_static(const std::string& filename)
    {
        std::shared_ptr<model_configuration> model_configuration =
        std::static_pointer_cast<gb::model_configuration>(m_configuration_accessor->get_model_configuration(filename));
        assert(model_configuration);
        model3d_static_shared_ptr model3d_static = nullptr;
        if(model_configuration)
        {
            model3d_static = std::make_shared<gb::model3d_static>();
            
            mesh_shared_ptr mesh = nullptr;
            if(model_configuration->get_mesh_filename().length() != 0)
            {
                mesh = m_resource_accessor->get_mesh(model_configuration->get_mesh_filename());
            }
            else if(model_configuration->get_mesh_base_class().length() != 0)
            {
                if(model_configuration->get_mesh_base_class() == "plane")
                {
                    mesh = mesh_constructor::create_plane(glm::vec2(1.f));
                }
                else
                {
                    assert(false);
                }
            }
            else
            {
                assert(false);
            }
        
            assert(mesh);
            model3d_static->set_mesh(mesh);
            
            for(const auto& iterator : model_configuration->get_materials_configurations())
            {
                std::shared_ptr<material_configuration> material_configuration =
                std::static_pointer_cast<gb::material_configuration>(iterator);
                
                std::shared_ptr<material> material = material::construct(material_configuration);
                gb::material::set_shader(material, material_configuration, m_resource_accessor);
                gb::material::set_textures(material, material_configuration, m_resource_accessor);
                model3d_static->add_material(material_configuration->get_render_technique_name(), material);
            }
            m_game_objects_container.insert(model3d_static);
        }
        return model3d_static;
    }
    
    model3d_animated_shared_ptr fabricator::create_model3d_animated(const std::string &filename)
    {
        std::shared_ptr<model_configuration> model_configuration =
        std::static_pointer_cast<gb::model_configuration>(m_configuration_accessor->get_model_configuration(filename));
        assert(model_configuration);
        model3d_animated_shared_ptr model3d_animated = nullptr;
        if(model_configuration)
        {
            model3d_animated = std::make_shared<gb::model3d_animated>();
            
            mesh_shared_ptr mesh = m_resource_accessor->get_mesh(model_configuration->get_mesh_filename());
            assert(mesh);

            model3d_animated->set_mesh(mesh);
            mesh->add_resource_loading_callback(std::make_shared<resource::f_resource_loading_callback>([model_configuration, model3d_animated, this](const resource_shared_ptr& resource, bool success) {
                
                mesh_shared_ptr mesh = std::static_pointer_cast<gb::mesh>(resource);
                assert(mesh->get_skeleton_data());
                assert(mesh->get_bindpose_data());

                model3d_animated->set_mesh(mesh);
                animation_mixer_shared_ptr animation_mixer = std::make_shared<gb::animation_mixer>(mesh->get_skeleton_data(),
                                                                                                   mesh->get_bindpose_data());
                
                for(const auto& iterator : model_configuration->get_animations_configurations())
                {
                    std::shared_ptr<animation_configuration> animation_configuration = std::static_pointer_cast<gb::animation_configuration>(iterator);
                    sequence_shared_ptr sequence = m_resource_accessor->get_animation(animation_configuration->get_animation_filename());
                    sequence->add_resource_loading_callback(std::make_shared<resource::f_resource_loading_callback>([animation_mixer](const resource_shared_ptr& resource, bool success) {
                        
                        sequence_shared_ptr sequence = std::static_pointer_cast<gb::sequence>(resource);
                        animation_mixer->add_sequence(sequence);
                    }));
                }
                model3d_animated->set_animation_mixer(animation_mixer);
                model3d_animated->create_animation_linkage(model_configuration);
            }));
            
            for(const auto& iterator : model_configuration->get_materials_configurations())
            {
                std::shared_ptr<material_configuration> material_configuration =
                std::static_pointer_cast<gb::material_configuration>(iterator);
                
                material_shared_ptr material = material::construct(material_configuration);
                gb::material::set_shader(material, material_configuration, m_resource_accessor);
                gb::material::set_textures(material, material_configuration, m_resource_accessor);
                model3d_animated->add_material(material_configuration->get_render_technique_name(), material);
            }
            m_game_objects_container.insert(model3d_animated);
        }
        return model3d_animated;
    }
    
    particle_emitter_shared_ptr fabricator::create_particle_emitter(const std::string& filename)
    {
        std::shared_ptr<particle_emitter_configuration> particle_emitter_configuration =
        std::static_pointer_cast<gb::particle_emitter_configuration>(m_configuration_accessor->get_particle_emitter_configuration(filename));
        assert(particle_emitter_configuration);
        particle_emitter_shared_ptr particle_emitter = nullptr;
        if(particle_emitter_configuration)
        {
            particle_emitter = std::make_shared<gb::particle_emitter>();
            particle_emitter->set_settings(particle_emitter_configuration);
            
            for(const auto& iterator : particle_emitter_configuration->get_materials_configurations())
            {
                std::shared_ptr<material_configuration> material_configuration =
                std::static_pointer_cast<gb::material_configuration>(iterator);
                
                std::shared_ptr<material> material = material::construct(material_configuration);
                gb::material::set_shader(material, material_configuration, m_resource_accessor);
                gb::material::set_textures(material, material_configuration, m_resource_accessor);
                particle_emitter->add_material(material_configuration->get_render_technique_name(), material);
            }
            m_game_objects_container.insert(particle_emitter);
        }
        return particle_emitter;
    }
    
    void fabricator::destroy_game_object(const game_object_shared_ptr& game_object)
    {
        m_game_objects_container.erase(game_object);
    }
}