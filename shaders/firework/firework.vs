#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float r_in;
layout (location = 2) in vec3 v_in;
layout (location = 3) in float birth_t_in; 
out vec3 v;
out float r;
out float t_birth;

void main(){
    gl_Position = vec4(aPos,1);
    v = v_in;
    r = r_in;
    t_birth = birth_t_in;
}