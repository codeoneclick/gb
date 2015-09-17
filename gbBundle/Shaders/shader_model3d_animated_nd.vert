
#if defined(__OPENGL_30__)

out vec2 v_depth;
out vec3 v_normal;

#else

varying vec2 v_depth;
varying vec3 v_normal;

#endif

uniform mat4 u_mat_m;
uniform mat4 u_mat_bones[32];
uniform vec4 u_vec_clip;
uniform mat4 u_mat_v;
uniform mat4 u_mat_p;

void main(void)
{
    vec4 position = vec4(a_position, 1.0);
    vec4 weights = a_extra / 255.0;
    vec4 normal = a_normal;
    
    int index = 0;
    vec4 bone_position = vec4(0.0);
    vec4 bone_normal = vec4(0.0);
    for(int i = 0; i < 4; i++)
    {
        index = int(a_color[i]);
        bone_position += u_mat_bones[index] * position * weights[i];
        bone_normal += u_mat_bones[index] * normal * weights[i];
    }
    position = bone_position;
    normal = bone_normal;
    
    position = u_mat_m * position;
    
    gl_Position = u_mat_p * u_mat_v * position;
    gl_ClipDistance[0] = dot(position.xyz, u_vec_clip.xyz);
    
    v_depth.x = position.z;
    v_depth.y = position.w;
    
    v_normal = normalize(normal).xyz;
    v_normal = (v_normal + 1.0) / 2.0;
}
