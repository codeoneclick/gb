
#if defined(__OPENGL_30__)

in vec3 v_normal;

#else

varying vec3 v_normal;

#endif

void main(void)
{
#define __ENCODE_NORMAL_SPHEREMAP__
#if defined(__ENCODE_NORMAL_XYZ__)
    
    
    gl_FragColor = vec4(v_normal * 0.5 + 0.5, 1.0);
    
#elif defined(__ENCODE_NORMAL_SPHEREMAP__)
    
    float p = sqrt(v_normal.z * 8.0 + 8.0);
    
    gl_FragColor = vec4(v_normal.xy / p + 0.5, 0.0, 1.0);
    
#endif

}
