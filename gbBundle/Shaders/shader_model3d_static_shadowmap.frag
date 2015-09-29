#if defined(__OPENGL_30__)

in vec2 v_texcoord;
in vec4 v_shadow_parameters;

#else

varying vec2 v_texcoord;
varying vec4 v_shadow_parameters;

#endif

uniform sampler2D sampler_01;
uniform sampler2D sampler_02;

uniform float u_f32_camera_near;
uniform float u_f32_camera_far;

float get_shadow_map_pass_depth(in vec2 texcoord)
{
    float depth = texture2D(sampler_02, texcoord).x;
    return (2.0 * u_f32_camera_near) / (u_f32_camera_near + u_f32_camera_far - depth * (u_f32_camera_far - u_f32_camera_near));
}

float get_current_depth(in float z)
{
    float depth = z;
    depth = (2.0 * u_f32_camera_near) / (u_f32_camera_near + u_f32_camera_far - depth * (u_f32_camera_far - u_f32_camera_near));
    return depth;
}

void main(void)
{
    vec2 shadow_texcoord = v_shadow_parameters.xy / v_shadow_parameters.w;
    float z = v_shadow_parameters.z / v_shadow_parameters.w;
    float shadow = step(get_current_depth(z), get_shadow_map_pass_depth(shadow_texcoord));
    
    vec4 color = texture2D(sampler_01, v_texcoord);
    
    color.rgb *= shadow;
    gl_FragColor = color;
}