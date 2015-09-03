#if defined(__OPENGL_30__)

in vec2 v_texcoord;
in vec4 v_shadow_parameters;

#else

varying vec2 v_texcoord;
varying vec4 v_shadow_parameters;

#endif

uniform sampler2D sampler_01;
uniform sampler2D sampler_02;

const vec2 camera_range = vec2(0.1, 1024.0);

float get_shadow_map_pass_depth(in vec2 texcoord)
{
    float near_z = camera_range.x;
    float far_z = camera_range.y;
    float depth = texture2D(sampler_02, texcoord).x;
    return (2.0 * near_z) / (near_z + far_z - depth * (far_z - near_z));
}

float get_current_depth(in float z)
{
    float depth = z;
    float near_z = camera_range.x;
    float far_z = camera_range.y;
    depth = (2.0 * near_z) / (near_z + far_z - depth * (far_z - near_z));
    return depth;
}

void main(void)
{
    vec2 shadow_texcoord = v_shadow_parameters.st / v_shadow_parameters.w;
    float z = v_shadow_parameters.z / v_shadow_parameters.w;
    float shadow = max(step(get_current_depth(z), get_shadow_map_pass_depth(shadow_texcoord)), 0.5);
    
    vec4 color = texture2D(sampler_01, v_texcoord);
    
    color.rgb *= shadow;
    gl_FragColor = color;
}