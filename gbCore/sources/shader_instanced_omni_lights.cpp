//
//  shader_instanced_omni_lights.cpp
//  gbCore
//
//  Created by sergey.sergeev on 10/1/15.
//  Copyright (c) 2015 sergey.sergeev. All rights reserved.
//

#include "main_headers.h"

const char* shader_instanced_omni_light_vert = string_shader
(
 
#if defined(__OPENGL_30__)
 
 out vec4 v_screen_position;
 out vec4 v_parameters;
 
#else
 
 varying vec4 v_screen_position;
 varying vec4 v_parameters;
 
#endif
 
 uniform mat4 u_mat_v;
 uniform mat4 u_mat_p;
 uniform mat4 u_matrices_m[8];
 uniform vec4 u_parameters[8];
 
 void main(void)
{
    vec4 position = u_matrices_m[gl_InstanceID] * vec4(a_position, 1.0);
    gl_Position = u_mat_p * u_mat_v * position;
    
    v_screen_position = gl_Position;
    v_parameters = u_parameters[gl_InstanceID];
}
 );

const char* shader_instanced_omni_light_frag = string_shader
(
 uniform mat4 u_mat_i_vp;
 
 uniform sampler2D sampler_01;
 uniform sampler2D sampler_02;
 
#if defined(__OPENGL_30__)
 
 in vec4 v_screen_position;
 in vec4 v_parameters;
 
#else
 
 varying vec4 v_screen_position;
 varying vec4 v_parameters;
 
#endif
 
 void main()
{
    vec4 screen_position = v_screen_position;
    screen_position.xy /= screen_position.w;
    
    vec2 texcoord = 0.5 * (screen_position.xy + 1.0);
    texcoord -= vec2(0.5 / 1024.0, 0.5 / 768.0);
    
    vec3 normal = texture2D(sampler_01, texcoord).rgb * 2.0 - 1.0;
    float depth = texture2D(sampler_02, texcoord).r;
    
    vec4 position = vec4(texcoord * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
    position = u_mat_i_vp * position;
    position.xyz = position.xyz / position.w;
    
    vec3 light_direction = v_parameters.xyz - position.xyz;
    float attenuation = 1.0 - length(light_direction) / v_parameters.w;
    light_direction = normalize(light_direction);
    
    gl_FragColor = vec4(vec3(attenuation * clamp(dot(normal, light_direction), 0.0, 1.0)), 1.0) * vec4(vec3(1.0, 0.5, 0.5), 1.0);
}
 );

