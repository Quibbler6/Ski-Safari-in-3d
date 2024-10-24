#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;  // 注意是三维纹理坐标

uniform mat4 projection;
uniform mat4 view;

void main(){
    TexCoords = aPos;  //使用顶点位置作为方向向量来映射纹理
    gl_Position = projection * view * vec4(aPos, 1.0);
}  