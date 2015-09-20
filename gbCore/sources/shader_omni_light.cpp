//
//  shader_omni_light.cpp
//  gbCore
//
//  Created by sergey.sergeev on 9/18/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "main_headers.h"

const char* shader_omni_light_vert = string_shader
(
 
#if defined(__OPENGL_30__)
 
 out vec4 v_screen_texcoord;
 
#else
 
 varying vec4 v_screen_texcoord;
 
#endif
 
 uniform mat4 u_mat_m;
 uniform mat4 u_mat_v;
 uniform mat4 u_mat_p;
 
 void main(void)
{
    vec4 position = u_mat_m * vec4(a_position, 1.0);
    v_screen_texcoord = u_mat_p * u_mat_v * position;
    gl_Position = v_screen_texcoord;
}
 );

const char* shader_omni_light_frag = string_shader
(
 uniform mat4 u_mat_i_vp;
 uniform vec3 u_light_position;
 uniform float u_light_radius;
 
 uniform float u_f32_camera_near;
 uniform float u_f32_camera_far;
 
 uniform sampler2D sampler_01;
 uniform sampler2D sampler_02;
 
#if defined(__OPENGL_30__)
 
 in vec4 v_screen_texcoord;
 
#else
 
 varying vec4 v_screen_texcoord;
 
#endif
 
 float get_depth(in vec2 texcoord)
{
    float depth = texture2D(sampler_01, texcoord).r;
    return (2.0 * u_f32_camera_near) / (u_f32_camera_near + u_f32_camera_far - depth * (u_f32_camera_far - u_f32_camera_near));
}
 
 void main()
{
    vec4 screen_position = v_screen_texcoord;
    screen_position.xy /= screen_position.w;
    
    vec2 screen_texcoord = 0.5 * (vec2(screen_position.x, screen_position.y) + 1.0);
    screen_texcoord -= vec2(0.5 / 1024.0, 0.5 / 768.0);
    
    vec3 normal = texture2D(sampler_02, screen_texcoord).rgb * 2.0 - 1.0;
    //normal = 2.0 * normal - 1.0;
    
    vec4 position;
    position.xy = v_screen_texcoord.xy;
    position.z = texture2D(sampler_02, screen_texcoord).a;
    position.w = 1.0;
    
    position = u_mat_i_vp * position;
    position /= position.w;
    
    vec3 light_direction = u_light_position - position.xyz;
    float attenuation = clamp(1.0 - length(light_direction) / u_light_radius, 0.0, 1.0);
    light_direction = normalize(light_direction);

    gl_FragColor = vec4(attenuation * vec3(clamp(dot(normal, light_direction), 0.0, 1.0)), 1.0);
}
 );
