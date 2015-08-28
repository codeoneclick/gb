//
//  render_technique_ws.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/17/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "render_technique_ws.h"
#include "texture.h"
#include "camera.h"
#include "frustum.h"
#include "mesh.h"
#include "ces_entity.h"
#include "ces_render_component.h"
#include "ces_geometry_component.h"
#include "ces_camera_component.h"
#include "ces_frustum_culling_component.h"
#include "ces_transformation_component.h"
#include "ces_global_light_component.h"

namespace gb
{
    render_technique_ws::render_technique_ws(ui32 width, ui32 height, const std::string& name, ui32 index) :
    gb::render_technique_base(width, height, name, index)
    {
        ui32 color_attachment_id;
        gl_create_textures(1, &color_attachment_id);
        gl_bind_texture(GL_TEXTURE_2D, color_attachment_id);
        gl_texture_parameter_i(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        gl_texture_parameter_i(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        gl_texture_parameter_i(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        gl_texture_parameter_i(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        gl_texture_image2d(GL_TEXTURE_2D, 0, GL_RGBA, m_frame_width, m_frame_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        
        ui32 depth_attachment_id;
        gl_create_textures(1, &depth_attachment_id);
        gl_bind_texture(GL_TEXTURE_2D, depth_attachment_id);
        gl_texture_parameter_i(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        gl_texture_parameter_i(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        gl_texture_parameter_i(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        gl_texture_parameter_i(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
#if defined(__OSX__)
        gl_texture_image2d(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_frame_width, m_frame_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
#else
#if defined(__OPENGL_30__)
        gl_texture_image2d(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_frame_width, m_frame_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
#else
        gl_texture_image2d(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_frame_width, m_frame_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
#endif
#endif
        
        gl_create_frame_buffers(1, &m_frame_buffer);
        gl_bind_frame_buffer(GL_FRAMEBUFFER, m_frame_buffer);
        gl_attach_frame_buffer_texture2d(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment_id, 0);
        gl_attach_frame_buffer_texture2d(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_attachment_id, 0);
        
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        assert(status == GL_FRAMEBUFFER_COMPLETE);
        
        std::string color_attachment_guid = m_name;
        color_attachment_guid.append(".color");
        m_color_attachment_texture = texture::construct(color_attachment_guid,
                                                        color_attachment_id,
                                                        m_frame_width,
                                                        m_frame_height);
        
        m_color_attachment_texture->set_wrap_mode(GL_CLAMP_TO_EDGE);
        
        std::string depth_attachment_guid = m_name;
        depth_attachment_guid.append(".depth");
        m_depth_attachment_texture = texture::construct(depth_attachment_guid,
                                                        depth_attachment_id,
                                                        m_frame_width,
                                                        m_frame_height);
        m_depth_attachment_texture->set_wrap_mode(GL_CLAMP_TO_EDGE);
    }
    
    render_technique_ws::~render_technique_ws(void)
    {
        
    }
    
    std::shared_ptr<texture> render_technique_ws::get_color_attachment_texture(void) const
    {
        assert(m_color_attachment_texture != nullptr);
        return m_color_attachment_texture;
    }
    
    std::shared_ptr<texture> render_technique_ws::get_depth_attachment_texture(void) const
    {
        assert(m_depth_attachment_texture != nullptr);
        return m_depth_attachment_texture;
    }
    
    void render_technique_ws::add_entity(const std::shared_ptr<ces_entity>& entity)
    {
        m_entities.push(entity);
    }
    
    void render_technique_ws::bind(void)
    {
        gl_bind_frame_buffer(GL_FRAMEBUFFER, m_frame_buffer);
        gl_viewport(0, 0, m_frame_width, m_frame_height);
        gl_clear_color(m_clear_color.r, m_clear_color.g, m_clear_color.b, m_clear_color.a);
        gl_clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    void render_technique_ws::unbind(void)
    {
        
    }
    
    void render_technique_ws::draw(void)
    {
        while (!m_entities.empty())
        {
            std::shared_ptr<ces_entity> entity = m_entities.front();
            std::shared_ptr<ces_render_component> render_component = std::static_pointer_cast<ces_render_component>(entity->get_component(e_ces_component_type_render));
            std::shared_ptr<ces_geometry_component> geometry_component = std::static_pointer_cast<ces_geometry_component>(entity->get_component(e_ces_component_type_geometry));
            std::shared_ptr<ces_camera_component> camera_component = std::static_pointer_cast<ces_camera_component>(entity->get_component(e_ces_component_type_camera));
            std::shared_ptr<ces_frustum_culling_component> frustum_component =
            std::static_pointer_cast<ces_frustum_culling_component>(entity->get_component(e_ces_component_type_frustum_culling));
            std::shared_ptr<ces_transformation_component> transformation_component =
            std::static_pointer_cast<ces_transformation_component>(entity->get_component(e_ces_component_type_transformation));
            std::shared_ptr<ces_global_light_component> global_light_component =
            std::static_pointer_cast<ces_global_light_component>(entity->get_component(e_ces_component_type_global_light));
            
            assert(render_component);
            assert(geometry_component);
            assert(camera_component);
            
            std::shared_ptr<material> material = render_component->on_bind(m_name);
            render_component->bind_camera_uniforms(m_name, camera_component->get_camera(), material);
            
            if(global_light_component)
            {
                render_component->bind_global_light_uniforms(m_name, global_light_component->get_global_light(), material);
            }
            
            if(transformation_component)
            {
                render_component->bind_transformation_uniforms(m_name, ces_transformation_component::get_matrix_m(transformation_component),
                                                               ces_transformation_component::get_matrix_mvp(transformation_component, camera_component->get_camera()->get_matrix_vp()),
                                                               ces_transformation_component::get_matrix_imvp(transformation_component, camera_component->get_camera()->get_matrix_ivp()));
            }
            
            if(frustum_component)
            {
                glm::vec3 min_bound = geometry_component->get_mesh()->get_min_bound();
                glm::vec3 max_bound = geometry_component->get_mesh()->get_max_bound();
                /*if(transformation_component)
                {
                    min_bound = geometry_component->get_mesh()->get_min_bound(ces_transformation_component::get_matrix_m(transformation_component));
                    max_bound = geometry_component->get_mesh()->get_max_bound(ces_transformation_component::get_matrix_m(transformation_component));
                }*/
                if(frustum_component->get_frustum()->is_bounding_box_in_frustum(min_bound, max_bound))
                {
                    render_component->on_draw(m_name, geometry_component->get_mesh(), material);
                }
            }
            else
            {
                render_component->on_draw(m_name, geometry_component->get_mesh(), material);
            }
            m_entities.pop();
        }
    }
}