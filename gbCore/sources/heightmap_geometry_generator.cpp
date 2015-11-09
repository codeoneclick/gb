//
//  heightmap_geometry_generator.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "heightmap_geometry_generator.h"
#include "heightmap_container.h"
#include "heightmap_loader.h"
#include "vbo.h"
#include "ibo.h"

namespace gb
{
    
    void heightmap_geometry_generator::generate(const std::shared_ptr<heightmap_container>& container, const std::string& filename,
                                                const glm::ivec2& size, const std::vector<f32>& heights)
    {
        if(!heightmap_loader::is_uncompressed_vertices_mmap_exist(filename) ||
           !heightmap_loader::is_compressed_vertices_mmap_exist(filename) ||
           !heightmap_loader::is_faces_mmap_exist(filename))
        {
            heightmap_geometry_generator::create_vertices_data(container, size, heights, filename);
            heightmap_geometry_generator::create_vbos_data(container, filename);
            heightmap_geometry_generator::create_ibos_data(container, filename);
        }
        else
        {
            if(!heightmap_loader::is_vbos_mmap_exist(filename))
            {
                heightmap_geometry_generator::create_vbos_data(container, filename);
            }
            
            if(!heightmap_loader::is_ibos_mmap_exist(filename))
            {
                heightmap_geometry_generator::create_ibos_data(container, filename);
            }
        }
    }
    
    void heightmap_geometry_generator::create_vertices_data(const std::shared_ptr<heightmap_container>& container, const glm::ivec2& size, const std::vector<f32>& heights,
                                                            const std::string& filename)
    {
        heightmap_container::uncompressed_vertex *uncompressed_vertices = container->get_uncopressed_vertices();
        heightmap_container::compressed_vertex *compressed_vertices = container->get_compressed_vertices();
        heightmap_container::face *faces = container->get_faces();
        
        for(ui32 i = 0; i < size.x; ++i)
        {
            for(ui32 j = 0; j < size.y; ++j)
            {
                uncompressed_vertices[i + j * size.x].m_position = glm::vec3(i, heights[i + j * size.x], j);
                uncompressed_vertices[i + j * size.x].m_texcoord = glm::vec2(static_cast<f32>(i) / static_cast<f32>(size.x),
                                                                             static_cast<f32>(j) / static_cast<f32>(size.y));
            }
        }
        
        ui32 index = 0;
        for(ui32 i = 0; i < (size.x - 1); ++i)
        {
            for(ui32 j = 0; j < (size.y - 1); ++j)
            {
                faces[index].m_indexes[0] = i + j * size.x;
                uncompressed_vertices[faces[index].m_indexes[0]].m_contains_in_face[uncompressed_vertices[faces[index].m_indexes[0]].m_contains_in_face_size++] = index;
                glm::vec3 point_01 = uncompressed_vertices[faces[index].m_indexes[0]].m_position;
                faces[index].m_indexes[1] = i + (j + 1) * size.x;
                uncompressed_vertices[faces[index].m_indexes[1]].m_contains_in_face[uncompressed_vertices[faces[index].m_indexes[1]].m_contains_in_face_size++] = index;
                glm::vec3 point_02 = uncompressed_vertices[faces[index].m_indexes[1]].m_position;
                faces[index].m_indexes[2] = i + 1 + j * size.x;
                uncompressed_vertices[faces[index].m_indexes[2]].m_contains_in_face[uncompressed_vertices[faces[index].m_indexes[2]].m_contains_in_face_size++] = index;
                glm::vec3 point_03 = uncompressed_vertices[faces[index].m_indexes[2]].m_position;
                
                glm::vec3 edge_01 = point_02 - point_01;
                glm::vec3 edge_02 = point_03 - point_01;
                glm::vec3 normal = glm::cross(edge_01, edge_02);
                f32 sin = glm::length(normal) / (glm::length(edge_01) * glm::length(edge_02));
                faces[index].m_normal = glm::normalize(normal) * asinf(sin);
                index++;
                
                faces[index].m_indexes[0] = i + (j + 1) * size.x;
                uncompressed_vertices[faces[index].m_indexes[0]].m_contains_in_face[uncompressed_vertices[faces[index].m_indexes[0]].m_contains_in_face_size++] = index;
                point_01 = uncompressed_vertices[faces[index].m_indexes[0]].m_position;
                faces[index].m_indexes[1] = i + 1 + (j + 1) * size.x;
                uncompressed_vertices[faces[index].m_indexes[1]].m_contains_in_face[uncompressed_vertices[faces[index].m_indexes[1]].m_contains_in_face_size++] = index;
                point_02 = uncompressed_vertices[faces[index].m_indexes[1]].m_position;
                faces[index].m_indexes[2] = i + 1 + j * size.x;
                uncompressed_vertices[faces[index].m_indexes[2]].m_contains_in_face[uncompressed_vertices[faces[index].m_indexes[2]].m_contains_in_face_size++] = index;
                point_03 = uncompressed_vertices[faces[index].m_indexes[2]].m_position;
                
                edge_01 = point_02 - point_01;
                edge_02 = point_03 - point_01;
                normal = glm::cross(edge_01, edge_02);
                sin = glm::length(normal) / (glm::length(edge_01) * glm::length(edge_02));
                faces[index].m_normal = glm::normalize(normal) * asinf(sin);
                index++;
            }
        }
        
        for(ui32 i = 0; i < size.x * size.y; ++i)
        {
            assert(uncompressed_vertices[i].m_contains_in_face_size != 0 && uncompressed_vertices[i].m_contains_in_face_size <= heightmap_container::k_max_contains_in_face);
            glm::vec3 normal = faces[uncompressed_vertices[i].m_contains_in_face[0]].m_normal;
            for(ui32 j = 1; j < uncompressed_vertices[i].m_contains_in_face_size; ++j)
            {
                normal += faces[uncompressed_vertices[i].m_contains_in_face[j]].m_normal;
            }
            normal = glm::normalize(normal);
            uncompressed_vertices[i].m_normal = normal;
        }
        
        for(ui32 i = 0; i < size.x * size.y; ++i)
        {
            compressed_vertices[i].m_position = uncompressed_vertices[i].m_position;
            compressed_vertices[i].m_texcoord = glm::packUnorm2x16(uncompressed_vertices[i].m_texcoord);
            compressed_vertices[i].m_normal = glm::packSnorm4x8(glm::vec4(uncompressed_vertices[i].m_normal, 0.0f));
        }
        
        { // writing compressed vertices metadata
            std::ofstream stream;
            stream.open(heightmap_loader::get_compressed_vertices_mmap_filename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
            if(!stream.is_open())
            {
                assert(false);
            }
            
            for(ui32 i = 0; i < size.x * size.y; ++i)
            {
                stream.write((char*)&compressed_vertices[i], sizeof(heightmap_container::compressed_vertex));
            }
            stream.close();
        }
        
        { // writing uncompressed vertices metadata
            std::ofstream stream;
            stream.open(heightmap_loader::get_uncompressed_vertices_mmap_filename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
            if(!stream.is_open())
            {
                assert(false);
            }
            
            for(ui32 i = 0; i < size.x * size.y; ++i)
            {
                stream.write((char*)&uncompressed_vertices[i], sizeof(heightmap_container::uncompressed_vertex));
            }
            stream.close();
        }
        
        { // writing faces metadata
            std::ofstream stream;
            stream.open(heightmap_loader::get_faces_mmap_filename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
            if(!stream.is_open())
            {
                assert(false);
            }
            
            for(ui32 i = 0; i < (size.x - 1) * (size.y - 1) * 2; ++i)
            {
                stream.write((char*)&faces[i], sizeof(heightmap_container::face));
            }
            stream.close();
        }
    }
    
    void heightmap_geometry_generator::create_vbos_data(const std::shared_ptr<heightmap_container>& container, const std::string& filename)
    {
        std::ofstream stream;
        stream.open(heightmap_loader::get_vbos_mmap_filename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
        if(!stream.is_open())
        {
            assert(false);
        }
        
        glm::ivec2 vertices_offset(0);
        vbo::vertex_attribute vertex;
        for(ui32 i = 0; i < container->get_chunks_num().x; ++i)
        {
            vertices_offset.y = 0;
            for(ui32 j = 0; j < container->get_chunks_num().y; ++j)
            {
                for(ui32 x = 0; x < container->get_chunk_size().x; ++x)
                {
                    for(ui32 y = 0; y < container->get_chunk_size().y; ++y)
                    {
                        vertex.m_position = container->get_vertex_position(x + vertices_offset.x, y + vertices_offset.y);
                        vertex.m_normal = container->get_compressed_vertex_normal(x + vertices_offset.x, y + vertices_offset.y);
                        vertex.m_texcoord = glm::packUnorm2x16(glm::vec2(static_cast<f32>(x) / static_cast<f32>(container->get_chunk_size().x),
                                                                         static_cast<f32>(y) / static_cast<f32>(container->get_chunk_size().y)));
                        stream.write((char*)&vertex, sizeof(vbo::vertex_attribute));
                    }
                }
                vertices_offset.y += container->get_chunk_size().y - 1;
            }
            vertices_offset.x += container->get_chunk_size().x - 1;
        }
        stream.close();
    }
    
    void heightmap_geometry_generator::create_ibos_data(const std::shared_ptr<heightmap_container>& container, const std::string& filename)
    {
        std::ofstream stream;
        stream.open(heightmap_loader::get_ibos_mmap_filename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
        if(!stream.is_open())
        {
            assert(false);
        }
        
        for(ui32 i = 0; i < container->get_chunks_num().x; ++i)
        {
            for(ui32 j = 0; j < container->get_chunks_num().y; ++j)
            {
                for(ui32 k = 0; k < heightmap_container::e_heigtmap_chunk_lod_max; ++k)
                {
                    glm::ivec2 current_chunk_size = glm::ivec2(container->get_chunk_lod_size(static_cast<heightmap_container::e_heigtmap_chunk_lod>(k)).x % 2 == 0 ?
                                                               container->get_chunk_lod_size(static_cast<heightmap_container::e_heigtmap_chunk_lod>(k)).x :
                                                               container->get_chunk_lod_size(static_cast<heightmap_container::e_heigtmap_chunk_lod>(k)).x - 1,
                                                               
                                                               container->get_chunk_lod_size(static_cast<heightmap_container::e_heigtmap_chunk_lod>(k)).y % 2 == 0 ?
                                                               container->get_chunk_lod_size(static_cast<heightmap_container::e_heigtmap_chunk_lod>(k)).y :
                                                               container->get_chunk_lod_size(static_cast<heightmap_container::e_heigtmap_chunk_lod>(k)).y - 1);
                    
                    glm::ivec2 vertices_lod_offset = glm::ivec2(std::max((container->get_chunk_size().x - 1) / current_chunk_size.x, 1),
                                                                std::max((container->get_chunk_size().y - 1) / current_chunk_size.y, 1));
                    
                    
                    i32 vertices_line_offset = container->get_chunk_size().x;
                    glm::ivec2 current_chunk_lod_start_index = glm::ivec2(0);
                    current_chunk_lod_start_index.x += k != heightmap_container::e_heigtmap_chunk_lod_01 ? 1 : 0;
                    current_chunk_lod_start_index.y += k != heightmap_container::e_heigtmap_chunk_lod_01 ? 1 : 0;
                    
                    std::vector<ui16> addition_indices;
                    if(k != heightmap_container::e_heigtmap_chunk_lod_01)
                    {
                        std::vector<ui16> current_lod_edge_indices;
                        for(ui32 x = 0; x <= current_chunk_size.x; ++x)
                        {
                            ui32 index = x * vertices_lod_offset.x + vertices_line_offset * vertices_lod_offset.y;
                            current_lod_edge_indices.push_back(index);
                        }
                        
                        ui32 current_lod_index = 0;
                        for(ui32 x = 0; x < container->get_chunk_size().x; ++x)
                        {
                            ui32 main_lod_index = x;
                            if(current_lod_edge_indices.size() > current_lod_index + 1)
                            {
                                if(x != 0 && x % vertices_lod_offset.x == 0)
                                {
                                    addition_indices.push_back(current_lod_edge_indices[current_lod_index]);
                                    addition_indices.push_back(current_lod_edge_indices[current_lod_index + 1]);
                                    addition_indices.push_back(main_lod_index);
                                    current_lod_index++;
                                }
                                
                                if(current_lod_edge_indices.size() - 1 == current_lod_index)
                                {
                                    break;
                                }
                                else
                                {
                                    addition_indices.push_back(main_lod_index);
                                    addition_indices.push_back(current_lod_edge_indices[current_lod_index]);
                                    addition_indices.push_back(main_lod_index + 1);
                                }
                            }
                        }
                        current_lod_edge_indices.clear();
                        
                        for(ui32 x = 0; x <= current_chunk_size.x; ++x)
                        {
                            ui32 index = x * vertices_lod_offset.x + vertices_line_offset * vertices_lod_offset.y * (current_chunk_size.y - 1);
                            current_lod_edge_indices.push_back(index);
                        }
                        
                        current_lod_index = 0;
                        for(ui32 x = 0; x < container->get_chunk_size().x; ++x)
                        {
                            ui32 main_lod_index = x + vertices_line_offset * (container->get_chunk_size().x - 1);
                            if(current_lod_edge_indices.size() > current_lod_index + 1)
                            {
                                if(x != 0 && x % vertices_lod_offset.x == 0)
                                {
                                    addition_indices.push_back(current_lod_edge_indices[current_lod_index]);
                                    addition_indices.push_back(main_lod_index);
                                    addition_indices.push_back(current_lod_edge_indices[current_lod_index + 1]);
                                    current_lod_index++;
                                }
                                
                                if(current_lod_edge_indices.size() - 1 == current_lod_index)
                                {
                                    break;
                                }
                                else
                                {
                                    addition_indices.push_back(current_lod_edge_indices[current_lod_index]);
                                    addition_indices.push_back(main_lod_index);
                                    addition_indices.push_back(main_lod_index + 1);
                                }
                            }
                        }
                        current_lod_edge_indices.clear();
                        
                        for(ui32 y = 0; y <= current_chunk_size.y; ++y)
                        {
                            ui32 index = vertices_lod_offset.x + y * vertices_line_offset * vertices_lod_offset.y;
                            current_lod_edge_indices.push_back(index);
                        }
                        
                        current_lod_index = 0;
                        for(ui32 y = 0; y < container->get_chunk_size().y; ++y)
                        {
                            ui32 main_lod_index = y * container->get_chunk_size().y;
                            if(current_lod_edge_indices.size() > current_lod_index + 1)
                            {
                                if(y != 0 && y % vertices_lod_offset.y == 0)
                                {
                                    addition_indices.push_back(current_lod_edge_indices[current_lod_index]);
                                    addition_indices.push_back(main_lod_index);
                                    addition_indices.push_back(current_lod_edge_indices[current_lod_index + 1]);
                                    current_lod_index++;
                                }
                                
                                if(current_lod_edge_indices.size() - 1 == current_lod_index)
                                {
                                    break;
                                }
                                else
                                {
                                    addition_indices.push_back(current_lod_edge_indices[current_lod_index]);
                                    addition_indices.push_back(main_lod_index);
                                    addition_indices.push_back(main_lod_index + container->get_chunk_size().y);
                                }
                            }
                        }
                        current_lod_edge_indices.clear();
                        
                        for(ui32 y = 0; y <= current_chunk_size.y; ++y)
                        {
                            ui32 index = vertices_lod_offset.x * (current_chunk_size.x - 1) + y * vertices_line_offset * vertices_lod_offset.y;
                            current_lod_edge_indices.push_back(index);
                        }
                        
                        current_lod_index = 0;
                        for(ui32 y = 0; y < container->get_chunk_size().y; ++y)
                        {
                            ui32 main_lod_index = (container->get_chunk_size().x - 1) + y * container->get_chunk_size().y;
                            if(current_lod_edge_indices.size() > current_lod_index + 1)
                            {
                                if(y != 0 && y % vertices_lod_offset.y == 0)
                                {
                                    addition_indices.push_back(current_lod_edge_indices[current_lod_index]);
                                    addition_indices.push_back(current_lod_edge_indices[current_lod_index + 1]);
                                    addition_indices.push_back(main_lod_index);
                                    current_lod_index++;
                                }
                                
                                if(current_lod_edge_indices.size() - 1 == current_lod_index)
                                {
                                    break;
                                }
                                else
                                {
                                    addition_indices.push_back(main_lod_index);
                                    addition_indices.push_back(current_lod_edge_indices[current_lod_index]);
                                    addition_indices.push_back(main_lod_index + container->get_chunk_size().y);
                                }
                            }
                        }
                        current_lod_edge_indices.clear();
                    }
                    
                    current_chunk_size.x -= k != heightmap_container::e_heigtmap_chunk_lod_01 ? 2 : 0;
                    current_chunk_size.y -= k != heightmap_container::e_heigtmap_chunk_lod_01 ? 2 : 0;
                    
                    ui32 indices_count = current_chunk_size.x * current_chunk_size.y * 6 + static_cast<ui32>(addition_indices.size());
                    
                    current_chunk_size.x += k != heightmap_container::e_heigtmap_chunk_lod_01 ? 1 : 0;
                    current_chunk_size.y += k != heightmap_container::e_heigtmap_chunk_lod_01 ? 1 : 0;
                    
                    ui16* indices = new ui16[indices_count];
                    
                    ui32 index = 0;
                    
                    for(ui32 x = 0; x < addition_indices.size(); ++x)
                    {
                        indices[index] = addition_indices[x];
                        index++;
                    }
                    
                    for(ui32 x = current_chunk_lod_start_index.x; x < current_chunk_size.x; ++x)
                    {
                        for(ui32 y = current_chunk_lod_start_index.y; y < current_chunk_size.y; ++y)
                        {
                            indices[index] = x * vertices_lod_offset.x + y * vertices_lod_offset.y * vertices_line_offset;
                            index++;
                            indices[index] = x * vertices_lod_offset.x + (y * vertices_lod_offset.y + vertices_lod_offset.y) * vertices_line_offset;
                            index++;
                            indices[index] = x * vertices_lod_offset.x + vertices_lod_offset.x + y * vertices_lod_offset.y * vertices_line_offset;
                            index++;
                            
                            indices[index] = x * vertices_lod_offset.x + (y * vertices_lod_offset.y + vertices_lod_offset.y) * vertices_line_offset;
                            index++;
                            indices[index] = x * vertices_lod_offset.x + vertices_lod_offset.x + (y * vertices_lod_offset.y + vertices_lod_offset.y) * vertices_line_offset;
                            index++;
                            indices[index] = x * vertices_lod_offset.x + vertices_lod_offset.x + y * vertices_lod_offset.y * vertices_line_offset;
                            index++;
                        }
                    }
                    
                    for(ui32 index_01 = 0; index_01 < 2; ++index_01)
                    {
                        for(ui32 index_02 = 0; index_02 < indices_count; ++index_02)
                        {
                            stream.write((char*)&indices[index_02], sizeof(ui16));
                        }
                    }
                    delete [] indices;
                }
            }
        }
        stream.close();
    }
    
    void heightmap_geometry_generator::generate_tangent_space(const std::shared_ptr<heightmap_container>& container, const std::string& filename)
    {
        if(!heightmap_loader::is_tangent_space_mmap_exist(filename))
        {
            heightmap_geometry_generator::create_tangent_space(container, filename);
        }
    }
    
    void heightmap_geometry_generator::create_tangent_space(const std::shared_ptr<heightmap_container>& container, const std::string& filename)
    {
        for(ui32 i = 0; i < container->get_chunks_num().x; ++i)
        {
            for(ui32 j = 0; j < container->get_chunks_num().y; ++j)
            {
                ui32 index = i + j * container->get_chunks_num().x;
                heightmap_geometry_generator::generate_tangent_space(container, index);
            }
        }
        
        std::ofstream stream;
        stream.open(heightmap_loader::get_tangent_space_mmap_filename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
        if(!stream.is_open())
        {
            assert(false);
        }
        
        ui8 value = 1;
        stream.write((char *)&value, sizeof(ui8));
        stream.close();
    }
    
    void heightmap_geometry_generator::generate_tangent_space(const std::shared_ptr<heightmap_container>& container, ui32 index)
    {
        std::vector<std::pair<glm::vec3, ui32>> tangents;
        vbo::vertex_attribute* vertices = container->get_vbo_mmap(index)->get_pointer();
        ui32 num_vertices = container->get_vbo_mmap(index)->get_size();
        
        tangents.resize(num_vertices, std::make_pair(glm::vec3(0.0f), 0));
        
        ui16* indices = container->get_ibo_mmap(index, heightmap_container::e_heigtmap_chunk_lod_01)->get_source_pointer();
        ui32 num_indices = container->get_ibo_mmap(index, heightmap_container::e_heigtmap_chunk_lod_01)->get_size();
        
        std::pair<i32, i32> minmax = std::make_pair(INT16_MAX, INT16_MIN);
        for (ui32 i = 0; i < num_indices; i += 3)
        {
            glm::vec3 point_01 = vertices[indices[i + 0]].m_position;
            glm::vec3 point_02 = vertices[indices[i + 1]].m_position;
            glm::vec3 point_03 = vertices[indices[i + 2]].m_position;
            
            glm::vec2 texcoord_01 = glm::unpackUnorm2x16(vertices[indices[i + 0]].m_texcoord);
            glm::vec2 texcoord_02 = glm::unpackUnorm2x16(vertices[indices[i + 1]].m_texcoord);
            glm::vec2 texcoord_03 = glm::unpackUnorm2x16(vertices[indices[i + 2]].m_texcoord);
            
            glm::vec3 tangent = heightmap_geometry_generator::generate_tangent(point_01, point_02, point_03,
                                                                               texcoord_01, texcoord_02, texcoord_03);
            
            assert(indices[i + 0] < num_vertices);
            assert(indices[i + 1] < num_vertices);
            assert(indices[i + 2] < num_vertices);
            
            tangents[indices[i + 0]].first += tangent;
            tangents[indices[i + 0]].second++;
            
            tangents[indices[i + 1]].first += tangent;
            tangents[indices[i + 1]].second++;
            
            tangents[indices[i + 2]].first += tangent;
            tangents[indices[i + 2]].second++;
            
            minmax.first = minmax.first < indices[i + 0] ? minmax.first : indices[i + 0];
            minmax.second = minmax.second > indices[i + 0] ? minmax.second : indices[i + 0];
            
            minmax.first = minmax.first < indices[i + 1] ? minmax.first : indices[i + 1];
            minmax.second = minmax.second > indices[i + 1] ? minmax.second : indices[i + 1];
            
            minmax.first = minmax.first < indices[i + 2] ? minmax.first : indices[i + 2];
            minmax.second = minmax.second > indices[i + 2] ? minmax.second : indices[i + 2];
        }
        
        for(i32 i = minmax.first; i <= minmax.second; i++)
        {
            glm::vec3 tangent = tangents[i].first / static_cast<f32>(tangents[i].second);
            glm::vec4 normal = glm::unpackSnorm4x8(vertices[i].m_normal);
            tangent = heightmap_geometry_generator::ortogonalize(glm::vec3(normal.x, normal.y, normal.z), tangent);
            vertices[i].m_tangent = glm::packSnorm4x8(glm::vec4(tangent.x, tangent.y, tangent.z, 0.0));
        }
    }
    
    glm::vec3 heightmap_geometry_generator::generate_tangent(const glm::vec3& point_01, const glm::vec3& point_02, const glm::vec3& point_03,
                                                             const glm::vec2& texcoord_01, const glm::vec2& texcoord_02, const glm::vec2& texcoord_03)
    {
        glm::vec3 P = point_02 - point_01;
        glm::vec3 Q = point_03 - point_01;
        f32 s1 = texcoord_02.x - texcoord_01.x;
        f32 t1 = texcoord_02.y - texcoord_01.y;
        f32 s2 = texcoord_03.x - texcoord_01.x;
        f32 t2 = texcoord_03.y - texcoord_01.y;
        f32 pqMatrix[2][3];
        pqMatrix[0][0] = P[0];
        pqMatrix[0][1] = P[1];
        pqMatrix[0][2] = P[2];
        pqMatrix[1][0] = Q[0];
        pqMatrix[1][1] = Q[1];
        pqMatrix[1][2] = Q[2];
        f32 temp = 1.0f / ( s1 * t2 - s2 * t1);
        f32 stMatrix[2][2];
        stMatrix[0][0] = t2 * temp;
        stMatrix[0][1] = -t1 * temp;
        stMatrix[1][0] = -s2 * temp;
        stMatrix[1][1] = s1 * temp;
        f32 tbMatrix[2][3];
        tbMatrix[0][0] = stMatrix[0][0] * pqMatrix[0][0] + stMatrix[0][1] * pqMatrix[1][0];
        tbMatrix[0][1] = stMatrix[0][0] * pqMatrix[0][1] + stMatrix[0][1] * pqMatrix[1][1];
        tbMatrix[0][2] = stMatrix[0][0] * pqMatrix[0][2] + stMatrix[0][1] * pqMatrix[1][2];
        tbMatrix[1][0] = stMatrix[1][0] * pqMatrix[0][0] + stMatrix[1][1] * pqMatrix[1][0];
        tbMatrix[1][1] = stMatrix[1][0] * pqMatrix[0][1] + stMatrix[1][1] * pqMatrix[1][1];
        tbMatrix[1][2] = stMatrix[1][0] * pqMatrix[0][2] + stMatrix[1][1] * pqMatrix[1][2];
        return glm::normalize(glm::vec3(tbMatrix[0][0], tbMatrix[0][1], tbMatrix[0][2]));
    }
    
    glm::vec3 heightmap_geometry_generator::get_closest_point_on_line(const glm::vec3& a, const glm::vec3& b, const glm::vec3& p)
    {
        glm::vec3 c = p - a;
        glm::vec3 v = b - a;
        f32 d = v.length();
        v = glm::normalize(v);
        f32 t = glm::dot( v, c );
        
        if ( t < 0.0f )
            return a;
        if ( t > d )
            return b;
        v *= t;
        return ( a + v );
    }
    
    glm::vec3 heightmap_geometry_generator::ortogonalize(const glm::vec3& v1, const glm::vec3& v2)
    {
        glm::vec3 v2ProjV1 = heightmap_geometry_generator::get_closest_point_on_line( v1, -v1, v2 );
        glm::vec3 res = v2 - v2ProjV1;
        res = glm::normalize(res);
        return res;
    }
    
    void heightmap_geometry_generator::generate_smooth_texcoord(const std::shared_ptr<heightmap_container> &container, const std::string &filename)
    {
        heightmap_geometry_generator::create_smooth_texcoord(container, filename);
    }
    
    void heightmap_geometry_generator::create_smooth_texcoord(const std::shared_ptr<heightmap_container> &container, const std::string &filename)
    {
        for(ui32 i = 0; i < container->get_chunks_num().x; ++i)
        {
            for(ui32 j = 0; j < container->get_chunks_num().y; ++j)
            {
                ui32 index = i + j * container->get_chunks_num().x;
                heightmap_geometry_generator::generate_smooth_texcoord(container, index);
            }
        }
    }
    
    void heightmap_geometry_generator::generate_smooth_texcoord(const std::shared_ptr<heightmap_container>& container, ui32 index)
    {
        vbo::vertex_attribute* vertices = container->get_vbo_mmap(index)->get_pointer();
        ui32 num_vertices = container->get_vbo_mmap(index)->get_size();
        
        const glm::ivec2 size = glm::ivec2(sqrt(num_vertices));
        
        std::vector<f32> widths_x;
        std::vector<f32> widths_y;
        
        for(i32 x = 0; x < size.x; ++x)
        {
            f32 width_x = 0;
            f32 width_y = 0;
            for(i32 y = 0; y < size.y; ++y)
            {
                if(y + 1 < size.y)
                {
                    width_x += glm::distance(vertices[x + y * size.y].m_position,
                                             vertices[x + (y + 1) * size.y].m_position);
                    
                    width_y += glm::distance(vertices[y + x * size.x].m_position,
                                             vertices[y + 1 + x * size.x].m_position);
                }
            }
            widths_y.push_back(width_y);
            widths_x.push_back(width_x);
        }
        
        for(i32 x = 0; x < size.x; ++x)
        {
            f32 width_x = widths_x[x];
            f32 distance_summ_x = 0.0f;
            
            for(i32 y = 0; y < size.y; ++y)
            {
                if(y + 1 < size.y)
                {
                    f32 distance_x = glm::distance(vertices[x + y * size.y].m_position,
                                                   vertices[x + (y + 1) * size.y].m_position);
                    glm::vec2 texcoord = glm::unpackUnorm2x16(vertices[x + (y + 1) * size.y].m_texcoord);
                    texcoord.x = (distance_summ_x + distance_x) / width_x;
                    distance_summ_x += distance_x;
                    texcoord = glm::clamp(texcoord, 0.0f, 1.0f);
                    vertices[x + (y + 1) * size.y].m_texcoord = glm::packUnorm2x16(texcoord);
                }
                else
                {
                    glm::vec2 texcoord = glm::unpackUnorm2x16(vertices[x + y * size.y].m_texcoord);
                    texcoord.x = 1.0f;
                    texcoord = glm::clamp(texcoord, 0.0f, 1.0f);
                    vertices[x + y * size.y].m_texcoord = glm::packUnorm2x16(texcoord);
                }
            }
        }
        
        for(i32 y = 0; y < size.y; ++y)
        {
            f32 width_y = widths_y[y];
            f32 distance_summ_y = 0.0f;
            
            for(i32 x = 0; x < size.x; ++x)
            {
                if(x + 1 < size.x)
                {
                    f32 distance_y = glm::distance(vertices[x + y * size.y].m_position,
                                                   vertices[x + 1 + y * size.y].m_position);
                    glm::vec2 texcoord = glm::unpackUnorm2x16(vertices[x + 1 + y * size.y].m_texcoord);
                    texcoord.y = (distance_summ_y + distance_y) / width_y;
                    distance_summ_y += distance_y;
                    texcoord = glm::clamp(texcoord, 0.0f, 1.0f);
                    vertices[x + 1 + y * size.y].m_texcoord = glm::packUnorm2x16(texcoord);
                }
                else
                {
                    glm::vec2 texcoord = glm::unpackUnorm2x16(vertices[x + y * size.y].m_texcoord);
                    texcoord.y = 1.0f;
                    texcoord = glm::clamp(texcoord, 0.0f, 1.0f);
                    vertices[x + y * size.y].m_texcoord = glm::packUnorm2x16(texcoord);
                }
            }
        }
    }
    
    void heightmap_geometry_generator::generate_attaches_to_vbo(const std::shared_ptr<heightmap_container> &container, const std::string &filename)
    {
        if(!heightmap_loader::is_attaches_to_vbo_mmap_exist(filename))
        {
            heightmap_geometry_generator::create_attaches_to_vbo(container, filename);
        }
    }
    
    void heightmap_geometry_generator::create_attaches_to_vbo(const std::shared_ptr<heightmap_container> &container, const std::string &filename)
    {
        glm::ivec2 vertices_offset(0);
        vbo::vertex_attribute vertex;
        for(ui32 i = 0; i < container->get_chunks_num().x; ++i)
        {
            vertices_offset.y = 0;
            for(ui32 j = 0; j < container->get_chunks_num().y; ++j)
            {
                for(ui32 x = 0; x < container->get_chunk_size().x; ++x)
                {
                    for(ui32 y = 0; y < container->get_chunk_size().y; ++y)
                    {
                        ui32 index = y + x * container->get_chunk_size().y;
                        container->attach_uncompressed_vertex_to_vbo(x + vertices_offset.x, y + vertices_offset.y,
                                                                     i + j * container->get_chunks_num().x, index);
                    }
                }
                vertices_offset.y += container->get_chunk_size().y - 1;
            }
            vertices_offset.x += container->get_chunk_size().x - 1;
        }
        
        std::ofstream stream;
        stream.open(heightmap_loader::get_attaches_to_vbo_mmap_filename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
        if(!stream.is_open())
        {
            assert(false);
        }
        
        ui8 value = 1;
        stream.write((char *)&value, sizeof(ui8));
        stream.close();
    }
    
    void heightmap_geometry_generator::update_vertices(const std::shared_ptr<heightmap_container>& container, const std::vector<glm::vec3>& vertices)
    {
        for(ui32 i = 0; i < vertices.size(); ++i)
        {
            i32 x = static_cast<i32>(vertices.at(i).x);
            i32 z = static_cast<i32>(vertices.at(i).z);
            i32 index = x + z * container->get_main_size().x;
            container->get_uncopressed_vertices()[index].m_position = vertices.at(i);
        }
        
        for(ui32 i = 0; i < vertices.size(); ++i)
        {
            i32 x = static_cast<i32>(vertices.at(i).x);
            i32 z = static_cast<i32>(vertices.at(i).z);
            i32 index = x + z * container->get_main_size().x;
            
            ui8 contains_in_vbo_size = 0;
            glm::ivec2 *contains_in_vbo = container->attached_vertices_to_vbo(x, z, &contains_in_vbo_size);
            assert(contains_in_vbo != nullptr);
            
            for(ui32 j = 0; j < contains_in_vbo_size; ++j)
            {
                ui32 face = container->get_uncopressed_vertices()[index].m_contains_in_face[j];
                
                glm::vec3 point_01 = container->get_uncopressed_vertices()[container->get_faces()[face].m_indexes[0]].m_position;
                glm::vec3 point_02 = container->get_uncopressed_vertices()[container->get_faces()[face].m_indexes[1]].m_position;
                glm::vec3 point_03 = container->get_uncopressed_vertices()[container->get_faces()[face].m_indexes[2]].m_position;
                
                glm::vec3 edge_01 = point_02 - point_01;
                glm::vec3 edge_02 = point_03 - point_01;
                glm::vec3 normal = glm::cross(edge_01, edge_02);
                f32 sin = glm::length(normal) / (glm::length(edge_01) * glm::length(edge_02));
                container->get_faces()[face].m_normal = glm::normalize(normal) * asinf(sin);
            }
        }
        
        for(ui32 i = 0; i < vertices.size(); ++i)
        {
            i32 x = static_cast<i32>(vertices.at(i).x);
            i32 z = static_cast<i32>(vertices.at(i).z);
            i32 index = x + z * container->get_main_size().x;
            
            ui8 contains_in_vbo_size = 0;
            glm::ivec2 *contains_in_vbo = container->attached_vertices_to_vbo(x, z, &contains_in_vbo_size);
            assert(contains_in_vbo != nullptr);
            
            glm::vec3 normal = container->get_faces()[container->get_uncopressed_vertices()[index].m_contains_in_face[0]].m_normal;
            for(ui32 j = 1; j < contains_in_vbo_size; ++j)
            {
                normal += container->get_faces()[container->get_uncopressed_vertices()[index].m_contains_in_face[j]].m_normal;
            }
            container->get_uncopressed_vertices()[index].m_normal = glm::normalize(normal);
            container->get_compressed_vertices()[index].m_position = container->get_uncopressed_vertices()[index].m_position;
            container->get_compressed_vertices()[index].m_normal = glm::packSnorm4x8(glm::vec4(container->get_uncopressed_vertices()[index].m_normal, 0.f));
        }
    }
}

