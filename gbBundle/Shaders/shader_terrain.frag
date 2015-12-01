#if defined(__OPENGL_30__)

in vec3 v_position;
in vec2 v_texcoord;
in vec3 v_camera_direction_ts;
in vec3 v_normal_ts;
in vec4 v_shadow_parameters;

#else

varying vec3 v_position;
varying vec2 v_texcoord;
varying vec3 v_camera_direction_ts;
varying vec3 v_normal_ts;
varying vec4 v_shadow_parameters;

#endif

uniform sampler2D sampler_01;
uniform sampler2D sampler_02;
uniform sampler2DShadow sampler_03;

const float k_scale = 0.0025;
const float k_min_steps_num = 8;
const float k_max_steps_num = 16;

vec2 get_pom_texcoord(in sampler2D sampler, in vec2 texcoord, in vec3 camera_direction_ts, in vec3 normal_ts)
{
    float current_height = 0.0;
    float current_bound = 1.0;
    float previous_height = 1.0;
    vec2 current_texcoord = v_texcoord;
    
    float steps_num = mix(k_min_steps_num, k_max_steps_num, dot(camera_direction_ts, normal_ts));
    float step_size = 1.0 / steps_num;
    vec2 step_offset = camera_direction_ts.xy * k_scale / ( steps_num * camera_direction_ts.z );
    
    float height_color = (1.0 - texture2D(sampler, current_texcoord).a);
    
    vec2 pt1 = vec2(0.0);
    vec2 pt2 = vec2(0.0);
    
    float step_index = 0;
    while(step_index < steps_num)
    {
        current_texcoord -= step_offset;
        
        current_height = (1.0 - texture2D(sampler, current_texcoord).a);
        
        current_bound -= step_size;
        
        if (current_height > current_bound)
        {
            pt1 = vec2(current_bound, current_height);
            pt2 = vec2(current_bound + step_size, previous_height);
            
            step_index = steps_num + 1.0;
        }
        else
        {
            step_index++;
            previous_height = current_height;
        }
    }
    
    float delta2 = pt2.x - pt2.y;
    float delta1 = pt1.x - pt1.y;
    float parallax_amount = (pt1.x * delta2 - pt2.x * delta1) / (delta2 - delta1);
    
    vec2 parallax_offset = step_offset * (1.0 - parallax_amount);
    current_texcoord = v_texcoord + parallax_offset;
    
    /*vec2 previous_texcoord = current_texcoord;
    float previous_height_color = height_color;
    
    while (height_color < current_height)
    {
        previous_texcoord = current_texcoord;
        previous_height_color = height_color;
        current_height -= step_size;
        current_texcoord += step_offset;
        height_color = (1.0 - texture2D(sampler, current_texcoord).a);
    }
    
    float previous_height = (1.0 - texture2D (sampler, previous_texcoord).a) - (current_height + step_size);
    current_height = height_color - current_height;
    float weight = current_height / (current_height - previous_height);
    
    current_texcoord = weight * previous_texcoord + (1.0 - weight) * current_texcoord;*/
    return current_texcoord;
}

void main()
{
    vec2 texcoord = get_pom_texcoord(sampler_02, v_texcoord, normalize(v_camera_direction_ts), normalize(v_normal_ts));
    vec4 color = texture2D(sampler_01, texcoord);
    
#if defined(__IOS__)
    
    float shadow = shadow2DProjEXT(sampler_03, v_shadow_parameters);
    
#else
    
    float shadow = textureProj(sampler_03, v_shadow_parameters);
    
#endif
    
    color.rgb *= clamp(shadow, 0.33, 1.0);
    
    gl_FragColor = color;
}