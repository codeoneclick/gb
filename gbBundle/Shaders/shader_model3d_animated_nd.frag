
#if defined(__OPENGL_30__)

in vec2 v_depth;

#else

varying vec2 v_depth;

#endif

void main(void)
{
    gl_FragColor = vec4(0.0);
}
