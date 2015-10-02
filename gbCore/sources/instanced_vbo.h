//
//  instanced_vbo.h
//  gbCore
//
//  Created by sergey.sergeev on 10/1/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#ifndef instanced_vbo_h
#define instanced_vbo_h

#include "main_headers.h"

namespace gb
{
    class instanced_vbo
    {
    private:
        
    protected:
        
        ui32 m_handle;
        
        i8* m_data;
        ui32 m_allocated_size;
        ui32 m_used_size;
        
    public:
        
        instanced_vbo();
        ~instanced_vbo();
        
        template<typename T_VALUE> void allocate_data(ui32 size)
        {
            assert(size != 0);
            m_allocated_size = size;
            
            gl_create_buffers(1, &m_handle);
            
            m_data = new i8[m_allocated_size * sizeof(T_VALUE)];
            memset(m_data, 0x0, sizeof(T_VALUE) * m_allocated_size);
        }
        
        ui32 get_allocated_size() const;
        ui32 get_used_size() const;
        
        template<typename T_VALUE> T_VALUE* lock() const
        {
            assert(m_data != nullptr);
            return (T_VALUE*)(m_data);
        }
        
        template<typename T_VALUE> void unlock(ui32 size = 0)
        {
            assert(m_data != nullptr);
            assert(m_allocated_size != 0);
            m_used_size = size > 0 && size < m_allocated_size ? size : m_allocated_size;
            gl_bind_buffer(GL_ARRAY_BUFFER, m_handle);
            gl_push_buffer_data(GL_ARRAY_BUFFER, sizeof(T_VALUE) * m_used_size, m_data, GL_DYNAMIC_DRAW);
        }
        
        template<typename T_VALUE> void bind(i32 attribute)
        {
            i32 value_size = sizeof(T_VALUE);
            assert(value_size % sizeof(f32) == 0);
            
            i32 f32_num = value_size / sizeof(f32);
            
            gl_bind_buffer(GL_ARRAY_BUFFER, m_handle);
            if(f32_num <= 4)
            {
                gl_enable_vertex_attribute(attribute);
                gl_bind_vertex_attribute(attribute, f32_num, GL_FLOAT, GL_FALSE, sizeof(T_VALUE), NULL);
                gl_set_vertex_attribute_divisor(attribute, 1);
            }
            else
            {
                assert(f32_num % 4 == 0);
                i32 vec4_num = f32_num / 4;
                
                for (ui32 i = 0; i < vec4_num ; i++)
                {
                    gl_enable_vertex_attribute(attribute + i);
                    gl_bind_vertex_attribute(attribute + i, 4, GL_FLOAT, GL_FALSE, sizeof(T_VALUE),
                                             (const GLvoid*)(sizeof(GLfloat) * i * 4));
                    gl_set_vertex_attribute_divisor(attribute + i, 1);
                }
            }
        }
        
        void unbind(i32 attribute)
        {
            gl_bind_buffer(GL_ARRAY_BUFFER, m_handle);
            gl_disable_vertex_attribute(attribute);
            gl_bind_buffer(GL_ARRAY_BUFFER, NULL);
        }
    };
};

#endif
