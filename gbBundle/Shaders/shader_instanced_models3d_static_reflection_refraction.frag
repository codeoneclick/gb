#if defined(__OPENGL_30__)

in vec2 v_texcoord;

#else

varying vec2 v_texcoord;

#endif

uniform sampler2D sampler_01;
uniform sampler2D sampler_02;

void main(void)
{
    vec4 color = texture2D(sampler_01, v_texcoord);
    gl_FragColor = color;
}