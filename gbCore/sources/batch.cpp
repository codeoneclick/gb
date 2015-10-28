//
//  batch.cpp
//  gbCore
//
//  Created by sergey.sergeev on 10/28/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#include "batch.h"
#include "mesh.h"
#include "vbo.h"
#include "ibo.h"
#include "glm_extensions.h"

namespace gb
{
    const ui32 batch::k_max_num_vertices = 65535 / 4; // 16k vertices
    const ui32 batch::k_max_num_indices = 65535 / 2;  // 32k indices
    
    batch::batch() :
    m_mesh(nullptr),
    m_num_vertices(0),
    m_num_indices(0),
    m_is_processed(false)
    {
        vbo_shared_ptr vbo = std::make_shared<gb::vbo>(k_max_num_vertices, GL_DYNAMIC_DRAW);
        vbo::vertex_attribute* vertices = vbo->lock();
        memset(vertices, 0x0, k_max_num_vertices * sizeof(vbo::vertex_attribute));
        vbo->unlock();
        
        ibo_shared_ptr ibo = std::make_shared<gb::ibo>(k_max_num_indices, GL_DYNAMIC_DRAW);
        ui16* indices = ibo->lock();
        memset(indices, 0x0, k_max_num_indices * sizeof(ui16));
        ibo->unlock();
        
        m_mesh = mesh::construct("batch", vbo, ibo, glm::vec3(INT16_MIN), glm::vec3(INT16_MAX));
    }
    
    batch::~batch()
    {
        
    }
    
    mesh_shared_ptr batch::get_mesh() const
    {
        return m_mesh;
    }
    
    bool batch::get_is_processed() const
    {
        return m_is_processed;
    }
    
    void batch::set_is_processed(bool value)
    {
        m_is_processed = value;
    }
    
    void batch::add_data(const mesh_shared_ptr& mesh, const glm::mat4& mat_m)
    {
        m_data.push_back(std::make_pair(mesh, mat_m));
    }
    
    void batch::cleanup()
    {
        m_num_vertices = 0;
        m_num_indices = 0;
        
        m_is_processed = false;
        
        m_data.clear();
    }
    
    void batch::generate()
    {
        for(const auto& data_iterator : m_data)
        {
            mesh_shared_ptr mesh = data_iterator.first;
            glm::mat4 m_mat = data_iterator.second;
            
            ui16* main_indices = m_mesh->get_ibo()->lock();
            ui16* sub_indices = mesh->get_ibo()->lock();
            
            for(ui32 i = 0; i < mesh->get_ibo()->get_used_size(); ++i)
            {
                main_indices[m_num_indices + i] = sub_indices[i] + m_num_vertices;
            }
            
            vbo::vertex_attribute* main_vertices = m_mesh->get_vbo()->lock();
            vbo::vertex_attribute* sub_vertices = mesh->get_vbo()->lock();
            
            glm::vec3 position = glm::vec3(0.f);
            glm::vec4 normal = glm::vec4(0.f);
            glm::vec4 tangent = glm::vec4(0.f);
            
            for(ui32 i = 0; i < mesh->get_vbo()->get_used_size(); ++i)
            {
                position = sub_vertices[i].m_position;
                normal = glm::unpackSnorm4x8(sub_vertices[i].m_normal);
                tangent = glm::unpackSnorm4x8(sub_vertices[i].m_tangent);
                
                main_vertices[m_num_vertices + i] = sub_vertices[i];
                main_vertices[m_num_vertices + i].m_position = glm::transform(position, m_mat);
                main_vertices[m_num_vertices + i].m_normal = glm::packSnorm4x8(glm::transform(normal, m_mat));
                main_vertices[m_num_vertices + i].m_tangent = glm::packSnorm4x8(glm::transform(tangent, m_mat));
            }
            
            m_num_vertices += mesh->get_vbo()->get_used_size();
            m_num_indices += mesh->get_ibo()->get_used_size();
        }
        m_mesh->get_vbo()->unlock(m_num_vertices);
        m_mesh->get_ibo()->unlock(m_num_indices);
    }
}