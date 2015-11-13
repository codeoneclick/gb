
#if defined(__OPENGL_30__)

in vec2 v_texcoord;

#else

varying vec2 v_texcoord;

#endif

uniform sampler2D sampler_02;

void main(void)
{
    gl_FragColor = texture2D(sampler_02, v_texcoord);
}
