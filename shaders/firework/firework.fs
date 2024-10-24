#version 330 core

out vec4 FragColor;
uniform vec3 fireworkColor;
void main(){
    FragColor = vec4(fireworkColor, 1.0);
}