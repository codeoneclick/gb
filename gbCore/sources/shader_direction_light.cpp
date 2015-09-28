//
//  shader_direction_light.cpp
//  gbCore
//
//  Created by sergey.sergeev on 9/22/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "main_headers.h"

const char* shader_direction_light_vert = string_shader
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

const char* shader_direction_light_frag = string_shader
(
#if defined(__OPENGL_30__)
 
 in vec2 v_texcoord;
 
#else
 
 varying vec2 v_texcoord;
 
#endif
 
 uniform vec3 u_light_direction;
 
 uniform float u_f32_camera_near;
 uniform float u_f32_camera_far;
 
 uniform mat4 u_mat_i_vp;
 
 uniform sampler2D  sampler_01;
 uniform sampler2D  sampler_02;
 
 void main()
{
    vec4 normal_color = texture2D(sampler_02, v_texcoord);
    vec3 normal = normal_color.rgb * 2.0 - 1.0;
    
    gl_FragColor = vec4(clamp(dot(normal, u_light_direction), 0.0, 1.0) * 0.5);
}
 );
