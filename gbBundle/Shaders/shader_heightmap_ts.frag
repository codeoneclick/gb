
#if defined(__OPENGL_30__)

in vec3 v_normal;
in vec3 v_tangent;

#else

varying vec3 v_normal;
varying vec3 v_tangent;

#endif

void main(void)
{
    float normal_z = sqrt(v_normal.z * 8.0 + 8.0);
    float tangent_z = sqrt(v_tangent.z * 8.0 + 8.0);
    
    gl_FragColor = vec4(v_normal.xy / normal_z + 0.5, v_tangent.xy / tangent_z + 0.5);
}
