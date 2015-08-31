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
}