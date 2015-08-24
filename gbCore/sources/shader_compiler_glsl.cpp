//
//  shader_compiler_glsl.cpp
//  gbCore
//
//  Created by Sergey Sergeev on 8/12/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "shader_compiler_glsl.h"
#include "resource_serializer.h"

namespace gb
{
    std::string shader_compiler_glsl::m_vs_shader_header = "#if defined(__IOS__)\
    #define GL_APPLE_clip_distance 1\
    #extension GL_APPLE_clip_distance : enable\
    precision highp float;\
    #endif\
    #if defined(__OPENGL_30__)\
    layout (location = 0) in vec3 a_position;\
    layout (location = 1) in vec2 a_texcoord;\
    layout (location = 2) in vec4 a_normal;\
    layout (location = 3) in vec4 a_tangent;\
    layout (location = 4) in vec4 a_color;\
    layout (location = 5) in vec4 a_extra;\
    #else\
    attribute vec3 a_position;\
    attribute vec2 a_texcoord;\
    attribute vec4 a_normal;\
    attribute vec4 a_tangent;\
    attribute vec4 a_color;\
    attribute vec4 a_extra;\
    #endif";
    
    std::string shader_compiler_glsl::m_fs_shader_header = "#if defined(__IOS__)\
    precision highp float;\
    #endif\
    #if defined(__OPENGL_30__)\
    layout (location = 0) out vec4 attachment_01;\
    #define gl_FragColor attachment_01\
    #define texture2D texture\
    #define textureCube texture\
    #endif";
    
    ui32 shader_compiler_glsl::compile(const std::string& source_code, GLenum shader_type, std::string* out_message, bool* out_success)
    {
        ui32 handle = glCreateShader(shader_type);
        
        std::string shader_header;
        if(shader_type == GL_VERTEX_SHADER)
        {
            shader_header = m_vs_shader_header;
        }
        else if(shader_type == GL_FRAGMENT_SHADER)
        {
            shader_header = m_fs_shader_header;
        }
        
        std::string define = "";
#if defined(__OPENGL_30__)
        
#if defined(__OSX__)
        
        define.append("#version 410\n");
        
#elif defined(__IOS__)
        
        define.append("#version 300 es\n");
        
#endif
#endif
        
#if defined(__OSX__)
        
        define.append("#define __OSX__\n");
        
#elif defined(__IOS__)
        
        define.append("#define __IOS__\n");
        
#endif
        
#if defined(__OPENGL_30__)
        
        define.append("#define __OPENGL_30__\n");
        
#endif
        
        define.append(shader_header);
        
        char* shader_data = const_cast<char*>(source_code.c_str());
        char* define_data = const_cast<char*>(define.c_str());
        char* sources[2] = { define_data, shader_data};
        glShaderSource(handle, 2, sources, NULL);
        glCompileShader(handle);
        
        i32 success;
        glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
        
        if(!success)
        {
            i32 message_size = 0;
            glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &message_size);
            GLchar *message_string = new GLchar[message_size];
            memset(message_string, NULL, message_size * sizeof(GLchar));
            glGetShaderInfoLog(handle, message_size, NULL, message_string);
            if(out_message)
            {
                *out_message = message_string;
            }
        }
        if(out_success)
        {
            *out_success = success;
        }
        return handle;
    }
    
    ui32 shader_compiler_glsl::link(ui32 vs_handle, ui32 fs_handle, std::string* out_message, bool* out_success)
    {
        ui32 handle = glCreateProgram();
        glAttachShader(handle, vs_handle);
        glAttachShader(handle, fs_handle);
        glLinkProgram(handle);
        
        i32 success;
        glGetProgramiv(handle, GL_LINK_STATUS, &success);
        
        if(!success)
        {
            i32 message_size = 0;
            glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &message_size);
            GLchar *message_string = new GLchar[message_size];
            memset(message_string, NULL, message_size * sizeof(GLchar));
            glGetShaderInfoLog(handle, message_size, NULL, message_string);
            if(out_message)
            {
                *out_message = message_string;
            }
        }
        if(out_success)
        {
            *out_success = success;
        }
        return handle;
    }
}