//
//  heightmap_accessor.hpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/14/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "heightmap_container.h"

namespace gb
{
    inline glm::ivec2 heightmap_accessor::get_main_size() const
    {
        assert(m_container != nullptr);
        return m_container ? m_container->get_main_size() : glm::ivec2(0);
    };
    
    inline glm::ivec2 heightmap_accessor::get_chunks_num() const
    {
        assert(m_container != nullptr);
        return m_container ? m_container->get_chunks_num() : glm::ivec2(0);
    };
    
    inline glm::ivec2 heightmap_accessor::get_chunk_size() const
    {
        assert(m_container != nullptr);
        return m_container ? m_container->get_chunk_size() : glm::ivec2(0);
    };
    
    inline const std::tuple<glm::vec3, glm::vec3> heightmap_accessor::get_chunk_bounds(ui32 i, ui32 j) const
    {
        i32 index = i + j * heightmap_accessor::get_chunks_num().x;
        assert(index >= 0 && index < m_chunks_bounds.size());
        return m_chunks_bounds[index];
    };
    
    inline texture_shared_ptr heightmap_accessor::get_deep_texture() const
    {
        assert(m_container != nullptr);
        return m_container ? m_container->get_deep_texture() : nullptr;
    };
};