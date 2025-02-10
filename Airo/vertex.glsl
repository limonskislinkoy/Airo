#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aColor; // the color variable has attribute position 1
  
out vec3 ourColor; // output a color to the fragment shader
out vec2 TexCoord;
uniform float gc_time;
uniform vec3 camera_pos;
uniform mat4 transmat_yaw;
uniform mat4 transmat_pitch;
uniform mat4 transmat_roll;

uniform mat4 transmat_rotarion_of_object;

uniform mat4 projection;

uniform vec3 obj_position;


void main()
{
    vec3 trans_pos=aPos;
    TexCoord = aTex;
    //trans_pos.z-=0.001;
    //trans_pos.x+=sin(gc_time);
    //trans_pos.y+=cos(gc_time/3.1);
    //trans_pos.z-=sin(gc_time)*10;
    trans_pos = vec3(transmat_rotarion_of_object * vec4(trans_pos,1.0));
    gl_Position = projection*transmat_yaw*transmat_pitch*vec4(trans_pos-camera_pos+obj_position, 1.0);
    
    ourColor = aColor; // set ourColor to the input color we got from the vertex data
}