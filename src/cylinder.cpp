#define GLEW_STATIC
#include "cylinder.h"

Cylinder::Cylinder(glm::vec3 start, glm::vec3 end, float radius, glm::vec3 Color) {

    tc_size = 0;
    glm::vec3 axis = glm::normalize(end - start); // 圆柱体的轴向量
    float height = glm::length(end - start); // 圆柱体的高度

    // ... [其他代码不变]

    // 修改圆柱体底部和顶部的顶点坐标生成代码
    for (int y = 0; y <= 1; y++) {
        glm::vec3 center = (y < 1) ? start : end;
        for (int x = 0; x <= X_SEGMENTS; x++) {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float xAngle = xSegment * 2.0f * Pi;
            glm::vec3 circleVec(cos(xAngle), 0.0f, sin(xAngle));

            // 底部和顶部顶点
            glm::vec3 vertex = center + radius * circleVec;
            cylinderVertices.push_back(vertex.x);
            cylinderVertices.push_back(vertex.y);
            cylinderVertices.push_back(vertex.z);

            // 计算法向量
            glm::vec3 normal = circleVec;  // 底部法向量为负轴向量，顶部法向量为轴向量

            // 存储法向量
            cylinderNormals.push_back(normal.x);
            cylinderNormals.push_back(normal.y);
            cylinderNormals.push_back(normal.z);
            // 纹理坐标
            textureCoords[tc_size++] = xSegment;
            textureCoords[tc_size++] = (y == 0) ? 0.0f : 1.0f;
        }
    }
    // 创建索引
    for (int x = 0; x < X_SEGMENTS; x++) {
        // 底部索引
        cylinderIndices.push_back(x);
        cylinderIndices.push_back(x + 1);
        cylinderIndices.push_back(X_SEGMENTS + x + 1);
        // 顶部索引
        cylinderIndices.push_back(X_SEGMENTS + x + 1);
        cylinderIndices.push_back(X_SEGMENTS + x + 2);
        cylinderIndices.push_back(x + 1);
        // 侧面索引
        cylinderIndices.push_back((2 * X_SEGMENTS + 1) + x);
        cylinderIndices.push_back((2 * X_SEGMENTS + 1) + x + 1);
        cylinderIndices.push_back((3 * X_SEGMENTS + 1) + x + 1);
        cylinderIndices.push_back((3 * X_SEGMENTS + 1) + x);
        cylinderIndices.push_back((2 * X_SEGMENTS + 1) + x);
        cylinderIndices.push_back((3 * X_SEGMENTS + 1) + x + 1);
    }
    // 创建OpenGL对象
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &NBO);
    glGenBuffers(1, &TBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, cylinderVertices.size() * sizeof(float), &cylinderVertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, cylinderNormals.size() * sizeof(float), &cylinderNormals[0], GL_STATIC_DRAW);
    // 圆柱的法向量需要计算
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cylinderIndices.size() * sizeof(int), &cylinderIndices[0], GL_STATIC_DRAW);
    // 纹理坐标
    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, tc_size * sizeof(float), &textureCoords[0], GL_STATIC_DRAW);
    // 设置顶点属性指针
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    this->Color = Color;
}

Cylinder::~Cylinder() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &NBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &TBO);
}

void Cylinder::draw(Shader& shader_, glm::mat4& projection, glm::mat4& view, glm::mat4& model, unsigned int* texture) {
    shader_.use();
    shader_.set_3trans_matrix(glm::value_ptr(projection), glm::value_ptr(view), glm::value_ptr(model));
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glDrawElements(GL_TRIANGLES, cylinderIndices.size(), GL_UNSIGNED_INT, 0);
}