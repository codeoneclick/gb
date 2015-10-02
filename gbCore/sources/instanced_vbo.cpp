//
//  instanced_vbo.cpp
//  gbCore
//
//  Created by sergey.sergeev on 10/1/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "instanced_vbo.h"

namespace gb
{
    instanced_vbo::instanced_vbo() :
    m_allocated_size(0),
    m_used_size(0)
    {

    }
    
    instanced_vbo::~instanced_vbo()
    {
        gl_delete_buffers(1, &m_handle);
        delete[] m_data;
    }
    
    ui32 instanced_vbo::get_allocated_size() const
    {
        return m_allocated_size;
    }
    
    ui32 instanced_vbo::get_used_size() const
    {
        return m_used_size;
    }
}