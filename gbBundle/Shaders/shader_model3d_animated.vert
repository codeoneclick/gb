
#if defined(__OPENGL_30__)

out vec2 v_texcoord;

#else

varying vec2 v_texcoord;

#endif

uniform mat4 u_mat_m;
uniform mat4 u_mat_v;
uniform mat4 u_mat_p;
uniform mat4 u_mat_bones[32];
uniform vec4 u_vec_clip;

void main(void)
{
    vec4 position = vec4(a_position, 1.0);
    vec4 weights = a_extra / 255.0;
    
    int index = 0;
    vec4 bone_position = vec4(0.0);
    
    for(int i = 0; i < 4; i++)
    {
        index = int(a_color[i]);
        bone_position += u_mat_bones[index] * position * weights[i];
    }
    position = bone_position;

    position = u_mat_m * position;
    gl_Position = u_mat_p * u_mat_v * position;
    gl_ClipDistance[0] = dot(position.xyz, u_vec_clip.xyz);
    v_texcoord = a_texcoord;
}