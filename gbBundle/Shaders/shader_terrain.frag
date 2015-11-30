#if defined(__OPENGL_30__)

in vec3 v_position;
in vec2 v_texcoord;
in vec3 v_camera_direction_tbn;
in vec4 v_shadow_parameters;

#else

varying vec3 v_position;
varying vec2 v_texcoord;
varying vec3 v_camera_direction_tbn;
varying vec4 v_shadow_parameters;

#endif

uniform sampler2D sampler_01;
uniform sampler2D sampler_02;
uniform sampler2DShadow sampler_03;

float scale = 0.0025;
float bias = 0.0025;

void main(void)
{
    const float num_steps = 8.0;
    
    float step_index = 1.0 / num_steps;
    vec2 offset = v_camera_direction_tbn.xy * scale / ( num_steps * v_camera_direction_tbn.z );
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
    
    vec4 color = texture2D(sampler_01, texcoord);
    
#if defined(__IOS__)
    
    float shadow = shadow2DProjEXT(sampler_03, v_shadow_parameters);
    
#else
    
    float shadow = textureProj(sampler_03, v_shadow_parameters);
    
#endif
    
    color.rgb *= clamp(shadow, 0.33, 1.0);
    
    gl_FragColor = color;
}