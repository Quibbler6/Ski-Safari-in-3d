#define GLEW_STATIC
#include "bush.h"
#include <cstdlib> // For rand() and srand()
#include <ctime> // For time()

bush::bush(float baseRadius, glm::vec3 Color) {
    this->Color = Color;
    tc_size = 0;
    srand((unsigned)time(0)); // Seed random number generator

    // Create vertices with random perturbation
    for (int y = 0; y <= 30; ++y) {
        for (int x = 0; x <= 30; ++x) {
            float xSegment = (float)x / 30;
            float ySegment = (float)y / 30;
            float xOffset = (rand() % 200 - 50) / 500.0f; // Random perturbation
            float yOffset = (rand() % 200 - 50) / 500.0f;
            float zOffset = (rand() % 200 - 50) / 500.0f;
            float xPos = (std::cos(xSegment * 2.0f * pi) * std::sin(ySegment * pi) * baseRadius) + xOffset;
            float yPos = (std::cos(ySegment * pi) * baseRadius) + yOffset;
            float zPos = (std::sin(xSegment * 2.0f * pi) * std::sin(ySegment * pi) * baseRadius) + zOffset;
            bushVertices.push_back(xPos);
            bushVertices.push_back(yPos);
            bushVertices.push_back(zPos);
            // Assuming normals are the same as vertices for simplicity
            bushVertices.push_back(xPos);
            bushVertices.push_back(yPos);
            bushVertices.push_back(zPos);
            // Texture coordinates can be random for bushs
            textureCoords[tc_size++] = (float)rand() / RAND_MAX;
            textureCoords[tc_size++] = (float)rand() / RAND_MAX;
        }
    }

    // Create indices (assuming each segment is a quad, split into two triangles)
    for (int i = 0; i < 30; ++i) {
        for (int j = 0; j < 30; ++j) {
            bushIndices.push_back(i * (30 + 1) + j);
            bushIndices.push_back((i + 1) * (30 + 1) + j);
            bushIndices.push_back((i + 1) * (30 + 1) + j + 1);

            bushIndices.push_back(i * (30 + 1) + j);
            bushIndices.push_back((i + 1) * (30 + 1) + j + 1);
            bushIndices.push_back(i * (30 + 1) + j + 1);
        }
    }

    // Setup OpenGL buffers and attributes
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &NBO);
    glGenBuffers(1, &TBO);

    glBindVertexArray(VAO);

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, bushVertices.size() * sizeof(float), &bushVertices[0], GL_STATIC_DRAW);

    // Normals
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, bushVertices.size() * sizeof(float), &bushVertices[0], GL_STATIC_DRAW);

    // Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bushIndices.size() * sizeof(unsigned int), &bushIndices[0], GL_STATIC_DRAW);

    // Texture Coordinates
    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, tc_size * sizeof(float), &textureCoords[0], GL_STATIC_DRAW);

    // Vertex Attributes
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Normal Attributes
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Texture Coordinate Attributes
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindVertexArray(0); // Unbind VAO
}

bush::~bush() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &NBO);
    glDeleteBuffers(1, &TBO);
}

void bush::draw(Shader& shader_, glm::mat4& projection, glm::mat4& view, glm::mat4& model, unsigned int* texture) {
    shader_.use();
    shader_.set_3trans_matrix(glm::value_ptr(projection), glm::value_ptr(view), glm::value_ptr(model));

    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glDrawElements(GL_TRIANGLES, bushIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}