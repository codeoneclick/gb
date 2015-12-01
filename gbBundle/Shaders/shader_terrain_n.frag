
#if defined(__OPENGL_30__)

in vec3 v_position;
in vec2 v_texcoord;
in vec3 v_normal;
in vec3 v_tangent;

#else

varying vec3 v_position;
varying vec2 v_texcoord;
varying vec3 v_normal;
varying vec3 v_tangent;

#endif

uniform vec3 u_vec_camera_position;

uniform sampler2D sampler_02;

const float k_scale = 0.0025;
const float k_min_steps_num = 8;
const float k_max_steps_num = 16;

vec2 get_pom_texcoord(in sampler2D sampler, in vec2 texcoord, in vec3 camera_direction_ts, in vec3 normal_ts)
{
    float current_height = 1.0;
    vec2 current_texcoord = v_texcoord;
    
    float steps_num = mix(k_min_steps_num, k_max_steps_num, dot(camera_direction_ts, normal_ts));
    float step_size = 1.0 / steps_num;
    vec2 step_offset = camera_direction_ts.xy * k_scale / ( steps_num * camera_direction_ts.z );
    
    float height_color = (1.0 - texture2D(sampler, current_texcoord).a);
    
    while (height_color < current_height)
    {
        current_height -= step_size;
        current_texcoord += step_offset;
        height_color = (1.0 - texture2D(sampler, current_texcoord).a);
    }
    
    vec2 previous_texcoord = current_texcoord - step_offset;
    float previous_height = (1.0 - texture2D (sampler, previous_texcoord).a) - (current_height + step_size);
    current_height = height_color - current_height;
    float weight = current_height / (current_height - previous_height);
    
    current_texcoord = weight * previous_texcoord + (1.0 - weight) * current_texcoord;
    return current_texcoord;
}

void main(void)
{
    mat3 mat_tangent_space = mat3(v_tangent, cross(-v_normal, v_tangent), v_normal);
    vec3 camera_direction_ts = (v_position - u_vec_camera_position) * mat_tangent_space;
    vec3 normal_ts = v_normal * mat_tangent_space;
    vec2 texcoord = get_pom_texcoord(sampler_02, v_texcoord, normalize(camera_direction_ts), normalize(normal_ts));
    
    vec4 color = texture2D(sampler_02, texcoord);
    
    normal_ts = normalize(mat_tangent_space * (color.xyz * 2.0 - 1.0));
    gl_FragColor = vec4(normal_ts * 0.5 + 0.5, color.a);
}
