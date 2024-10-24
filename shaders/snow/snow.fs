#version 330 core

out vec4 FragColor;
uniform vec3 snowColor;
void main(){
    FragColor = vec4(snowColor, 1.0);
}