#version 330 core
layout (location = 0) in vec3 position;

layout (location = 1) in vec3 color_vertex;

layout (location = 2) in vec3 normal;


out vec3 out_color_vertex;
out vec3 Normal;
out vec3 Frag_pos;


uniform float gc_time;

uniform vec3 transmat_offset;
uniform mat4 transmat_rotate_roll;
uniform mat4 transmat_rotate_pitch;
uniform mat4 transmat_rotate_yaw;

uniform mat4 projection;

void main()
{
   
    out_color_vertex = color_vertex;
    Frag_pos = position;
    Normal=normal;
   
    vec3 newpos= vec3(vec4(position+(gc_time/1.0f)*normal/100.f,1.0f));

    gl_Position = projection*(transmat_rotate_pitch*transmat_rotate_yaw*transmat_rotate_roll*vec4(position-transmat_offset,1.0));
  
    
}