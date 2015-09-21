
#if defined(__OPENGL_30__)

out vec4 v_position;

#else

varying vec4 v_position;

#endif

uniform mat4 u_mat_m;
uniform mat4 u_mat_bones[32];
uniform vec4 u_vec_clip;
uniform mat4 u_mat_v;
uniform mat4 u_mat_p;

void main(void)
{
    vec4 position = vec4(a_position, 1.0);
    position = u_mat_m * position;
    v_position = position;
    gl_Position = u_mat_p * u_mat_v * position;
    gl_ClipDistance[0] = dot(position.xyz, u_vec_clip.xyz);
}
