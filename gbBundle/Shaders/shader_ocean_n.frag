
#if defined(__OPENGL_30__)

in vec2 v_texcoord;
in vec4 v_texcoord_proj;
in vec2 v_texcoord_displace_01;
in vec2 v_texcoord_displace_02;

in vec3 v_eye_position_ws;
in vec3 v_vertex_position_ws;

in float v_fog_distance;

#else

varying vec2 v_texcoord;
varying vec4 v_texcoord_proj;
varying vec2 v_texcoord_displace_01;
varying vec2 v_texcoord_displace_02;

varying vec3 v_eye_position_ws;
varying vec3 v_vertex_position_ws;

varying float v_fog_distance;

#endif

uniform sampler2D sampler_01;
uniform sampler2D sampler_02;
uniform sampler2D sampler_04;

const vec2 k_perturbation_factor = vec2(0.025, 0.025);

const float k_0 = 0.0;
const float k_1 = 1.0;
const float k_2 = 2.0;
const float k_05 = 0.5;
const float k_025 = 0.25;
const float k_033 = 0.33;

void main()
{
    vec3 normal = texture2D(sampler_02, v_texcoord_displace_01).rgb;
    normal += texture2D(sampler_02, v_texcoord_displace_02).rgb;
    normal -= 1.0;
    
    vec2 texcoord_proj = v_texcoord_proj.xy;
    texcoord_proj = k_05 + k_05 * texcoord_proj / v_texcoord_proj.w * vec2(-k_1, k_1);
    
    float deep = pow(clamp(1.0 - texture2D(sampler_04, v_texcoord).r, 0.0, 1.0), 2.0);
    vec2 perturbation_intensity = k_perturbation_factor * normal.xy * deep;
    vec2 perturbated_texcoord = texcoord_proj + perturbation_intensity;
    
    vec4 color = texture2D(sampler_01, vec2(k_05 + (k_05 - perturbated_texcoord.x), perturbated_texcoord.y));
    color.a = 1.0;
    
    gl_FragColor = color;
}
