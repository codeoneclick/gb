
#if defined(__OPENGL_30__)

in vec4 v_position;

#else

varying vec4 v_position;

#endif

void main(void)
{
    gl_FragColor = vec4(v_position.xyz, 0.0);
}
