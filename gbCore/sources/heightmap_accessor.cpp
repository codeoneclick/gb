//
//  CHeightmapAccessor.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "heightmap_accessor.h"
#include "thread_operation.h"
#include "heightmap_loader.h"
#include "heightmap_container.h"
#include "heightmap_texture_generator.h"
#include "heightmap_geometry_generator.h"
#include "vbo.h"
#include "ibo.h"
#include "mesh.h"
#include "texture.h"

namespace gb
{
    
#define k_splatting_textures_cache_size 4
    
    heightmap_accessor::heightmap_accessor() :
    m_container(std::make_shared<heightmap_container>()),
    m_isGenerated(false),
    m_graphics_context(nullptr)
    {
        for(ui32 i = 0; i < heightmap_texture_generator::e_splatting_texture_max; ++i)
        {
            m_diffuse_textures[i] = nullptr;
            m_normal_textures[i] = nullptr;
            m_displace_textures[i] = nullptr;
        }
    }
    
    heightmap_accessor::~heightmap_accessor()
    {
        for(ui32 i = 0; i < heightmap_texture_generator::e_splatting_texture_max; ++i)
        {
            m_diffuse_textures[i] = nullptr;
            m_normal_textures[i] = nullptr;
            m_displace_textures[i] = nullptr;
        }
    }
    
    void heightmap_accessor::create_loading_operations()
    {
        m_callbacks.clear();
        m_callbacks.resize(m_container->get_chunks_num().x * m_container->get_chunks_num().y,
                           std::make_tuple(nullptr, nullptr));
        
        m_executed_operations.clear();
        m_executed_operations.resize(m_container->get_chunks_num().x * m_container->get_chunks_num().y, nullptr);
    }
    
    void heightmap_accessor::erase_loading_operations()
    {
        m_callbacks.clear();
        m_executed_operations.clear();
    }
    
    void heightmap_accessor::create_bounding_boxes()
    {
        m_chunks_bounds.clear();
        m_chunks_bounds.resize(m_container->get_chunks_num().x * m_container->get_chunks_num().y,
                               std::make_tuple(glm::vec3(INT16_MAX),
                                               glm::vec3(INT16_MIN)));
        
        for(ui32 i = 0; i < m_container->get_chunks_num().x; ++i)
        {
            for(ui32 j = 0; j < m_container->get_chunks_num().y; ++j)
            {
                heightmap_accessor::create_bounding_box(i, j);
            }
        }
    }
    
    void heightmap_accessor::create_bounding_box(ui32 i, ui32 j)
    {
        ui32 index = i + j * m_container->get_chunks_num().x;
        glm::vec3 min_bound = glm::vec3(INT16_MAX);
        glm::vec3 max_bound = glm::vec3(INT16_MIN);
        
        for(ui32 x = 0; x < m_container->get_chunk_size().x; ++x)
        {
            for(ui32 y = 0; y < m_container->get_chunk_size().y; ++y)
            {
                glm::ivec2 position = glm::ivec2(x + i * m_container->get_chunk_size().x - i,
                                                 y + j * m_container->get_chunk_size().y - j);
                
                position.x = position.x < m_container->get_main_size().x ? position.x : m_container->get_main_size().x - 1;
                position.y = position.y < m_container->get_main_size().y ? position.y : m_container->get_main_size().y - 1;
                
                glm::vec3 point = m_container->get_vertex_position(position.x, position.y);
                min_bound = glm::min(point, min_bound);
                max_bound = glm::max(point, max_bound);
            }
        }
        m_chunks_bounds[index] = std::make_tuple(min_bound, max_bound);
    }
    
    void heightmap_accessor::erase_bounding_boxes()
    {
        m_chunks_bounds.clear();
    }
    
    void heightmap_accessor::create_data_containers()
    {
        m_chunks_data.clear();
        m_chunks_data.resize(m_container->get_chunks_num().x * m_container->get_chunks_num().y,
                             std::make_tuple(nullptr, nullptr, nullptr, heightmap_container::e_heigtmap_chunk_lod_unknown, std::make_shared<std::mutex>()));
        
        for(ui32 i = 0; i < heightmap_container::e_heigtmap_chunk_lod_max; ++i)
        {
            while(!m_splatting_diffuse_textures_cache[i].empty())
            {
                m_splatting_diffuse_textures_cache[i].pop();
            }
            
            while(!m_splatting_normal_textures_cache[i].empty())
            {
                m_splatting_normal_textures_cache[i].pop();
            }
            
            while(!m_splatting_displace_textures_cache[i].empty())
            {
                m_splatting_displace_textures_cache[i].pop();
            }
        }
    }
    
    void heightmap_accessor::erase_data_containers()
    {
        m_chunks_data.clear();
    }
    
    void heightmap_accessor::erase_chunk_data(i32 index)
    {
        assert(std::get<4>(m_chunks_data[index]));
        std::lock_guard<std::mutex> guard(*std::get<4>(m_chunks_data[index]).get());
        
        if(m_splatting_diffuse_textures_cache[std::get<3>(m_chunks_data[index])].size() < k_splatting_textures_cache_size &&
           std::get<1>(m_chunks_data[index]))
        {
            m_splatting_diffuse_textures_cache[std::get<3>(m_chunks_data[index])].push(std::get<1>(m_chunks_data[index]));
        }
        
        if(m_splatting_normal_textures_cache[std::get<3>(m_chunks_data[index])].size() < k_splatting_textures_cache_size &&
           std::get<2>(m_chunks_data[index]))
        {
            m_splatting_normal_textures_cache[std::get<3>(m_chunks_data[index])].push(std::get<2>(m_chunks_data[index]));
        }
        
        std::get<0>(m_chunks_data[index]) = nullptr;
        std::get<1>(m_chunks_data[index]) = nullptr;
        std::get<2>(m_chunks_data[index]) = nullptr;
        std::get<3>(m_chunks_data[index]) = heightmap_container::e_heigtmap_chunk_lod_unknown;
        
        std::get<0>(m_callbacks[index]) = nullptr;
        std::get<1>(m_callbacks[index]) = nullptr;
        
        m_executed_operations[index] = nullptr;
    }
    
    void heightmap_accessor::generate(const std::string& filename, const graphics_context_shared_ptr& graphics_context,
                                      const std::array<texture_shared_ptr, 3>& diffuse_textures,
                                      const std::array<texture_shared_ptr, 3>& normal_textures,
                                      const std::array<texture_shared_ptr, 3>& displace_textures,
                                      const std::function<void()>& callback)
    {
        m_isGenerated = false;
       
        for(ui32 i = 0; i < heightmap_texture_generator::e_splatting_texture_max; ++i)
        {
            m_diffuse_textures[i] = diffuse_textures[i];
            m_normal_textures[i] = normal_textures[i];
            m_displace_textures[i] = displace_textures[i];
        }
        m_graphics_context = graphics_context;
        
        thread_operation_shared_ptr completion_operation = std::make_shared<thread_operation>(thread_operation::e_thread_operation_queue_main);
        completion_operation->set_execution_callback([this, callback]() {
            m_isGenerated = true;
            callback();
        });
        
        thread_operation_shared_ptr generate_geometry_operation = std::make_shared<thread_operation>(thread_operation::e_thread_operation_queue_main);
        generate_geometry_operation->set_execution_callback([this, filename]() {
            
            std::tuple<glm::ivec2, std::vector<f32>> heights_data = heightmap_loader::get_heights_data(filename);;
            
            m_container->init(std::get<0>(heights_data));
            
            heightmap_geometry_generator::generate(m_container, filename, std::get<0>(heights_data), std::get<1>(heights_data));
            
            heightmap_accessor::create_loading_operations();
            heightmap_accessor::create_data_containers();
        });
        completion_operation->add_dependency(generate_geometry_operation);
        
        thread_operation_shared_ptr mmap_geometry_operation = std::make_shared<thread_operation>(thread_operation::e_thread_operation_queue_main);
        mmap_geometry_operation->set_execution_callback([this, filename]() {
            
            m_container->mmap_geometry(filename);
            
            heightmap_geometry_generator::generate_tangent_space(m_container, filename);
            heightmap_geometry_generator::generate_attaches_to_vbo(m_container, filename);
            
            heightmap_accessor::create_bounding_boxes();
        });
        completion_operation->add_dependency(mmap_geometry_operation);
        
        thread_operation_shared_ptr generate_heightmap_texture_operation = std::make_shared<thread_operation>(thread_operation::e_thread_operation_queue_main);
        generate_heightmap_texture_operation->set_execution_callback([this, filename]() {
            
            m_container->create_deep_texture();
            heightmap_texture_generator::generate_deep_texture(m_container, true, 0, 0, 0, 0);
        });
        completion_operation->add_dependency(generate_heightmap_texture_operation);
        
        thread_operation_shared_ptr generate_splatting_masks_operation = std::make_shared<thread_operation>(thread_operation::e_thread_operation_queue_main);
        generate_splatting_masks_operation->set_execution_callback([this, filename]() {
            
            heightmap_texture_generator::splatting_masks(m_container, filename);
        });
        completion_operation->add_dependency(generate_splatting_masks_operation);
        
        thread_operation_shared_ptr mmap_masks_operation = std::make_shared<thread_operation>(thread_operation::e_thread_operation_queue_main);
        mmap_masks_operation->set_execution_callback([this, filename]() {
            
            m_container->mmap_mask_textures(filename);
        });
        completion_operation->add_dependency(mmap_masks_operation);
        
        thread_operation_shared_ptr generate_splatting_diffuse_textures_operation = std::make_shared<thread_operation>(thread_operation::e_thread_operation_queue_main);
        generate_splatting_diffuse_textures_operation->set_execution_callback([this, filename, graphics_context]() {
            
            heightmap_texture_generator::splatting_diffuse_textures(graphics_context, m_container, filename, m_diffuse_textures);
        });
        completion_operation->add_dependency(generate_splatting_diffuse_textures_operation);
        
        thread_operation_shared_ptr mmap_diffuse_textures_operation = std::make_shared<thread_operation>(thread_operation::e_thread_operation_queue_main);
        mmap_diffuse_textures_operation->set_execution_callback([this, filename]() {
            
            m_container->mmap_diffuse_textures(filename);
        });
        completion_operation->add_dependency(mmap_diffuse_textures_operation);
        
        thread_operation_shared_ptr generate_splatting_normal_textures_operation = std::make_shared<thread_operation>(thread_operation::e_thread_operation_queue_main);
        generate_splatting_normal_textures_operation->set_execution_callback([this, filename, graphics_context]() {
            
            heightmap_texture_generator::splatting_normal_displace_textures(graphics_context, m_container, filename, m_normal_textures, m_displace_textures);
        });
        completion_operation->add_dependency(generate_splatting_normal_textures_operation);
        
        thread_operation_shared_ptr mmap_normal_textures_operation = std::make_shared<thread_operation>(thread_operation::e_thread_operation_queue_main);
        mmap_normal_textures_operation->set_execution_callback([this, filename]() {
            
            m_container->mmap_normal_textures(filename);
        });
        completion_operation->add_dependency(mmap_normal_textures_operation);
        
        completion_operation->add_to_execution_queue();
    }
    
    void heightmap_accessor::generate_mesh(i32 index, heightmap_container::e_heigtmap_chunk_lod lod)
    {
        vbo_shared_ptr vbo = std::make_shared<gb::vbo>(m_container->get_vbo_mmap(index)->get_size(),
                                                       GL_STATIC_DRAW,
                                                       m_container->get_vbo_mmap(index)->get_pointer());
        vbo->unlock();
        
        m_container->get_ibo_mmap(index, lod)->update_source_pointer();
        
        ibo_shared_ptr ibo = std::make_shared<gb::ibo>(m_container->get_ibo_mmap(index, lod)->get_size(),
                                                       GL_DYNAMIC_DRAW,
                                                       m_container->get_ibo_mmap(index, lod)->get_source_pointer());
        ibo->unlock();
        
        std::ostringstream stringstream;
        stringstream<<"chunk_"<<index<<"_"<<lod<<std::endl;
        mesh_shared_ptr mesh = mesh::construct(stringstream.str(), vbo, ibo,
                                               std::get<0>(m_chunks_bounds[index]), std::get<1>(m_chunks_bounds[index]));
        std::get<0>(m_chunks_data[index]) = mesh;
    }
    
    void heightmap_accessor::generate_splatting_textures(i32 index, heightmap_container::e_heigtmap_chunk_lod lod)
    {
        {
            texture_shared_ptr diffuse_texture = nullptr;
            if(!m_splatting_diffuse_textures_cache[lod].empty())
            {
                diffuse_texture = m_splatting_diffuse_textures_cache[lod].front();
                assert(diffuse_texture);
                m_splatting_diffuse_textures_cache[lod].pop();
            }
            else
            {
                std::ostringstream stringstream;
                stringstream<<"diffuse_texture_"<<index<<std::endl;
                
                ui32 diffuse_texture_id;
                gl_create_textures(1, &diffuse_texture_id);
                
                diffuse_texture = texture::construct(stringstream.str(), diffuse_texture_id,
                                                     m_container->get_textures_lod_size(lod).x,
                                                     m_container->get_textures_lod_size(lod).y);
                
                diffuse_texture->set_wrap_mode(GL_CLAMP_TO_EDGE);
                diffuse_texture->set_mag_filter(GL_LINEAR);
                diffuse_texture->set_min_filter(GL_LINEAR_MIPMAP_NEAREST);
            }
            
            diffuse_texture->bind();
            
            gl_texture_image2d(GL_TEXTURE_2D, 0, GL_RGB,
                               m_container->get_textures_lod_size(lod).x, m_container->get_textures_lod_size(lod).y,
                               0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, m_container->get_splatting_diffuse_textures_mmap(index, lod)->get_pointer());
            gl_generate_mipmap(GL_TEXTURE_2D);
            
            std::get<1>(m_chunks_data[index]) = diffuse_texture;
        }
        {
            texture_shared_ptr normal_texture = nullptr;
            if(!m_splatting_normal_textures_cache[lod].empty())
            {
                normal_texture = m_splatting_normal_textures_cache[lod].front();
                assert(normal_texture);
                m_splatting_normal_textures_cache[lod].pop();
            }
            else
            {
                std::ostringstream stringstream;
                stringstream<<"normal_texture_"<<index<<std::endl;
                
                ui32 normal_texture_id;
                gl_create_textures(1, &normal_texture_id);
                
                normal_texture = texture::construct(stringstream.str(), normal_texture_id,
                                                    m_container->get_textures_lod_size(lod).x,
                                                    m_container->get_textures_lod_size(lod).y);
                normal_texture->set_wrap_mode(GL_CLAMP_TO_EDGE);
                normal_texture->set_mag_filter(GL_LINEAR);
                normal_texture->set_min_filter(GL_LINEAR_MIPMAP_NEAREST);
            }
            
            normal_texture->bind();
            
            gl_texture_image2d(GL_TEXTURE_2D, 0, GL_RGBA,
                         m_container->get_textures_lod_size(lod).x, m_container->get_textures_lod_size(lod).y,
                         0, GL_RGBA, GL_UNSIGNED_BYTE, m_container->get_splatting_normal_textures_mmap(index, lod)->get_pointer());
            gl_generate_mipmap(GL_TEXTURE_2D);
            
            std::get<2>(m_chunks_data[index]) = normal_texture;
        }
    }
    
    void heightmap_accessor::update_splatting_textures(i32 index)
    {
        heightmap_container::e_heigtmap_chunk_lod lod = std::get<3>(m_chunks_data[index]);
        
        {
            texture_shared_ptr diffuse_texture = std::get<1>(m_chunks_data[index]);
            if(diffuse_texture)
            {
                
                diffuse_texture->bind();
                gl_texture_image2d(GL_TEXTURE_2D, 0, GL_RGBA,
                                   m_container->get_textures_lod_size(lod).x, m_container->get_textures_lod_size(lod).y,
                                   0, GL_RGBA, GL_UNSIGNED_BYTE, m_container->get_splatting_diffuse_textures_mmap(index, lod)->get_pointer());
                gl_generate_mipmap(GL_TEXTURE_2D);
            }
        }
        {
            texture_shared_ptr normal_texture = std::get<2>(m_chunks_data[index]);
            if(normal_texture)
            {
                normal_texture->bind();
                gl_texture_image2d(GL_TEXTURE_2D, 0, GL_RGBA,
                                   m_container->get_textures_lod_size(lod).x, m_container->get_textures_lod_size(lod).y,
                                   0, GL_RGBA, GL_UNSIGNED_BYTE, m_container->get_splatting_normal_textures_mmap(index, lod)->get_pointer());
                gl_generate_mipmap(GL_TEXTURE_2D);
            }
        }
    }
    
    void heightmap_accessor::start_chunk_loading(i32 i, i32 j, heightmap_container::e_heigtmap_chunk_lod lod,
                                        const std::function<void(const mesh_shared_ptr&)>& callback_mesh_loaded,
                                        const std::function<void(const texture_shared_ptr&, const texture_shared_ptr&)>& callback_textures_loaded)
    {
        ui32 index = i + j * m_container->get_chunks_num().x;
        if(m_executed_operations[index] != nullptr || !m_isGenerated)
        {
            return;
        }
        
        std::get<0>(m_callbacks[index]) = callback_mesh_loaded;
        std::get<1>(m_callbacks[index]) = callback_textures_loaded;
        
        if(std::get<0>(m_chunks_data[index]) != nullptr)
        {
            std::get<0>(m_chunks_data[index]) = nullptr;
        }
        if(std::get<1>(m_chunks_data[index]) != nullptr)
        {
            std::get<1>(m_chunks_data[index]) = nullptr;
        }
        if(std::get<2>(m_chunks_data[index]) != nullptr)
        {
            std::get<2>(m_chunks_data[index]) = nullptr;
        }
        std::get<3>(m_chunks_data[index]) = lod;
        
        assert(std::get<0>(m_callbacks[index]) != nullptr);
        assert(std::get<1>(m_callbacks[index]) != nullptr);
        
        thread_operation_shared_ptr completion_operation = std::make_shared<thread_operation>(thread_operation::e_thread_operation_queue_main);
        completion_operation->set_execution_callback([this, index]() {
            m_executed_operations[index] = nullptr;
        });
        
        thread_operation_shared_ptr create_mesh_operation = std::make_shared<thread_operation>(thread_operation::e_thread_operation_queue_main);
        create_mesh_operation->set_execution_callback([this, index, lod]() {
            
            heightmap_accessor::generate_mesh(index, lod);
            assert(std::get<0>(m_callbacks[index]));
            std::get<0>(m_callbacks[index])(std::get<0>(m_chunks_data[index]));
        });
        completion_operation->add_dependency(create_mesh_operation);
        
        thread_operation_shared_ptr generate_splatting_textures_operation = std::make_shared<thread_operation>(thread_operation::e_thread_operation_queue_main);
        generate_splatting_textures_operation->set_execution_callback([this, index, lod]() {
            heightmap_accessor::generate_splatting_textures(index, lod);
            assert(std::get<1>(m_callbacks[index]));
            std::get<1>(m_callbacks[index])(std::get<1>(m_chunks_data[index]), std::get<2>(m_chunks_data[index]));
        });
        completion_operation->add_dependency(generate_splatting_textures_operation);

        assert(m_executed_operations[index] == nullptr);
        m_executed_operations[index] = completion_operation;
        
        completion_operation->set_cancel_callback([this, index]() {
            assert(m_executed_operations[index] != nullptr);
            heightmap_accessor::erase_chunk_data(index);
        });
        completion_operation->add_to_execution_queue();
    }
    
    void heightmap_accessor::start_chunk_unloading(i32 i, i32 j)
    {
        ui32 index = i + j * m_container->get_chunks_num().x;
        
        if(m_executed_operations[index])
        {
            m_executed_operations[index]->cancel();
        }
        else
        {
            heightmap_accessor::erase_chunk_data(index);
        }
    }
    
    f32 heightmap_accessor::get_angle(const glm::vec3& point_01,
                                      const glm::vec3& point_02,
                                      const glm::vec3& point_03)
    {
        f32 vector_length_01 = sqrt(pow(point_02.x - point_01.x, 2) + pow(point_02.y - point_01.y, 2) + pow(point_02.z - point_01.z, 2));
        f32 vector_length_02 = sqrt(pow(point_03.x - point_01.x, 2) + pow(point_03.y - point_01.y, 2) + pow(point_03.z - point_01.z, 2));
        
        f32 scalar = (point_02.x - point_01.x) * (point_03.x - point_01.x) +
        (point_02.y - point_01.y) * (point_03.y - point_01.y) +
        (point_02.z - point_01.z) * (point_03.z - point_01.z);
        return scalar / (vector_length_01 * vector_length_02);
    }
    
    f32 heightmap_accessor::get_height(const glm::vec3& position) const
    {
        return heightmap_accessor::get_height(m_container, position);
    }
    
    glm::vec3 heightmap_accessor::get_normal(const glm::vec3& position) const
    {
        return heightmap_accessor::get_normal(m_container, position);
    }
    
    glm::vec2 heightmap_accessor::get_angles_xz(const glm::vec3& position) const
    {
        return heightmap_accessor::get_angles_xz(m_container, position);
    }
    
    f32 heightmap_accessor::get_height(std::shared_ptr<heightmap_container> container, const glm::vec3& position)
    {
        f32 _x = position.x / 1.f;
        f32 _z = position.z / 1.f;
        i32 x = static_cast<i32>(floor(_x));
        i32 z = static_cast<i32>(floor(_z));
        f32 dx = _x - x;
        f32 dy = _z - z;
        
        if((x < 0) || (z < 0) || (x > (container->get_main_size().x - 1)) || (z > (container->get_main_size().y - 1)))
        {
            return 0.f;
        }
        
        f32 height_00 = container->get_vertex_position(x, z).y;
        f32 height_01 = container->get_vertex_position(x, z).y;
        if(z < (container->get_main_size().y - 1) && z >= 0)
        {
            height_01 = container->get_vertex_position(x, z + 1).y;
        }
        
        f32 height_10 = container->get_vertex_position(x, z).y;
        if(x < (container->get_main_size().x - 1) && x >= 0)
        {
            height_10 = container->get_vertex_position(x + 1, z).y;
        }
        
        f32 height_11 = container->get_vertex_position(x, z).y;
        if(z < (container->get_main_size().y - 1) && z >= 0 && x < (container->get_main_size().x - 1) && x >= 0)
        {
            height_11 = container->get_vertex_position(x + 1, z + 1).y;
        }
        
        f32 height_0 = height_00 * (1.0f - dy) + height_01 * dy;
        f32 height_1 = height_10 * (1.0f - dy) + height_11 * dy;
        return height_0 * (1.0f - dx) + height_1 * dx;
    }
    
    glm::vec3 heightmap_accessor::get_normal(std::shared_ptr<heightmap_container> container, const glm::vec3& position)
    {
        f32 _x = position.x / 1.f;
        f32 _z = position.z / 1.f;
        i32 x = static_cast<i32>(floor(_x));
        i32 z = static_cast<i32>(floor(_z));
        
        if((x < 0) || (z < 0) || (x > (container->get_main_size().x - 1)) || (z > (container->get_main_size().y - 1)))
        {
            return glm::vec3(0.f, 1.f, 0.f);
        }
        
        glm::vec3 normal_00 = container->get_uncompressed_vertex_normal(x, z);
        
        glm::vec3 normal_01 = container->get_uncompressed_vertex_normal(x, z);
        if(z < (container->get_main_size().y - 1) && z >= 0)
        {
            normal_01 = container->get_uncompressed_vertex_normal(x, z + 1);
        }
        
        glm::vec3 normal_10 = container->get_uncompressed_vertex_normal(x, z);
        if(x < (container->get_main_size().x - 1) && x >= 0)
        {
            normal_10 = container->get_uncompressed_vertex_normal(x + 1, z);
        }
        
        glm::vec3 normal_11 = container->get_uncompressed_vertex_normal(x, z);
        if(z < (container->get_main_size().y - 1) && z >= 0 && x < (container->get_main_size().x - 1) && x >= 0)
        {
            normal_11 = container->get_uncompressed_vertex_normal(x + 1, z + 1);
        }
        
        glm::vec3 normal = normal_00 + normal_01 + normal_10 + normal_11 / 4.f;
        return glm::normalize(glm::vec3(normal.x, normal.y, normal.z));
    }
    
    glm::vec2 heightmap_accessor::get_angles_xz(std::shared_ptr<heightmap_container> container, const glm::vec3& position)
    {
        f32 offset = .25f;
        glm::vec3 point_01 = position;
        glm::vec3 point_02 = glm::vec3(position.x, position.y + offset, position.z);
        f32 height =  heightmap_accessor::get_height(container, glm::vec3(position.x + offset, 0.0f, position.z));
        glm::vec3 point_03 = glm::vec3(position.x + offset, height, position.z);
        height = heightmap_accessor::get_height(container, glm::vec3(position.x, 0.0f, position.z + offset));
        glm::vec3 point_04 = glm::vec3(position.x, height, position.z + offset);
        
        f32 angle_01 = heightmap_accessor::get_angle(point_01, point_02, point_03);
        f32 angle_02 = heightmap_accessor::get_angle(point_01, point_02, point_04);
        
        return glm::vec2(glm::degrees(acos(angle_02) - M_PI_2), glm::degrees(asin(angle_01)));
    }
    
    /*void heightmap_accessor::update(void)
    {
        if(!m_updateHeightmapOperations.empty())
        {
            CSharedThreadOperation operation = m_updateHeightmapOperations.front();
            if(!operation->isExecuted() && !operation->isCompleted() && !operation->isCanceled())
            {
                operation->addToExecutionQueue();
            }
            else if(operation->isCompleted() || operation->isCanceled())
            {
                m_updateHeightmapOperations.pop();
            }
        }
    }
    
    void heightmap_accessor::updateVertices(const std::vector<glm::vec3>& vertices,
                                            const glm::ivec2& minBound, const glm::ivec2& maxBound)
    {
        CHeightmapGeometryGenerator::updateVertices(m_container, vertices);
        
        std::set<std::shared_ptr<CVertexBuffer>> updatedVBOs;
        for(ui32 i = 0; i < vertices.size(); ++i)
        {
            i32 x = static_cast<i32>(vertices.at(i).x);
            i32 z = static_cast<i32>(vertices.at(i).z);
            
            ui8 containsInVBOSize = 0;
            glm::ivec2 *containsInVBO = m_container->attachedVerticesToVBO(x, z, &containsInVBOSize);
            assert(containsInVBO != nullptr);
            
            for(ui32 j = 0; j < containsInVBOSize; ++j)
            {
                if(std::get<0>(m_chunksMetadata[containsInVBO[j].x]) != nullptr)
                {
                    std::get<0>(m_chunksMetadata[containsInVBO[j].x])->getVertexBuffer()->lock()[containsInVBO[j].y].m_position =
                    m_container->getVertexPosition(x, z);
                    std::get<0>(m_chunksMetadata[containsInVBO[j].x])->getVertexBuffer()->lock()[containsInVBO[j].y].m_normal =
                    m_container->getCompressedVertexNormal(x, z);
                    
                    updatedVBOs.insert(std::get<0>(m_chunksMetadata[containsInVBO[j].x])->getVertexBuffer());
                }
            }
        }
        
        for(auto vbo : updatedVBOs)
        {
            vbo->unlock();
        }
        
        CSharedThreadOperation executedOperation = nullptr;
        if(!m_updateHeightmapOperations.empty() && m_updateHeightmapOperations.front()->isExecuted())
        {
            executedOperation = m_updateHeightmapOperations.front();
        }
        while (!m_updateHeightmapOperations.empty())
        {
            m_updateHeightmapOperations.pop();
        }
        if(executedOperation)
        {
            m_updateHeightmapOperations.push(executedOperation);
        }
        
        for(ui32 i = 0; i < m_container->getChunksNum().x; ++i)
        {
            for(ui32 j = 0; j < m_container->getChunksNum().y; ++j)
            {
                ui32 index = i + j * m_container->getChunksNum().x;
                if(CBoundingBox::isPointInXZ(glm::vec2(minBound.x, minBound.y), std::get<0>(m_chunksBounds[index]), std::get<1>(m_chunksBounds[index])) ||
                   CBoundingBox::isPointInXZ(glm::vec2(maxBound.x, minBound.y), std::get<0>(m_chunksBounds[index]), std::get<1>(m_chunksBounds[index])) ||
                   CBoundingBox::isPointInXZ(glm::vec2(minBound.x, maxBound.y), std::get<0>(m_chunksBounds[index]), std::get<1>(m_chunksBounds[index])) ||
                   CBoundingBox::isPointInXZ(glm::vec2(maxBound.x, maxBound.y), std::get<0>(m_chunksBounds[index]), std::get<1>(m_chunksBounds[index])))
                {
                    CHeightmapAccessor::createBoundingBox(i, j);
                    
                    CSharedThreadOperation updateGeometryOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
                    updateGeometryOperation->setExecutionBlock([this, i , j, index](void) {
                        CHeightmapGeometryGenerator::generateTangentSpace(m_container, index);
                    });
                    m_updateHeightmapOperations.push(updateGeometryOperation);
                    
                    CSharedThreadOperation generateHeightmapTextureOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
                    generateHeightmapTextureOperation->setExecutionBlock([this, minBound, maxBound](void) {
                        CHeightmapTextureGenerator::generateDeepTexture(m_container, false, minBound.x, minBound.y,
                                                                        maxBound.x - minBound.x, maxBound.y - minBound.y);
                    });
                    m_updateHeightmapOperations.push(generateHeightmapTextureOperation);
                    
                    CSharedThreadOperation updateSplattingMTextureOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
                    updateSplattingMTextureOperation->setExecutionBlock([this, i , j](void) {
                        CHeightmapTextureGenerator::generateSplattingMTexture(m_container, i, j);
                    });
                    m_updateHeightmapOperations.push(updateSplattingMTextureOperation);
                    
                    CSharedThreadOperation updateSplattingDTextureOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
                    updateSplattingDTextureOperation->setExecutionBlock([this, i , j](void) {
                        CHeightmapTextureGenerator::generateSplattingDTexture(m_renderTechniqueAccessor, m_container, m_splattingDTextures, i, j);
                    });
                    m_updateHeightmapOperations.push(updateSplattingDTextureOperation);
                    
                    CSharedThreadOperation updateSplattingNTextureOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
                    updateSplattingNTextureOperation->setExecutionBlock([this, i , j](void) {
                        CHeightmapTextureGenerator::generateSplattingNTexture(m_renderTechniqueAccessor, m_container, m_splattingNTextures, i, j);
                    });
                    m_updateHeightmapOperations.push(updateSplattingNTextureOperation);
                    
                    CSharedThreadOperation updateTexturesOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
                    updateTexturesOperation->setExecutionBlock([this, index](void) {
                        CHeightmapAccessor::updateSplattingTextures(index);
                    });
                    m_updateHeightmapOperations.push(updateTexturesOperation);
                }
            }
        }
    }*/
}

