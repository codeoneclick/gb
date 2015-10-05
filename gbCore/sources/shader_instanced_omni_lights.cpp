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
 out vec4 v_transform_parameters;
 out vec4 v_light_color;
 
#else
 
 varying vec4 v_screen_position;
 varying vec4 v_transform_parameters;
 varying vec4 v_light_color;
 
#endif
 
 uniform mat4 u_mat_v;
 uniform mat4 u_mat_p;
 uniform vec4 u_transform_parameters[32];
 uniform vec4 u_lights_colors[32];
 
 mat4 mat_m = mat4(0.0, 0.0, 0.0, 0.0,
                   0.0, 0.0, 0.0, 0.0,
                   0.0, 0.0, 0.0, 0.0,
                   0.0, 0.0, 0.0, 1.0);
 
 void main(void)
{
    mat_m[0][0] = u_transform_parameters[gl_InstanceID].w;
    mat_m[1][1] = u_transform_parameters[gl_InstanceID].w;
    mat_m[2][2] = u_transform_parameters[gl_InstanceID].w;
    mat_m[3][0] = u_transform_parameters[gl_InstanceID].x;
    mat_m[3][1] = u_transform_parameters[gl_InstanceID].y;
    mat_m[3][2] = u_transform_parameters[gl_InstanceID].z;

    vec4 position = mat_m * vec4(a_position, 1.0);
    gl_Position = u_mat_p * u_mat_v * position;
    
    v_screen_position = gl_Position;
    v_transform_parameters = u_transform_parameters[gl_InstanceID];
    v_light_color = u_lights_colors[gl_InstanceID];
}
 );

const char* shader_instanced_omni_light_frag = string_shader
(
 uniform mat4 u_mat_i_vp;
 uniform vec3 u_vec_camera_position;
 
 uniform sampler2D sampler_01;
 uniform sampler2D sampler_02;
 
#if defined(__OPENGL_30__)
 
 in vec4 v_screen_position;
 in vec4 v_transform_parameters;
 in vec4 v_light_color;
 
#else
 
 varying vec4 v_screen_position;
 varying vec4 v_transform_parameters;
 varying vec4 v_light_color;
 
#endif
 
 void main()
{
    vec4 screen_position = v_screen_position;
    screen_position.xy /= screen_position.w;
    vec2 texcoord = 0.5 * (screen_position.xy + 1.0);

    vec3 normal = texture2D(sampler_01, texcoord).rgb * 2.0 - 1.0;
    float depth = texture2D(sampler_02, texcoord).r;
    
    vec4 position = vec4(texcoord * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
    position = u_mat_i_vp * position;
    position.xyz = position.xyz / position.w;
    
    vec3 light_direction = v_transform_parameters.xyz - position.xyz;
    float attenuation = 1.0 - length(light_direction) / v_transform_parameters.w;
    light_direction = normalize(light_direction);
    
    vec3 diffuse = vec3(clamp(dot(normal, light_direction), 0.0, 1.0));
    
#if defined(__SPECULAR__)
    
    vec3 camera_direction = normalize(u_vec_camera_position - position.xyz);
    vec3 light_reflect = normalize(2.0 * diffuse * normal - light_direction);
    float specular = pow(clamp(dot(light_reflect, camera_direction), 0.0, 1.0), 32.0);
    
#else
    
    float specular = 0.0;
    
#endif
    
    gl_FragColor = vec4(attenuation * diffuse + specular * v_light_color.rgb, 1.0) * v_light_color;
}
 );

