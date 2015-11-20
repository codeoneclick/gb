
#if defined(__OPENGL_30__)

out vec3 v_position;
out vec2 v_texcoord;
out vec3 v_normal;
out vec3 v_tangent;
out vec4 v_shadow_parameters;

#else

varying vec3 v_position;
varying vec2 v_texcoord;
varying vec3 v_normal;
varying vec3 v_tangent;
varying vec4 v_shadow_parameters;

#endif

uniform vec4 u_vec_clip;
uniform mat4 u_mat_m;
uniform mat4 u_mat_v;
uniform mat4 u_mat_p;

uniform mat4 u_mat_global_light_p;
uniform mat4 u_mat_global_light_v;

const mat4 bias_matrix = mat4(0.5, 0.0, 0.0, 0.0,
                              0.0, 0.5, 0.0, 0.0,
                              0.0, 0.0, 0.5, 0.0,
                              0.5, 0.5, 0.5, 1.0);

void main(void)
{
    vec4 position = vec4(a_position, 1.0);
    position = u_mat_m * position;
    gl_Position = u_mat_p * u_mat_v * position;
    gl_ClipDistance[0] = dot(position, u_vec_clip);
    
    v_position = position.xyz;
    v_texcoord = a_texcoord;
    v_normal = normalize(u_mat_m * a_normal).xyz;
    v_tangent = normalize(u_mat_m * a_tangent).xyz;
    
    v_shadow_parameters = bias_matrix * u_mat_global_light_p * u_mat_global_light_v * position;
}