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
 
 uniform vec3 u_light_position;
 
#if defined(__OPENGL_30__)
 
 out vec4 v_screen_position;
 out vec3 v_light_position;
 
#else
 
 varying vec4 v_screen_position;
 varying vec3 v_light_position;
 
#endif
 
 uniform mat4 u_mat_m;
 uniform mat4 u_mat_v;
 uniform mat4 u_mat_p;
 
 void main(void)
{
    vec4 position = u_mat_m * vec4(a_position, 1.0);
    gl_Position = u_mat_p * u_mat_v * position;
    v_screen_position = gl_Position;
    v_light_position = (u_mat_v * u_mat_m * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
}
 );

const char* shader_omni_light_frag = string_shader
(
 uniform float u_light_radius;
 uniform vec3 u_light_position;
 
 uniform float u_f32_camera_near;
 uniform float u_f32_camera_far;
 
 uniform mat4 u_mat_i_vp;
 
 uniform sampler2D sampler_01;
 uniform sampler2D sampler_02;
 
#if defined(__OPENGL_30__)
 
 in vec4 v_screen_position;
 in vec3 v_light_position;
 
#else
 
 varying vec4 v_screen_position;
 varying vec3 v_light_position;
 
#endif
 
 float get_depth(in vec2 texcoord)
{
    float depth = texture2D(sampler_01, texcoord).x;
    return (2.0 * u_f32_camera_near) / (u_f32_camera_near + u_f32_camera_far - depth * (u_f32_camera_far - u_f32_camera_near));
}
 
 vec3 get_uv_to_eye(in vec2 texcoord, in float z)
{
    float fovy = radians(45.0);
    float aspect = 1024.0 / 768.0;
    float invFocalLenX   = tan(fovy * 0.5) * aspect;
    float invFocalLenY   = tan(fovy * 0.5);
    texcoord = (texcoord * vec2(2.0, 2.0) - vec2(1.0, 1.0));
    return vec3(texcoord * vec2(invFocalLenX, invFocalLenY) * z, z);
}
 
 vec3 get_position(in vec2 texcoord )
{
    float depth = get_depth(texcoord);
    vec3 position = get_uv_to_eye(texcoord, depth);
    position.z *= -1.0;
    return (u_mat_i_vp * vec4(position, 1.0)).xyz;
}
 
 void main()
{
    vec4 screen_position = v_screen_position;
    screen_position.xy /= screen_position.w;
    
    vec2 tex = gl_FragCoord.xy / vec2(1024.0, 768.0);
    
    vec2 texcoord = 0.5 * (screen_position.xy + 1.0);
    texcoord -= vec2(0.5 / 1024.0, 0.5 / 768.0);
    
    vec4 normal_color = texture2D(sampler_02, texcoord);
    vec4 position_color = texture2D(sampler_01, texcoord);
    
    vec3 normal = normal_color.rgb * 2.0 - 1.0;
    vec4 position = vec4(texcoord.x * 2.0 - 1.0, texcoord.y * 2.0 - 1.0, position_color.r * 2.0 - 1.0, 1.0);
    position = u_mat_i_vp * position;
    position.xyz = position.xyz / position.w;
    
    vec3 light_direction = u_light_position - position.xyz;

    //
    float a = radians(45.0);
    float attenuation = 1.0 - length(light_direction) / u_light_radius;
    //float attenuation = 1.0 / (1.0 + 0.2 * pow(length(light_direction), 2));

    light_direction = normalize(light_direction);

    if(normal_color.a != 0.0)
    {
        gl_FragColor = vec4(vec3(attenuation * clamp(dot(normal, light_direction), 0.0, 1.0)), 1.0) * vec4(vec3(1.0, 0.5, 0.5), 1.0);
    }
    else
    {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
    //gl_FragColor = vec4(position.xyz, 1.0);
}
 );
