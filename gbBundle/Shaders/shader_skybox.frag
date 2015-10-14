#if defined(__OPENGL_30__)

in vec3 v_texcoord;

#else

varying vec3 v_texcoord;

#endif

vec4 fog_color = vec4(0.0, 0.0, 0.0, 1.0);

uniform samplerCube sampler_01;

void main(void)
{
    vec4 color = textureCube(sampler_01, v_texcoord);
    /*float height = v_texcoord.y * (1.0 / 0.4);
    float fog_blend_intensity = 1.0 - clamp(height, 0.0, 1.0);
    color = mix(color, fog_color, fog_blend_intensity);*/
    gl_FragColor = color;
}