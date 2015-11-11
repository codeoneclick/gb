#if defined(__OPENGL_30__)

out vec3 v_normal;
out vec3 v_tangent;

#else

varying vec3 v_normal;
varying vec3 v_tangent;

#endif

uniform vec4 u_vec_clip;
uniform mat4 u_mat_m;
uniform mat4 u_mat_v;
uniform mat4 u_mat_p;

void main(void)
{
    vec4 position = vec4(a_position, 1.0);
    position = u_mat_m * position;
    gl_Position = u_mat_p * u_mat_v * position;
    gl_ClipDistance[0] = dot(position.xyz, u_vec_clip.xyz);
    
    v_normal = normalize(u_mat_m * a_normal).xyz;
    v_tangent = normalize(u_mat_m * a_tangent).xyz;
}