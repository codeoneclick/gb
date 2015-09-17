
#if defined(__OPENGL_30__)

in vec2 v_depth;
in vec3 v_normal;

#else

varying vec2 v_depth;
varying vec3 v_normal;

#endif

void main(void)
{
    gl_FragColor = vec4(v_normal, v_depth.x / v_depth.y);
}
