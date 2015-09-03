
#if defined(__OPENGL_30__)

in vec4 v_position;

#else

varying vec4 v_position;

#endif

const vec2 camera_range = vec2(0.01, 128.0);

float get_depth(in float z)
{
    float depth = z;
    float near_z = camera_range.x;
    float far_z = camera_range.y;
    depth = (2.0 * near_z) / (near_z + far_z - depth * (far_z - near_z));
    return depth;
}

void main(void)
{
    float z = v_position.z / v_position.w;
    gl_FragColor = vec4(1.0);
}
