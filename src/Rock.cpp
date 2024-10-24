#define GLEW_STATIC
#include "rock.h"
#include <cstdlib> // For rand() and srand()
#include <ctime> // For time()

Rock::Rock(float radius, glm::vec3 Color) {
    this->Color = Color;
    tc_size = 0;
    srand((unsigned)time(0)); // Seed the random number generator

    // Create vertices with random perturbation
    for (int y = 0; y <= Y_SEGMENTS; ++y) {
        for (int x = 0; x <= X_SEGMENTS; ++x) {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = 1.2 * std::cos(xSegment * 2.0f * phi) * std::sin(ySegment * phi) * radius;
            float yPos = std::abs(std::cos(ySegment * phi) * radius);
            float zPos = std::sin(xSegment * 2.0f * phi) * std::sin(ySegment * phi) * radius;

            // Add random perturbation
            float perturbFactor = (rand() % 100) / 100.0f * 0.1f; // Random perturbation up to 10% of radius
            xPos += xPos * perturbFactor;
            yPos += yPos * perturbFactor;
            zPos += zPos * perturbFactor;

            rockVertices.push_back(xPos);
            rockVertices.push_back(yPos);
            rockVertices.push_back(zPos);
            textureCoords[tc_size++] = 1 - xSegment;
            textureCoords[tc_size++] = ySegment;
        }
    }

    // Create indices for the rock
    for (int i = 0; i < Y_SEGMENTS; ++i) {
        for (int j = 0; j < X_SEGMENTS; ++j) {
            rockIndices.push_back(i * (X_SEGMENTS + 1) + j);
            rockIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
            rockIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
            rockIndices.push_back(i * (X_SEGMENTS + 1) + j);
            rockIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
            rockIndices.push_back(i * (X_SEGMENTS + 1) + j + 1);
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &NBO);
    glGenBuffers(1, &TBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, rockVertices.size() * sizeof(float), &rockVertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, rockVertices.size() * sizeof(float), &rockVertices[0], GL_STATIC_DRAW);
    //球的法向量正好是坐标
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, rockIndices.size() * sizeof(int), &rockIndices[0], GL_STATIC_DRAW);
    //纹理坐标
    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, tc_size * sizeof(float), &textureCoords[0], GL_STATIC_DRAW);
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

Rock::~Rock() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &NBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &TBO);
}
void Rock::draw(Shader& shader_, glm::mat4& projection, glm::mat4& view, glm::mat4& model, unsigned int* texture) {
    shader_.use();
    shader_.set_3trans_matrix(glm::value_ptr(projection), glm::value_ptr(view), glm::value_ptr(model));
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glDrawElements(GL_TRIANGLES, rockIndices.size(), GL_UNSIGNED_INT, 0);
}