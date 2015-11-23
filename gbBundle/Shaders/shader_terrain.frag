#if defined(__OPENGL_30__)

in vec3 v_position;
in vec2 v_texcoord;
in vec3 v_normal;
in vec3 v_tangent;
in vec4 v_shadow_parameters;

#else

varying vec3 v_position;
varying vec2 v_texcoord;
varying vec3 v_normal;
varying vec3 v_tangent;
varying vec4 v_shadow_parameters;

#endif

uniform vec3 u_vec_camera_position;

uniform sampler2D sampler_01;
uniform sampler2D sampler_02;
uniform sampler2DShadow sampler_03;

float scale = 0.005;
float bias = 0.0025;

void main(void)
{
    mat3 mat_tbn = mat3(v_tangent, cross(-v_normal, v_tangent), v_normal);
    vec3 camera_direction = normalize((v_position - u_vec_camera_position) * mat_tbn);
    
    vec4 color = texture2D(sampler_02, v_texcoord);
    
    float displace = color.a * scale - bias;
    vec2 offset = (displace * camera_direction.xy) / camera_direction.z;
    
    color = texture2D(sampler_01, v_texcoord + offset);
    
#if defined(__IOS__)
    
    float shadow = shadow2DProjEXT(sampler_03, v_shadow_parameters);
    
#else
    
    float shadow = textureProj(sampler_03, v_shadow_parameters);
    
#endif
    
    color.rgb *= clamp(shadow, 0.33, 1.0);
    
    gl_FragColor = color;
}