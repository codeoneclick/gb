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
 
 out vec4 v_screen_position;
 
#else
 
 varying vec4 v_screen_position;
 
#endif
 
 uniform mat4 u_mat_m;
 uniform mat4 u_mat_v;
 uniform mat4 u_mat_p;
 
 void main(void)
{
    vec4 position = u_mat_m * vec4(a_position, 1.0);
    gl_Position = u_mat_p * u_mat_v * position;
    
    v_screen_position = gl_Position;
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
 
 in vec4 v_screen_position;
 
#else
 
 varying vec4 v_screen_position;
 
#endif
 
 void main()
{
    vec4 screen_position = v_screen_position;
    screen_position.xy /= screen_position.w;
    
    vec2 texcoord = 0.5 * (screen_position.xy + 1.0);
    texcoord -= vec2(0.5 / 1024.0, 0.5 / 768.0);
    
    vec4 normal_color = texture2D(sampler_02, texcoord);
    vec4 position_color = texture2D(sampler_01, texcoord);
    
    vec3 normal = normal_color.rgb * 2.0 - 1.0;
    vec3 position = position_color.rgb;
    
    vec3 light_direction = u_light_position - position_color.xyz;
    
    float attenuation = clamp(length(light_direction) / u_light_radius, 0.0, 1.0);
    light_direction = normalize(light_direction);

    if(normal_color.a != 0.0)
    {
        gl_FragColor = vec4(attenuation * vec3(clamp(dot(normal, light_direction), 0.0, 1.0)), 1.0);
    }
    else
    {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}
 );
