//
//  heightmap_texture_generator.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "heightmap_texture_generator.h"
#include "texture.h"
#include "material.h"
#include "shader.h"
#include "graphics_context.h"
#include "built_in_shaders.h"
#include "render_target.h"
#include "mesh.h"
#include "mesh_constructor.h"
#include "heightmap_loader.h"
#include "heightmap_accessor.h"

namespace gb
{
    const f32 heightmap_texture_generator::k_min_splatting_texture_height = 32.f;
    const f32 heightmap_texture_generator::k_max_splatting_texture_height = 32.f;
    const ui8 heightmap_texture_generator::k_splatting_texture_channels = 4;
    const glm::ivec2 heightmap_texture_generator::k_splatting_texture_mask_size = glm::ivec2(64);
    const glm::ivec2 heightmap_texture_generator::k_splatting_texture_size_lod_01 = glm::ivec2(1024);
    const glm::ivec2 heightmap_texture_generator::k_splatting_texture_size_lod_02 = glm::ivec2(512);
    const glm::ivec2 heightmap_texture_generator::k_splatting_texture_size_lod_03 = glm::ivec2(128);
    const glm::ivec2 heightmap_texture_generator::k_splatting_texture_size_lod_04 = glm::ivec2(64);
    
    std::once_flag g_create_splatting_textures_material_once;
    material_shared_ptr heightmap_texture_generator::get_splatting_textures_material(const std::array<texture_shared_ptr,
                                                                                     e_splatting_texture_max>& splatting_textures)
    {
        static material_shared_ptr material = std::make_shared<gb::material>();
        std::call_once(g_create_splatting_textures_material_once, []() {
            
            shader_shared_ptr shader = shader::construct("splatting_textures", shader_splatting_textures_vert, shader_splatting_textures_frag);
            assert(shader != nullptr);
            material->set_shader(shader);
            
            material->set_culling(false);
            material->set_culling_mode(GL_BACK);
            
            material->set_stencil_test(false);
            material->set_stencil_function(GL_ALWAYS);
            material->set_stencil_function_parameter_1(1);
            material->set_stencil_function_parameter_2(255);
            material->set_stencil_mask_parameter(255);
            
            material->set_blending(false);
            material->set_blending_function_source(GL_SRC_ALPHA);
            material->set_blending_function_destination(GL_ONE);
            
            material->set_depth_test(false);
            material->set_depth_mask(true);
            
            material->set_clipping(false);
            material->set_clipping_plane(glm::vec4(0.0, 0.0, 0.0, 0.0));
            
            material->set_reflecting(false);
            material->set_shadowing(false);
            material->set_debugging(false);
        });
        
        assert(splatting_textures[0]->is_loaded() && splatting_textures[0]->is_commited());
        material->set_texture(splatting_textures[0], e_shader_sampler_01);
        assert(splatting_textures[1]->is_loaded() && splatting_textures[1]->is_commited());
        material->set_texture(splatting_textures[1], e_shader_sampler_02);
        assert(splatting_textures[2]->is_loaded() && splatting_textures[2]->is_commited());
        material->set_texture(splatting_textures[2], e_shader_sampler_03);
        
        return material;
    }
    
    std::once_flag g_create_render_targets_once;
    std::once_flag g_create_splatting_mask_texture;
    void heightmap_texture_generator::generate_splatting_texture(const std::shared_ptr<graphics_context> &graphics_context,
                                                                 const std::shared_ptr<heightmap_container> &container,
                                                                 const std::array<texture_shared_ptr, e_splatting_texture_max> &splatting_textures,
                                                                 ui32 i, ui32 j, const std::function<void (ui8 *, ui32, heightmap_container::e_heigtmap_chunk_lod)> &callback)
    {
        material_shared_ptr material = heightmap_texture_generator::get_splatting_textures_material(splatting_textures);
        mesh_shared_ptr screen_quad = mesh_constructor::create_screen_quad();
        
        static std::array<std::shared_ptr<render_target>, heightmap_container::e_heigtmap_chunk_lod_max> render_targets;
        std::call_once(g_create_render_targets_once, [container, graphics_context]() {
            for(ui32 i = 0; i < heightmap_container::e_heigtmap_chunk_lod_max; ++i)
            {
                glm::ivec2 texture_size = container->get_textures_lod_size(static_cast<heightmap_container::e_heigtmap_chunk_lod>(i));
                render_targets[i] = std::make_shared<render_target>(graphics_context, GL_RGBA,
                                                                    texture_size.x, texture_size.y);
            }
        });
        
        glm::ivec2 mask_texture_size = glm::ivec2(sqrt(container->get_splatting_mask_textures_mmap(0)->get_size()));
        static texture_shared_ptr splatting_mask_texture = nullptr;
        std::call_once(g_create_splatting_mask_texture, [container, material, mask_texture_size]() {
            
            ui32 texture_id;
            gl_create_textures(1, &texture_id);
            
            splatting_mask_texture = texture::construct("splatting.MTexture", texture_id,
                                                        mask_texture_size.x, mask_texture_size.y);
            
            splatting_mask_texture->set_wrap_mode(GL_CLAMP_TO_EDGE);
            splatting_mask_texture->set_mag_filter(GL_LINEAR);
            splatting_mask_texture->set_min_filter(GL_LINEAR);
            material->set_texture(splatting_mask_texture, e_shader_sampler_04);
        });
        
        for(ui32 lod = 0; lod < heightmap_container::e_heigtmap_chunk_lod_max; ++lod)
        {
            glm::ivec2 diffuse_texture_size = container->get_textures_lod_size(static_cast<heightmap_container::e_heigtmap_chunk_lod>(lod));
            ui32 data_size = static_cast<ui32>(diffuse_texture_size.x) * static_cast<ui32>(diffuse_texture_size.y) * k_splatting_texture_channels;
            ui8 *data = new ui8[data_size];
            
            ui32 index = i + j * container->get_chunks_num().x;
            splatting_mask_texture->bind();
            
            gl_texture_image2d(GL_TEXTURE_2D, 0, GL_RGB,
                               mask_texture_size.x, mask_texture_size.y,
                               0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5,
                               container->get_splatting_mask_textures_mmap(index)->get_pointer());
            
            render_targets[lod]->begin();
            render_targets[lod]->clear();
            
            material->bind();
            assert(material->get_shader()->get_attributes().at(e_shader_attribute_position) >= 0);
            assert(material->get_shader()->get_attributes().at(e_shader_attribute_texcoord) >= 0);
            screen_quad->bind(material->get_shader()->get_guid(), material->get_shader()->get_attributes());
            
            screen_quad->draw();
            
            screen_quad->unbind(material->get_shader()->get_guid(), material->get_shader()->get_attributes());
            material->unbind();
            
            render_targets[lod]->end(data);
            
            assert(callback);
            callback(data, data_size, static_cast<heightmap_container::e_heigtmap_chunk_lod>(lod));
            delete [] data;
        }
    }
    
    void heightmap_texture_generator::generate_splatting_mask_textures(const std::shared_ptr<heightmap_container>& container, const std::string& filename)
    {
        if(!heightmap_loader::is_splatting_mask_textures_mmap_exist(filename))
        {
            heightmap_texture_generator::create_splatting_mask_textures(container, filename);
        }
    }
    
    void heightmap_texture_generator::create_splatting_mask_textures(const std::shared_ptr<heightmap_container>& container, const std::string& filename)
    {
        std::shared_ptr<std::ofstream> stream = std::make_shared<std::ofstream>();
        stream->open(heightmap_loader::get_splatting_mask_textures_mmap_filename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
        if(!stream->is_open())
        {
            assert(false);
        }
        
        for(ui32 i = 0; i < container->get_chunks_num().x; ++i)
        {
            for(ui32 j = 0; j < container->get_chunks_num().y; ++j)
            {
                heightmap_texture_generator::generate_splatting_mask_texture(container, i, j, stream);
            }
        }
        stream->close();
    }
    
    void heightmap_texture_generator::generate_splatting_mask_texture(const std::shared_ptr<heightmap_container> &container,
                                                                      ui32 i, ui32 j, const std::shared_ptr<std::ofstream> stream)
    {
        glm::ivec2 vertices_offset( i * (container->get_chunk_size().x - 1), j * (container->get_chunk_size().y - 1));
        glm::vec2 step = glm::vec2(static_cast<f32>(container->get_chunk_size().x) / static_cast<f32>(k_splatting_texture_mask_size.x) ,
                                   static_cast<f32>(container->get_chunk_size().y) / static_cast<f32>(k_splatting_texture_mask_size.y));
        
        glm::vec3 offset = glm::vec3(0.0f);
        f32 max_height = k_max_splatting_texture_height;
        
        ui16* pixels = new ui16[k_splatting_texture_mask_size.x * k_splatting_texture_mask_size.y];
        
        for(ui32 x = 0; x < k_splatting_texture_mask_size.x; ++x)
        {
            offset.z = 0.0f;
            for(ui32 y = 0; y < k_splatting_texture_mask_size.y; ++y)
            {
                ui32 index = x + y * k_splatting_texture_mask_size.x;
                
                pixels[index] = TO_RGB565(255, 0, 0);
                f32 height = heightmap_accessor::get_height(container, glm::vec3(offset.x + vertices_offset.x, 0.0f, offset.z + vertices_offset.y)) + heightmap_container::k_deep;
                glm::vec3 normal = heightmap_accessor::get_normal(container, glm::vec3(offset.x + vertices_offset.x, 0.0f, offset.z + vertices_offset.y));
                
                f32 normalized_height = height / max_height;
                ui8 red = normalized_height <= heightmap_container::k_layer_section_01 ? 255 : 0;
                if(normalized_height > heightmap_container::k_layer_section_01 &&
                   normalized_height <= heightmap_container::k_layer_section_01 + heightmap_container::k_layer_section_offset)
                {
                    f32 interpolation = (normalized_height - heightmap_container::k_layer_section_01) / heightmap_container::k_layer_section_offset;
                    red = glm::mix(255, 0, interpolation);
                }
                
                ui8 green = normalized_height > heightmap_container::k_layer_section_01 && normalized_height <= heightmap_container::k_layer_section_02 ? 255 : 0;
                if(normalized_height < heightmap_container::k_layer_section_01 &&
                   normalized_height >= heightmap_container::k_layer_section_01 - heightmap_container::k_layer_section_offset)
                {
                    f32 interpolation = (normalized_height - (heightmap_container::k_layer_section_01 - heightmap_container::k_layer_section_offset)) /
                    heightmap_container::k_layer_section_offset;
                    green = glm::mix(0, 255, interpolation);
                }
                else if(normalized_height > heightmap_container::k_layer_section_02 &&
                        normalized_height <= heightmap_container::k_layer_section_02 + heightmap_container::k_layer_section_offset)
                {
                    f32 interpolation = (normalized_height - heightmap_container::k_layer_section_02) / heightmap_container::k_layer_section_offset;
                    green = glm::mix(255, 0, interpolation);
                }
                
                ui8 blue = normalized_height > heightmap_container::k_layer_section_02 ? 255 : 0;
                if(normalized_height < heightmap_container::k_layer_section_02 &&
                   normalized_height >= heightmap_container::k_layer_section_02 - heightmap_container::k_layer_section_offset)
                {
                    f32 interpolation = (normalized_height - (heightmap_container::k_layer_section_02 - heightmap_container::k_layer_section_offset)) /
                    heightmap_container::k_layer_section_offset;
                    blue = glm::mix(0, 255, interpolation);
                }
                
                f32 angle = glm::dot(glm::vec3(0.f, 1.f, 0.f), normal);
                angle = glm::degrees(acosf(angle));
                assert(angle >= 0.0);
                angle = std::min(angle / 45.f, 1.f);
                blue = std::max(static_cast<ui8>(glm::mix(0, 255, angle)), blue);
                
                pixels[index] = TO_RGB565(red, green, blue);
                
                offset.z += step.y;
            }
            offset.x += step.x;
        }
        
        if(stream && stream->is_open())
        {
            stream->write((char*)&pixels[0], sizeof(ui16) * k_splatting_texture_mask_size.x * k_splatting_texture_mask_size.y);
        }
        else
        {
            ui32 index = i + j * container->get_chunks_num().x;
            assert(container->get_splatting_mask_textures_mmap(index)->get_pointer());
            memcpy(container->get_splatting_mask_textures_mmap(index)->get_pointer(), pixels,
                   sizeof(ui16) * k_splatting_texture_mask_size.x * k_splatting_texture_mask_size.y);
        }
        
        delete[] pixels;
        pixels = nullptr;
    }
    
    void heightmap_texture_generator::generate_splatting_diffuse_textures(const graphics_context_shared_ptr& graphics_context,
                                                                          const std::shared_ptr<heightmap_container>& container, const std::string& filename,
                                                                          const std::array<texture_shared_ptr, e_splatting_texture_max>& splatting_diffuse_textures)
    {
        if(!heightmap_loader::is_splatting_diffuse_textures_mmap_exist(filename))
        {
            heightmap_texture_generator::create_splatting_diffuse_textures(graphics_context, container, filename, splatting_diffuse_textures);
        }
    }
    
    void heightmap_texture_generator::create_splatting_diffuse_textures(const graphics_context_shared_ptr&  graphics_context,
                                                                        const std::shared_ptr<heightmap_container>& container, const std::string& filename,
                                                                        const std::array<texture_shared_ptr, e_splatting_texture_max>& splatting_diffuse_textures)
    {
        
        material_shared_ptr material = heightmap_texture_generator::get_splatting_textures_material(splatting_diffuse_textures);
        mesh_shared_ptr screen_quad = mesh_constructor::create_screen_quad();
        
        std::shared_ptr<std::ofstream> stream = std::make_shared<std::ofstream>();
        stream->open(heightmap_loader::get_splatting_diffuse_textures_mmap_filename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
        if(!stream->is_open())
        {
            assert(false);
        }
        
        for(ui32 i = 0; i < container->get_chunks_num().x; ++i)
        {
            for(ui32 j = 0; j < container->get_chunks_num().y; ++j)
            {
                heightmap_texture_generator::generate_splatting_diffuse_texture(graphics_context, container, splatting_diffuse_textures, i, j, stream);
            }
        }
        stream->close();
    }
    
    void heightmap_texture_generator::generate_splatting_diffuse_texture(const graphics_context_shared_ptr& graphics_context,
                                                                         const std::shared_ptr<heightmap_container>& container,
                                                                         const std::array<texture_shared_ptr, e_splatting_texture_max>& splatting_diffuse_textures,
                                                                         ui32 i, ui32 j, const std::shared_ptr<std::ofstream> stream)
    {
        ui32 index = i + j * container->get_chunks_num().x;
        heightmap_texture_generator::generate_splatting_texture(graphics_context, container, splatting_diffuse_textures, i, j, [stream, container, index](ui8 *data, ui32 size, heightmap_container::e_heigtmap_chunk_lod lod) {
            
            ui32 size_565 = size / 4;
            ui16* data_565 = new ui16[size_565];
            ui32 index = 0;
            for(ui32 i = 0; i < size; i += 4)
            {
                data_565[index] = TO_RGB565(data[i + 0], data[i + 1], data[i + 2]);
                index++;
            }
            
            if(stream)
            {
                stream->write((char *)data_565, size_565 * sizeof(ui16));
            }
            else
            {
                assert(container->get_splatting_diffuse_textures_mmap(index, lod)->get_pointer());
                memcpy(container->get_splatting_diffuse_textures_mmap(index, lod)->get_pointer(), data_565, size_565 * sizeof(ui16));
            }
            delete[] data_565;
        });
    }
    
    void heightmap_texture_generator::generate_splatting_normal_textures(const graphics_context_shared_ptr& graphics_context,
                                                                         const std::shared_ptr<heightmap_container>& container, const std::string& filename,
                                                                         const std::array<texture_shared_ptr, e_splatting_texture_max>& splatting_normal_textures)
    {
        if(!heightmap_loader::is_splatting_normal_textures_mmap_exist(filename))
        {
            heightmap_texture_generator::create_splatting_normal_textures(graphics_context, container, filename, splatting_normal_textures);
        }
    }
    
    void heightmap_texture_generator::create_splatting_normal_textures(const graphics_context_shared_ptr& graphics_context,
                                                                       const std::shared_ptr<heightmap_container>& container, const std::string& filename,
                                                                       const std::array<texture_shared_ptr, e_splatting_texture_max>& splatting_normal_textures)
    {
        material_shared_ptr material = heightmap_texture_generator::get_splatting_textures_material(splatting_normal_textures);
        mesh_shared_ptr screen_quad = mesh_constructor::create_screen_quad();
        
        std::shared_ptr<std::ofstream> stream = std::make_shared<std::ofstream>();
        stream->open(heightmap_loader::get_splatting_normal_textures_mmap_filename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
        if(!stream->is_open())
        {
            assert(false);
        }
        
        for(ui32 i = 0; i < container->get_chunks_num().x; ++i)
        {
            for(ui32 j = 0; j < container->get_chunks_num().y; ++j)
            {
                heightmap_texture_generator::generate_splatting_normal_texture(graphics_context, container, splatting_normal_textures, i, j, stream);
            }
        }
        stream->close();
    }
    
    void heightmap_texture_generator::generate_splatting_normal_texture(const graphics_context_shared_ptr& graphics_context,
                                                                        const std::shared_ptr<heightmap_container>& container,
                                                                        const std::array<texture_shared_ptr, e_splatting_texture_max>& splatting_normal_textures,
                                                                        ui32 i, ui32 j, const std::shared_ptr<std::ofstream> stream)
    {
        ui32 index = i + j * container->get_chunks_num().x;
        heightmap_texture_generator::generate_splatting_texture(graphics_context, container, splatting_normal_textures, i, j, [stream, container, index](ui8 *data, ui32 size, heightmap_container::e_heigtmap_chunk_lod lod) {
            if(stream)
            {
                stream->write((char *)data, size * sizeof(ui8));
            }
            else
            {
                assert(container->get_splatting_normal_textures_mmap(index, lod)->get_pointer());
                memcpy(container->get_splatting_normal_textures_mmap(index, lod)->get_pointer(), data, size * sizeof(ui8));
            }
        });
    }
    
    void heightmap_texture_generator::generate_splatting_displace_textures(const graphics_context_shared_ptr& graphics_context,
                                                                           const std::shared_ptr<heightmap_container>& container, const std::string& filename,
                                                                           const std::array<texture_shared_ptr, e_splatting_texture_max>& splatting_displace_textures)
    {
        if(!heightmap_loader::is_splatting_displace_textures_mmap_exist(filename))
        {
            heightmap_texture_generator::create_splatting_displace_textures(graphics_context, container, filename, splatting_displace_textures);
        }
    }
    
    void heightmap_texture_generator::create_splatting_displace_textures(const graphics_context_shared_ptr& graphics_context,
                                                                         const std::shared_ptr<heightmap_container>& container, const std::string& filename,
                                                                         const std::array<texture_shared_ptr, e_splatting_texture_max>& splatting_displace_textures)
    {
        material_shared_ptr material = heightmap_texture_generator::get_splatting_textures_material(splatting_displace_textures);
        mesh_shared_ptr screen_quad = mesh_constructor::create_screen_quad();
        
        std::shared_ptr<std::ofstream> stream = std::make_shared<std::ofstream>();
        stream->open(heightmap_loader::get_splatting_displace_textures_mmap_filename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
        if(!stream->is_open())
        {
            assert(false);
        }
        
        for(ui32 i = 0; i < container->get_chunks_num().x; ++i)
        {
            for(ui32 j = 0; j < container->get_chunks_num().y; ++j)
            {
                heightmap_texture_generator::generate_splatting_displace_texture(graphics_context, container, splatting_displace_textures, i, j, stream);
            }
        }
        stream->close();
    }
    
    void heightmap_texture_generator::generate_splatting_displace_texture(const graphics_context_shared_ptr& graphics_context,
                                                                          const std::shared_ptr<heightmap_container>& container,
                                                                          const std::array<texture_shared_ptr, e_splatting_texture_max>& splatting_displace_textures,
                                                                          ui32 i, ui32 j, const std::shared_ptr<std::ofstream> stream)
    {
        ui32 index = i + j * container->get_chunks_num().x;
        heightmap_texture_generator::generate_splatting_texture(graphics_context, container, splatting_displace_textures, i, j, [stream, container, index](ui8 *data, ui32 size, heightmap_container::e_heigtmap_chunk_lod lod) {
            if(stream)
            {
                stream->write((char *)data, size * sizeof(ui8));
            }
            else
            {
                assert(container->get_splatting_displace_textures_mmap(index, lod)->get_pointer());
                memcpy(container->get_splatting_displace_textures_mmap(index, lod)->get_pointer(), data, size * sizeof(ui8));
            }
        });
    }
    
    void heightmap_texture_generator::generate_deep_texture(const std::shared_ptr<heightmap_container>& container, bool create,
                                                            ui32 offset_x, ui32 offset_y,
                                                            ui32 sub_width, ui32 sub_height)
    {
        assert(container->get_deep_texture());
        container->get_deep_texture()->bind();
        
        ui8* data = nullptr;
        if(create)
        {
            data = new ui8[container->get_main_size().x * container->get_main_size().y];
            f32 max_deep = k_max_splatting_texture_height;
            for(int i = 0; i < container->get_main_size().x; i++)
            {
                for(int j = 0; j < container->get_main_size().y; j++)
                {
                    f32 height = heightmap_accessor::get_height(container, glm::vec3(i, 0.f, j));
                    height = height <= 0.f ? height : 0.f;
                    height /= max_deep;
                    height = std::max(0.f, std::min((height + 1.f) / 2.f, 1.f));
                    ui8 color = static_cast<ui8>(height * 255);
                    data[i + j * container->get_main_size().x] = color;
                }
            }
            
            gl_texture_image2d(GL_TEXTURE_2D, 0,
#if defined(__OPENGL_30__)
                               GL_RED,
#else
                               GL_ALPHA,
#endif
                               container->get_main_size().x,
                               container->get_main_size().y,
                               0,
#if defined(__OPENGL_30__)
                               GL_RED,
#else
                               GL_ALPHA,
#endif
                               GL_UNSIGNED_BYTE, data);
        }
        else
        {
            assert(offset_x >= 0);
            assert(offset_x + sub_width < container->get_deep_texture()->get_width());
            assert(offset_y >= 0);
            assert(offset_y + sub_height < container->get_deep_texture()->get_height());
            
            f32 max_deep = k_max_splatting_texture_height;
            
            data = new ui8[sub_width * sub_height];
            for(int i = 0; i < sub_width; i++)
            {
                for(int j = 0; j < sub_height; j++)
                {
                    f32 height = heightmap_accessor::get_height(container, glm::vec3(i + offset_x , 0.f, j + offset_y));
                    height = height <= 0.f ? height : 0.f;
                    height /= max_deep;
                    height = std::max(0.f, std::min((height + 1.f) / 2.f, 1.f));
                    ui8 color = static_cast<ui8>(height * 255);
                    data[i + j * sub_width] = color;
                }
            }
            glTexSubImage2D(GL_TEXTURE_2D, 0,
                            offset_x, offset_y,
                            sub_width, sub_height,
#if defined(__OPENGL_30__)
                            GL_RED,
#else
                            GL_ALPHA,
#endif
                            GL_UNSIGNED_BYTE, data);
        }
        delete[] data;
    }
}

