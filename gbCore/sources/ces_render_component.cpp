//
//  ces_render_component.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "ces_render_component.h"
#include "texture.h"
#include "mesh.h"
#include "camera.h"
#include "global_light.h"

namespace gb
{
    ces_render_component::ces_render_component(void)
    {
        m_type = e_ces_component_type_render;
    }
    
    ces_render_component::~ces_render_component(void)
    {
        m_materials.clear();
    }
    
    void ces_render_component::add_material(const std::string& technique_name, const std::shared_ptr<material>& material)
    {
        m_materials[technique_name] = material;
    }
    
    void ces_render_component::remove_material(const std::string& technique_name)
    {
        const auto& iterator = m_materials.find(technique_name);
        if(iterator != m_materials.end())
        {
            m_materials.erase(iterator);
        }
    }
    
    std::shared_ptr<material> ces_render_component::get_material(const std::string& technique_name) const
    {
        const auto& iterator = m_materials.find(technique_name);
        std::shared_ptr<material> material = iterator != m_materials.end() ? iterator->second : nullptr;
        return material;
    }
    
    void ces_render_component::set_z_order(i32 z_order)
    {
        
    }
    
    i32 ces_render_component::get_z_order(void) const
    {
        return m_z_order;
    }
    
    void ces_render_component::set_texture(const std::shared_ptr<texture> &texture, e_shader_sampler sampler,  const std::string& technique_name)
    {
        if(technique_name.length() != 0)
        {
            std::shared_ptr<material> material = ces_render_component::get_material(technique_name);
            assert(material);
            material->set_texture(texture, sampler);
        }
        else
        {
            std::for_each(m_materials.cbegin(), m_materials.cend(), [texture, sampler](std::pair<std::string, std::shared_ptr<material>> material) {
                material.second->set_texture(texture, sampler);
            });
        }
    }
    
    void ces_render_component::bind_custom_shader_uniforms(const std::shared_ptr<material>& material)
    {
        assert(material != nullptr);
        std::map<std::string, std::shared_ptr<shader_uniform>> custom_shader_uniforms = material->get_custom_uniforms();
        std::shared_ptr<shader_uniform> current_uniform = nullptr;
        for(const auto& iterator : custom_shader_uniforms)
        {
            current_uniform = iterator.second;
            assert(current_uniform != nullptr);
            switch (current_uniform->get_type())
            {
                case e_uniform_type_mat4:
                {
                    material->get_shader()->set_custom_mat4(current_uniform->get_mat4(), iterator.first);
                }
                    break;
                    
                case e_uniform_type_mat3:
                {
                    material->get_shader()->set_custom_mat3(current_uniform->get_mat3(), iterator.first);
                }
                    break;
                    
                case e_uniform_type_vec4:
                {
                    material->get_shader()->set_custom_vec4(current_uniform->get_vec4(), iterator.first);
                }
                    break;
                    
                case e_uniform_type_vec3:
                {
                    material->get_shader()->set_custom_vec3(current_uniform->get_vec3(), iterator.first);
                }
                    break;
                    
                case e_uniform_type_vec2:
                {
                    material->get_shader()->set_custom_vec2(current_uniform->get_vec2(), iterator.first);
                }
                    break;
                    
                case e_uniform_type_f32:
                {
                    material->get_shader()->set_custom_f32(current_uniform->get_f32(), iterator.first);
                }
                    break;
                    
                case e_uniform_type_i32:
                {
                    material->get_shader()->set_custom_i32(current_uniform->get_i32(), iterator.first);
                }
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    void ces_render_component::bind_transformation_uniforms(const std::string& technique_name,
                                                            const glm::mat4& matrix_m,
                                                            const glm::mat4& matrix_mvp,
                                                            const glm::mat4& matrix_imvp, const std::shared_ptr<material>& material)
    {
        std::shared_ptr<gb::material> using_material = material;
        if(!using_material)
        {
            using_material = ces_render_component::get_material(technique_name);
        }
        assert(using_material);
        using_material->get_shader()->set_mat4(matrix_m, e_shader_uniform_mat_m);
        using_material->get_shader()->set_mat4(material->is_reflecting() ? matrix_imvp : matrix_mvp, e_shader_uniform_mat_mvp);
    }
    
    void ces_render_component::bind_camera_uniforms(const std::string& technique_name,
                                                    const std::shared_ptr<camera>& camera, const std::shared_ptr<material>& material)
    {
        std::shared_ptr<gb::material> using_material = material;
        if(!using_material)
        {
            using_material = ces_render_component::get_material(technique_name);
        }
        assert(using_material);
        
        using_material->get_shader()->set_mat4(camera->get_matrix_p(), e_shader_uniform_mat_p);
        using_material->get_shader()->set_mat4(!material->is_reflecting() ? camera->get_matrix_v() : camera->get_matrix_iv(), e_shader_uniform_mat_v);
        using_material->get_shader()->set_mat4(!material->is_reflecting() ? camera->get_matrix_vp() : camera->get_matrix_ivp(), e_shader_uniform_mat_vp);
        using_material->get_shader()->set_mat4(camera->get_matrix_n(), e_shader_uniform_mat_n);
        
        using_material->get_shader()->set_vec3(camera->get_position(), e_shader_uniform_vec_camera_position);
        using_material->get_shader()->set_f32(camera->get_near(), e_shader_uniform_f32_camera_near);
        using_material->get_shader()->set_f32(camera->get_far(), e_shader_uniform_f32_camera_far);
        using_material->get_shader()->set_vec4(material->get_clipping_plane(), e_shader_uniform_vec_clip);
    }
    
    void ces_render_component::bind_global_light_uniforms(const std::string& technique_name,
                                                          const std::shared_ptr<global_light>& global_light, const std::shared_ptr<material>& material)
    {
        std::shared_ptr<gb::material> using_material = material;
        if(!using_material)
        {
            using_material = ces_render_component::get_material(technique_name);
        }
        assert(using_material);
        
        using_material->get_shader()->set_vec3(global_light->get_position(), e_shader_uniform_vec_global_light_position);
        using_material->get_shader()->set_mat4(global_light->get_matrix_p(), e_shader_uniform_mat_global_light_p);
        using_material->get_shader()->set_mat4(global_light->get_matrix_v(), e_shader_uniform_mat_global_light_v);
    }
    
    std::shared_ptr<material> ces_render_component::on_bind(const std::string& technique_name)
    {
        std::shared_ptr<gb::material> material = ces_render_component::get_material(technique_name);
        assert(material);
        material->bind();
        ces_render_component::bind_custom_shader_uniforms(material);
        return material;
    }
    
    void ces_render_component::on_unbind(const std::string& technique_name,
                                         const std::shared_ptr<material>& material)
    {
        std::shared_ptr<gb::material> using_material = material;
        if(!using_material)
        {
            using_material = ces_render_component::get_material(technique_name);
        }
        assert(using_material);
        using_material->unbind();
    }

    void ces_render_component::on_draw(const std::string& technique_name, const std::shared_ptr<mesh>& mesh, const std::shared_ptr<material>& material)
    {
        std::shared_ptr<gb::material> using_material = material;
        if(!using_material)
        {
            using_material = ces_render_component::get_material(technique_name);
        }
        assert(using_material);

        mesh->bind(using_material->get_shader()->get_guid(), using_material->get_shader()->get_attributes());
    }
}