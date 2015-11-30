
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

float scale = 0.0025;
float bias = 0.0025;

void main(void)
{
    mat3 mat_tbn = mat3(v_tangent, cross(-v_normal, v_tangent), v_normal);
    vec3 camera_direction = normalize((v_position - u_vec_camera_position) * mat_tbn);
    
    const float num_steps = 8.0;
    
    float step_index = 1.0 / num_steps;
    vec2 offset = camera_direction.xy * scale / ( num_steps * camera_direction.z );
    float height = 1.0;
    vec2 texcoord = v_texcoord;
    float height_color = (1.0 - texture2D(sampler_02, texcoord).a);
    
    while (height_color < height)
    {
        height -= step_index;
        texcoord += offset;
        height_color = (1.0 - texture2D (sampler_02, texcoord).a);
    }
    
    vec2 previous_texcoord = texcoord - offset;
    float previous_height = (1.0 - texture2D (sampler_02, previous_texcoord).a) - (height + step_index);
    float current_height = height_color - height;
    float weight = current_height / (current_height - previous_height);
    
    texcoord = weight * previous_texcoord + (1.0 - weight) * texcoord;
    
    //vec3 camera_direction = normalize((v_position - u_vec_camera_position) * mat_tbn);
    
    //vec4 color = texture2D(sampler_02, v_texcoord);
    //vec2 offset = color.a * scale * camera_direction.xy;
    
    vec4 color = texture2D(sampler_02, texcoord);
    
    vec3 normal_ts = normalize(mat_tbn * (color.xyz * 2.0 - 1.0));
    gl_FragColor = vec4(normal_ts * 0.5 + 0.5, color.a);
}
