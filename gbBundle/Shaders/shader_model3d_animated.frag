#if defined(__OPENGL_30__)

in vec2 v_texcoord;
in vec3 v_normal;
in vec3 v_light_direction;

#else

varying vec2 v_texcoord;
varying vec3 v_normal;
varying vec3 v_light_direction;

#endif

uniform sampler2D sampler_01;

void main(void)
{
    float diffuse_intensity = clamp(dot(v_normal, v_light_direction), 0.0, 1.0);
    vec4 diffuse_color = texture2D(sampler_01, v_texcoord);
    diffuse_color.rbg *= diffuse_intensity;
    gl_FragColor = diffuse_color;
}