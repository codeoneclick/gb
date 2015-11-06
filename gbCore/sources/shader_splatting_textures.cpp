//
//  shader_splatting_textures.cpp
//  gbCore
//
//  Created by sergey.sergeev on 11/6/15.
//  Copyright © 2015 sergey.sergeev. All rights reserved.
//

#include "main_headers.h"

const char* shader_splatting_textures_vert = string_shader
(
#if defined(__OPENGL_30__)
 
 out vec2 v_texcoord;
 
#else
 
 varying vec2 v_texcoord;
 
#endif
 
 void main(void)
{
    v_texcoord = a_texcoord;
    gl_Position = vec4(a_position, 1.0);
}
 );

const char* shader_splatting_textures_frag = string_shader
(
#if defined(__OPENGL_30__)
 
 in vec2 v_texcoord;
 
#else
 
 varying vec2 v_texcoord;
 
#endif
 
 uniform sampler2D  sampler_01;
 
 void main()
{
    gl_FragColor = texture2D(sampler_01, v_texcoord);
}
 );