#define GLEW_STATIC
#include "sphere.h"

Sphere::Sphere(float radius,glm::vec3 Color){
    for (int y = 0;y <= Y_SEGMENTS;y++){
		for (int x = 0;x <= X_SEGMENTS;x++){
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
            // printf("%f %f\n",xSegment,ySegment);
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI) * radius;
			float yPos = std::cos(ySegment * PI) * radius;
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI) * radius;
			sphereVertices.push_back(xPos);
			sphereVertices.push_back(yPos);
			sphereVertices.push_back(zPos);
            textureCoords.push_back(1- xSegment); // 直接用xSegment会出现纹理颠倒的情况
            textureCoords.push_back(ySegment);
		}
	}

    for (int i=0;i < Y_SEGMENTS;i++){
		for (int j=0;j < X_SEGMENTS;j++){
			sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j+1);
			sphereIndices.push_back(i* (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
			sphereIndices.push_back(i * (X_SEGMENTS + 1) + j + 1);
		}
	}

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &NBO);
    glGenBuffers(1, &TBO);
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);
    //球的法向量正好是坐标
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(int), &sphereIndices[0], GL_STATIC_DRAW);
    //纹理坐标
    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(float), &textureCoords[0], GL_STATIC_DRAW);
    //设置顶点属性指针
    glEnableVertexAttribArray(0);
    //绑定VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //绑定NBO
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //绑定TBO
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    //解绑VAO,VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
    this->Color = Color;
}

Sphere::~Sphere(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1,&NBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &TBO);
}
void Sphere::draw(Shader &shader_ ,glm::mat4 & projection, glm::mat4 &view, glm::mat4 &model,unsigned int texture){
    shader_.use();
    shader_.set_3trans_matrix(glm::value_ptr(projection),glm::value_ptr(view),glm::value_ptr(model));
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
}