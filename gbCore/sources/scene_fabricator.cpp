//
//  scene_fabricator.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/26/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "scene_fabricator.h"
#include "resource_accessor.h"
#include "mesh.h"
#include "instanced_mesh.h"
#include "material.h"
#include "sequence.h"
#include "animation_mixer.h"
#include "camera.h"
#include "shadow_cast_light.h"
#include "omni_light.h"
#include "instanced_omni_lights.h"
#include "direction_light.h"
#include "model3d_static.h"
#include "instanced_models3d_static.h"
#include "model3d_animated.h"
#include "particle_emitter.h"
#include "skybox.h"
#include "ocean.h"
#include "heightmap.h"
#include "mesh_constructor.h"
#include "heightmap_configuration.h"
#include "texture_configuration.h"
#include "heightmap_texture_generator.h"

namespace gb
{
    scene_fabricator::scene_fabricator()
    {
        
    }
    
    scene_fabricator::~scene_fabricator()
    {
        
    }
    
    void scene_fabricator::add_materials(const renderable_interface_shared_ptr& renderable_object, const std::vector<std::shared_ptr<configuration>>& configurations)
    {
        for(const auto& iterator : configurations)
        {
            std::shared_ptr<material_configuration> material_configuration =
            std::static_pointer_cast<gb::material_configuration>(iterator);
            
            material_shared_ptr material = material::construct(material_configuration);
            gb::material::set_shader(material, material_configuration, m_resource_accessor);
            gb::material::set_textures(material, material_configuration, m_resource_accessor);
            renderable_object->add_material(material_configuration->get_technique_name(), material_configuration->get_technique_pass(), material);
        }
    }
    
    camera_shared_ptr scene_fabricator::create_camera(f32 fov, f32 near, f32 far,const glm::ivec4& viewport)
    {
        camera_shared_ptr camera = std::make_shared<gb::camera>(fov, near, far, viewport);
        m_cameras_container.insert(camera);
        return camera;
    }
    
    void scene_fabricator::destroy_camera(const camera_shared_ptr& camera)
    {
        m_cameras_container.erase(camera);
    }
    
    std::once_flag g_omni_light_adds_created;
    omni_light_shared_ptr scene_fabricator::create_omni_light()
    {
        static shader_shared_ptr shader = nullptr;
        static mesh_shared_ptr mesh = nullptr;
        std::call_once(g_omni_light_adds_created, [this]{
            shader = shader::construct("omni_light",
                                       shader_omni_light_vert,
                                       shader_omni_light_frag);
            assert(shader);
            
            mesh = mesh_constructor::create_sphere(1.f, 8, 8);
            assert(mesh);
        });
        
        material_shared_ptr material = std::make_shared<gb::material>();
        material->set_shader(shader);
        
        material->set_culling(true);
        material->set_culling_mode(GL_BACK);
        
        material->set_blending(true);
        material->set_blending_function_source(GL_ONE);
        material->set_blending_function_destination(GL_ONE);
        
        material->set_stencil_test(false);
        material->set_stencil_function(GL_ALWAYS);
        material->set_stencil_function_parameter_1(1);
        material->set_stencil_function_parameter_2(255);
        material->set_stencil_mask_parameter(255);
        
        material->set_depth_test(false);
        material->set_depth_mask(false);
        
        material->set_clipping(false);
        material->set_clipping_plane(glm::vec4(0.f));
        
        material->set_reflecting(false);
        material->set_shadowing(false);
        material->set_debugging(false);
        
        texture_shared_ptr texture_01 = m_resource_accessor->get_texture("ws.forward.rendering.normal.depth");
        assert(texture_01);
        texture_shared_ptr texture_02 = m_resource_accessor->get_texture("ws.forward.rendering.normal.color");
        assert(texture_02);
        
        material->set_texture(texture_01, e_shader_sampler_01);
        material->set_texture(texture_02, e_shader_sampler_02);
        
        omni_light_shared_ptr omni_light = std::make_shared<gb::omni_light>();
        
        omni_light->add_material("ws.deferred.lighting", 0, material);
        omni_light->set_mesh(mesh);
        m_omni_lights_container.insert(omni_light);
        return omni_light;
    }
    
    void scene_fabricator::destroy_omni_light(const omni_light_shared_ptr& omni_light)
    {
         m_omni_lights_container.erase(omni_light);
    }
    
    std::once_flag g_instanced_omni_light_adds_created;
    instanced_omni_lights_shared_ptr scene_fabricator::create_instanced_omni_lights(i32 num_instances)
    {
        static shader_shared_ptr shader = nullptr;
        std::call_once(g_instanced_omni_light_adds_created, [this]{
            shader = shader::construct("instanced_omni_light",
                                       shader_instanced_omni_light_vert,
                                       shader_instanced_omni_light_frag);
            assert(shader);
        });
        
        material_shared_ptr material = std::make_shared<gb::material>();
        material->set_shader(shader);
        
        material->set_culling(true);
        material->set_culling_mode(GL_BACK);
        
        material->set_blending(true);
        material->set_blending_function_source(GL_ONE);
        material->set_blending_function_destination(GL_ONE);
        
        material->set_stencil_test(false);
        material->set_stencil_function(GL_ALWAYS);
        material->set_stencil_function_parameter_1(1);
        material->set_stencil_function_parameter_2(255);
        material->set_stencil_mask_parameter(255);
        
        material->set_depth_test(false);
        material->set_depth_mask(false);
        
        material->set_clipping(false);
        material->set_clipping_plane(glm::vec4(0.f));
        
        material->set_reflecting(false);
        material->set_shadowing(false);
        material->set_debugging(false);
        
        texture_shared_ptr texture_01 = m_resource_accessor->get_texture("ws.forward.rendering.normal.color");
        assert(texture_01);
        texture_shared_ptr texture_02 = m_resource_accessor->get_texture("ws.forward.rendering.normal.depth");
        assert(texture_02);
        
        material->set_texture(texture_01, e_shader_sampler_01);
        material->set_texture(texture_02, e_shader_sampler_02);
        
        instanced_mesh_shared_ptr mesh = mesh_constructor::create_spheres(num_instances, 1.f, 8, 8);
        
        instanced_omni_lights_shared_ptr instanced_omni_light = std::make_shared<gb::instanced_omni_lights>(num_instances);
        
        instanced_omni_light->add_material("ws.deferred.lighting", 0, material);
        instanced_omni_light->set_mesh(mesh);
        m_instanced_omni_lights_container.insert(instanced_omni_light);
        return instanced_omni_light;
    }
    
    void scene_fabricator::destroy_instanced_omni_lights(const instanced_omni_lights_shared_ptr& instanced_omni_lights)
    {
        m_instanced_omni_lights_container.erase(instanced_omni_lights);
    }
    
    std::once_flag g_direction_light_shader_created;
    direction_light_shared_ptr scene_fabricator::create_direction_light()
    {
        static shader_shared_ptr shader = nullptr;
        static mesh_shared_ptr mesh = nullptr;
        std::call_once(g_direction_light_shader_created, [this]{
            shader = shader::construct("direction_light",
                                       shader_direction_light_vert,
                                       shader_direction_light_frag);
            assert(shader);
            
            mesh = mesh_constructor::create_screen_quad();
            assert(mesh);
        });
        
        material_shared_ptr material = std::make_shared<gb::material>();
        material->set_shader(shader);
        
        material->set_culling(false);
        material->set_culling_mode(GL_BACK);
        
        material->set_blending(true);
        material->set_blending_function_source(GL_ONE);
        material->set_blending_function_destination(GL_ONE);
        
        material->set_stencil_test(false);
        material->set_stencil_function(GL_ALWAYS);
        material->set_stencil_function_parameter_1(1);
        material->set_stencil_function_parameter_2(255);
        material->set_stencil_mask_parameter(255);
        
        material->set_depth_test(false);
        material->set_depth_mask(false);
        
        material->set_clipping(false);
        material->set_clipping_plane(glm::vec4(0.f));
        
        material->set_reflecting(false);
        material->set_shadowing(false);
        material->set_debugging(false);
        
        texture_shared_ptr texture_01 = m_resource_accessor->get_texture("ws.forward.rendering.normal.color");
        assert(texture_01);
        texture_shared_ptr texture_02 = m_resource_accessor->get_texture("ws.forward.rendering.normal.depth");
        assert(texture_02);
        
        material->set_texture(texture_01, e_shader_sampler_01);
        material->set_texture(texture_02, e_shader_sampler_02);
        
        direction_light_shared_ptr direction_light = std::make_shared<gb::direction_light>();
        
        direction_light->add_material("ws.deferred.lighting", 0, material);
        direction_light->set_mesh(mesh);
        m_direction_lights_container.insert(direction_light);
        return direction_light;
    }
    
    void scene_fabricator::destroy_direction_light(const direction_light_shared_ptr &direction_light)
    {
        m_direction_lights_container.erase(direction_light);
    }
    
    shadow_cast_light_shared_ptr scene_fabricator::create_shadow_cast_light(f32 fov, f32 near, f32 far)
    {
        shadow_cast_light_shared_ptr shadow_cast_light = std::make_shared<gb::shadow_cast_light>(fov, near, far);
        m_shadow_cast_lights_container.insert(shadow_cast_light);
        return shadow_cast_light;
    }
    
    void scene_fabricator::destroy_shadow_cast_light(const shadow_cast_light_shared_ptr& shadow_cast_light)
    {
        m_shadow_cast_lights_container.erase(shadow_cast_light);
    }
    
    model3d_static_shared_ptr scene_fabricator::create_model3d_static(const std::string& filename)
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
                else if(model_configuration->get_mesh_base_class() == "box")
                {
                    mesh = mesh_constructor::create_box(glm::vec3(-.5f), glm::vec3(.5f));
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
            scene_fabricator::add_materials(model3d_static, model_configuration->get_materials_configurations());
            m_game_objects_container.insert(model3d_static);
        }
        return model3d_static;
    }
    
    model3d_animated_shared_ptr scene_fabricator::create_model3d_animated(const std::string &filename)
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
            
            scene_fabricator::add_materials(model3d_animated, model_configuration->get_materials_configurations());
            m_game_objects_container.insert(model3d_animated);
        }
        return model3d_animated;
    }
    
    particle_emitter_shared_ptr scene_fabricator::create_particle_emitter(const std::string& filename)
    {
        std::shared_ptr<particle_emitter_configuration> particle_emitter_configuration =
        std::static_pointer_cast<gb::particle_emitter_configuration>(m_configuration_accessor->get_particle_emitter_configuration(filename));
        assert(particle_emitter_configuration);
        particle_emitter_shared_ptr particle_emitter = nullptr;
        if(particle_emitter_configuration)
        {
            particle_emitter = std::make_shared<gb::particle_emitter>();
            particle_emitter->set_settings(particle_emitter_configuration);
            
            scene_fabricator::add_materials(particle_emitter, particle_emitter_configuration->get_materials_configurations());
            m_game_objects_container.insert(particle_emitter);
        }
        return particle_emitter;
    }
    
    instanced_models3d_static_shared_ptr scene_fabricator::create_instanced_models3d_static(const std::string& filename, i32 num_instances)
    {
        std::shared_ptr<model_configuration> model_configuration =
        std::static_pointer_cast<gb::model_configuration>(m_configuration_accessor->get_model_configuration(filename));
        assert(model_configuration);
        instanced_models3d_static_shared_ptr instanced_models3d_static = nullptr;
        if(model_configuration)
        {
            instanced_models3d_static = std::make_shared<gb::instanced_models3d_static>(num_instances);
            
            instanced_mesh_shared_ptr mesh = nullptr;
            if(model_configuration->get_mesh_base_class().length() != 0)
            {
                if(model_configuration->get_mesh_base_class() == "box")
                {
                    mesh = mesh_constructor::create_boxes(glm::vec3(-.5f), glm::vec3(.5f), num_instances);
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
            instanced_models3d_static->set_mesh(mesh);
            
            scene_fabricator::add_materials(instanced_models3d_static, model_configuration->get_materials_configurations());
            m_game_objects_container.insert(instanced_models3d_static);
        }
        return instanced_models3d_static;
    }
    
    void scene_fabricator::destroy_game_object(const game_object_shared_ptr& game_object)
    {
        m_game_objects_container.erase(game_object);
    }
    
    skybox_shared_ptr scene_fabricator::create_skybox(const std::string& filename)
    {
        std::shared_ptr<skybox_configuration> skybox_configuration =
        std::static_pointer_cast<gb::skybox_configuration>(m_configuration_accessor->get_skybox_configuration(filename));
        assert(skybox_configuration);
        skybox_shared_ptr skybox = nullptr;
        if(skybox_configuration)
        {
            skybox = std::make_shared<gb::skybox>();
            
            mesh_shared_ptr mesh = mesh_constructor::create_box(glm::vec3(-1.f), glm::vec3(1.f));
            assert(mesh);
            skybox->set_mesh(mesh);
            
            scene_fabricator::add_materials(skybox, skybox_configuration->get_materials_configurations());
            m_game_objects_container.insert(skybox);
        }
        return skybox;
    }
    
    void scene_fabricator::destroy_skybox(const skybox_shared_ptr& skybox)
    {
        m_game_objects_container.erase(skybox);
    }
    
    ocean_shared_ptr scene_fabricator::create_ocean(const std::string& filename)
    {
        std::shared_ptr<ocean_configuration> ocean_configuration =
        std::static_pointer_cast<gb::ocean_configuration>(m_configuration_accessor->get_ocean_configuration(filename));
        assert(ocean_configuration);
        ocean_shared_ptr ocean = nullptr;
        if(ocean_configuration)
        {
            ocean = std::make_shared<gb::ocean>();
            
            mesh_shared_ptr mesh = mesh_constructor::create_ocean_plane(ocean_configuration->get_size(), ocean_configuration->get_altitude());
            assert(mesh);
            ocean->set_mesh(mesh);
            
            ocean->set_wave_generator_interval(ocean_configuration->get_wave_generation_interval());
            
            scene_fabricator::add_materials(ocean, ocean_configuration->get_materials_configurations());
            m_game_objects_container.insert(ocean);
        }
        return ocean;
    }
    
    void scene_fabricator::destroy_ocean(const ocean_shared_ptr& ocean)
    {
        m_game_objects_container.erase(ocean);
    }
    
    heightmap_shared_ptr scene_fabricator::create_heightmap(const std::string& filename)
    {
        std::shared_ptr<heightmap_configuration> heightmap_configuration =
        std::static_pointer_cast<gb::heightmap_configuration>(m_configuration_accessor->get_heightmap_configuration(filename));
        assert(heightmap_configuration);
        
        heightmap_shared_ptr heightmap = nullptr;
        
        if(heightmap_configuration)
        {
            std::shared_ptr<material_configuration> splatting_configuration = std::make_shared<material_configuration>();
            splatting_configuration->serialize(heightmap_configuration->get_splatting_material_filename());
            
            std::array<texture_shared_ptr, heightmap_texture_generator::e_splatting_texture_max> splatting_diffuse_textures;
            std::array<texture_shared_ptr, heightmap_texture_generator::e_splatting_texture_max> splatting_normal_textures;
            std::array<texture_shared_ptr, heightmap_texture_generator::e_splatting_texture_max> splatting_displace_textures;
            
            assert(splatting_configuration->get_textures_configurations().size() == 9);
            ui32 index = 0;
            for(const auto& iterator : splatting_configuration->get_textures_configurations())
            {
                std::shared_ptr<texture_configuration> texture_configuration = std::static_pointer_cast<gb::texture_configuration>(iterator);
                assert(texture_configuration);
                assert(texture_configuration->get_texture_filename().length() != 0);
                texture_shared_ptr texture = m_resource_accessor->get_texture(texture_configuration->get_texture_filename(), true);
                if(index >= 6)
                {
                    splatting_displace_textures[index - 6] = texture;
                }
                else if(index >= 3)
                {
                    splatting_normal_textures[index - 3] = texture;
                }
                else
                {
                    splatting_diffuse_textures[index] = texture;
                }
                index++;
            }
            
            heightmap = std::make_shared<gb::heightmap>(heightmap_configuration->get_heightmap_data_filename());
            
            heightmap->set_splatting_diffuse_textures(splatting_diffuse_textures);
            heightmap->set_splatting_normal_textures(splatting_normal_textures);
            heightmap->set_splatting_displace_textures(splatting_displace_textures);
            
            for(const auto& iterator : heightmap_configuration->get_materials_configurations())
            {
                std::shared_ptr<material_configuration> material_configuration =
                std::static_pointer_cast<gb::material_configuration>(iterator);
                
                material_shared_ptr material = material::construct(material_configuration);
                gb::material::set_shader(material, material_configuration, m_resource_accessor);
                gb::material::set_textures(material, material_configuration, m_resource_accessor);
                heightmap->add_material(material_configuration->get_technique_name(), material_configuration->get_technique_pass(), material);
            }
            
            m_heightmaps_container.insert(heightmap);
        }
        return heightmap;
    }
    
    void scene_fabricator::destroy_heightmap(const heightmap_shared_ptr& heightmap)
    {
        m_heightmaps_container.erase(heightmap);
    }
}