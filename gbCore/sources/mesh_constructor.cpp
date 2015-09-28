//
//  mesh_constructor.cpp
//  gbCore
//
//  Created by sergey.sergeev on 8/13/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "mesh_constructor.h"
#include "mesh.h"
#include "vbo.h"
#include "ibo.h"

namespace gb
{
    mesh_shared_ptr mesh_constructor::create_wireframe_box(const glm::vec3& min_bound,
                                                           const glm::vec3& max_bound)
    {
        vbo_shared_ptr vbo = std::make_shared<gb::vbo>(8, GL_STATIC_DRAW);
        vbo::vertex_attribute* vertices = vbo->lock();
        
        glm::vec3 middle = (min_bound + max_bound) * .5f;
        glm::vec3 min_coords = middle + (min_bound - middle) * 1.01f;
        glm::vec3 max_coords = middle + (max_bound - middle) * 1.01f;
        
        vertices[0].m_position = min_coords;
        vertices[1].m_position = glm::vec3(max_coords.x, min_coords.y, min_coords.z);
        vertices[2].m_position = glm::vec3(max_coords.x, max_coords.y, min_coords.z);
        vertices[3].m_position = glm::vec3(min_coords.x, max_coords.y, min_coords.z);
        
        vertices[4].m_position = glm::vec3(min_coords.x, min_coords.y, max_coords.z);
        vertices[5].m_position = glm::vec3(max_coords.x, min_coords.y, max_coords.z);
        vertices[6].m_position = max_coords;
        vertices[7].m_position = glm::vec3(min_coords.x, max_coords.y, max_coords.z);
        
        vbo->unlock();
        
        GLushort wireframe_indices[] =
        {
            0, 1, 1, 2,	2, 3, 3, 0,
            4, 5, 5, 6, 6, 7, 7, 4,
            0, 4, 1, 5, 2, 6, 3, 7
        };
        
        ibo_shared_ptr ibo = std::make_shared<gb::ibo>(24, GL_STATIC_DRAW);
        ui16* indices = ibo->lock();
        memcpy(indices, wireframe_indices, sizeof(wireframe_indices));
        ibo->unlock();
        
        mesh_shared_ptr mesh = gb::mesh::construct("wireframe_box",
                                                   vbo, ibo,
                                                   min_bound, max_bound,
                                                   GL_LINES);
        return mesh;
    }
    
    mesh_shared_ptr mesh_constructor::create_plane(const glm::vec2& size)
    {
        vbo_shared_ptr vbo = std::make_shared<gb::vbo>(4, GL_STATIC_DRAW);
        vbo::vertex_attribute* vertices = vbo->lock();
        
        vertices[0].m_position = glm::vec3(0.f);
        vertices[1].m_position = glm::vec3(size.x, 0.f, 0.f);
        vertices[2].m_position = glm::vec3(size.x, 0.f, size.y);
        vertices[3].m_position = glm::vec3(0.f, 0.f, size.y);
        
        vertices[0].m_normal = glm::packSnorm4x8(glm::vec4(0.f, 1.f, 0.f, 0.f));
        vertices[1].m_normal = glm::packSnorm4x8(glm::vec4(0.f, 1.f, 0.f, 0.f));
        vertices[2].m_normal = glm::packSnorm4x8(glm::vec4(0.f, 1.f, 0.f, 0.f));
        vertices[3].m_normal = glm::packSnorm4x8(glm::vec4(0.f, 1.f, 0.f, 0.f));
        
        vertices[0].m_texcoord = glm::packUnorm2x16(glm::vec2(0.f,  0.f));
        vertices[1].m_texcoord = glm::packUnorm2x16(glm::vec2(1.f,  0.f));
        vertices[2].m_texcoord = glm::packUnorm2x16(glm::vec2(1.f,  1.f));
        vertices[3].m_texcoord = glm::packUnorm2x16(glm::vec2(0.f,  1.f));
        
        vbo->unlock();
        
        ibo_shared_ptr ibo = std::make_shared<gb::ibo>(6, GL_STATIC_DRAW);
        ui16* indices = ibo->lock();
        
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 0;
        indices[4] = 2;
        indices[5] = 3;
        
        ibo->unlock();
        
        mesh_shared_ptr mesh = gb::mesh::construct("plane", vbo, ibo,
                                                   glm::vec3(0.f), glm::vec3(size.x, 0.f, size.y));
        return mesh;
    }
    
    mesh_shared_ptr mesh_constructor::create_sphere(f32 radius, i32 rings, i32 sectors)
    {
        vbo_shared_ptr vbo = std::make_shared<gb::vbo>((rings + 1) * (sectors + 1), GL_STATIC_DRAW);
        vbo::vertex_attribute* vertices = vbo->lock();
        
        ibo_shared_ptr ibo = std::make_shared<gb::ibo>((rings * sectors + rings) * 6, GL_STATIC_DRAW);
        ui16* indices = ibo->lock();
        
        i32 index = 0;
        for(i32 i = 0; i <= sectors; ++i)
        {
            f32 v = i / static_cast<f32>(sectors);
            f32 phi = v * M_PI;
            
            for (i32 j = 0; j <= rings; ++j )
            {
                f32 u = j / static_cast<f32>(rings);
                f32 theta = u * M_PI * 2.f;
                
                f32 x = cos(theta) * sin(phi);
                f32 y = cos(phi);
                f32 z = sin(theta) * sin(phi);
                
                vertices[index].m_position = glm::vec3(x, y, z) * radius;
                vertices[index].m_normal = glm::packSnorm4x8(glm::vec4(x, y, z, 0.f));
                vertices[index].m_texcoord =  glm::packUnorm2x16(glm::vec2(u, v));
                
                ++index;
            }
        }
        
        index = 0;
        for(i32 i = 0; i < rings * sectors + rings; ++i)
        {
            indices[index++] = i;
            indices[index++] = i + rings + 1;
            indices[index++] = i + rings;
            
            indices[index++] = i + rings + 1;
            indices[index++] = i;
            indices[index++] = i + 1;
        }
        
        vbo->unlock();
        ibo->unlock();
        mesh_shared_ptr mesh = gb::mesh::construct("sphere", vbo, ibo,
                                                   glm::vec3(-radius / 2.f), glm::vec3(radius / 2.f));
        return mesh;
    }
    
    mesh_shared_ptr mesh_constructor::create_screen_quad()
    {
        vbo_shared_ptr vbo = std::make_shared<gb::vbo>(4, GL_STATIC_DRAW);
        vbo::vertex_attribute * vertices = vbo->lock();
        
        vertices[0].m_position = glm::vec3(-1.0f, -1.0f, 0.0f);
        vertices[0].m_texcoord = glm::packUnorm2x16(glm::vec2(0.0f, 0.0f));
        vertices[1].m_position = glm::vec3(-1.0f, 1.0f, 0.0f);
        vertices[1].m_texcoord = glm::packUnorm2x16(glm::vec2(0.0f, 1.0f));
        vertices[2].m_position = glm::vec3(1.0f, -1.0f, 0.0f);
        vertices[2].m_texcoord = glm::packUnorm2x16(glm::vec2(1.0f, 0.0f));
        vertices[3].m_position = glm::vec3(1.0f, 1.0f, 0.0f);
        vertices[3].m_texcoord = glm::packUnorm2x16(glm::vec2(1.0f, 1.0f));
        vbo->unlock();
        
        ibo_shared_ptr ibo = std::make_shared<gb::ibo>(6, GL_STATIC_DRAW);
        ui16* indices = ibo->lock();
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 1;
        indices[4] = 2;
        indices[5] = 3;
        ibo->unlock();
        
        mesh_shared_ptr mesh = gb::mesh::construct("screen_quad", vbo, ibo,
                                                   glm::vec3(-4096.f),
                                                   glm::vec3( 4096.f));
        return mesh;
    }
}