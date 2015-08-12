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
    attribute vec4 a_color;\
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
        
        std::string shaderCommon;
        if(shader_type == GL_VERTEX_SHADER)
        {
            shaderCommon = CShaderCompiler_GLSL::getVSCommonShader();
        }
        else if(shader_type == GL_FRAGMENT_SHADER)
        {
            shaderCommon = CShaderCompiler_GLSL::getFSCommonShader();
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
        if(/*g_highPerformancePlatforms.count(getPlatform()) != 0*/ true)
        {
            define.append("#define __IOS_HIGH_PERFORMANCE__\n");
        }
        
#endif
        
#if defined(__OPENGL_30__)
        
        define.append("#define __OPENGL_30__\n");
        
#endif
        
        define.append(shaderCommon);
        
        char* shaderData = const_cast<char*>(sourceCode.c_str());
        char* defineData = const_cast<char*>(define.c_str());
        char* sources[2] = { defineData, shaderData};
        glShaderSource(handle, 2, sources, NULL);
        glCompileShader(handle);
        
        i32 success;
        glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
        
        if(!success)
        {
            i32 messageSize = 0;
            glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &messageSize);
            GLchar *messageString = new GLchar[messageSize];
            memset(messageString, NULL, messageSize * sizeof(GLchar));
            glGetShaderInfoLog(handle, messageSize, NULL, messageString);
            *outMessage = messageString;
        }
        else
        {
            outMessage = nullptr;
        }
        *outSuccess = success;
        return handle;
    }
    
    ui32 shader_compiler_glsl::link(ui32 vsHandle, ui32 fsHandle, std::string* outMessage, bool* outSuccess)
    {
        ui32 handle = glCreateProgram();
        glAttachShader(handle, vsHandle);
        glAttachShader(handle, fsHandle);
        glLinkProgram(handle);
        
        i32 success;
        glGetProgramiv(handle, GL_LINK_STATUS, &success);
        
        if(!success)
        {
            i32 messageSize = 0;
            glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &messageSize);
            GLchar *messageString = new GLchar[messageSize];
            memset(messageString, NULL, messageSize * sizeof(GLchar));
            glGetShaderInfoLog(handle, messageSize, NULL, messageString);
            *outMessage = messageString;
        }
        else
        {
            outMessage = nullptr;
        }
        *outSuccess = success;
        return handle;
    }
}