# include "groundSurface.h"

GroundSurface::GroundSurface(float start_x, float start_z, float end_x, float end_z, float y){
    // set groundVertices
    groundVertices.push_back(start_x);
    groundVertices.push_back(y);
    groundVertices.push_back(start_z);

    groundVertices.push_back(start_x);
    groundVertices.push_back(y);
    groundVertices.push_back(end_z);

    groundVertices.push_back(end_x);
    groundVertices.push_back(y);
    groundVertices.push_back(end_z);

    groundVertices.push_back(end_x);
    groundVertices.push_back(y);
    groundVertices.push_back(start_z);
    // set groundNormals
    for(int i = 0;i < 3;i++){
        groundNormals.push_back(0.0f);
        groundNormals.push_back(1.0f);
        groundNormals.push_back(0.0f);
    }

    // set groundIndices
    groundIndices.push_back(0);
    groundIndices.push_back(1);
    groundIndices.push_back(3);

    groundIndices.push_back(1);
    groundIndices.push_back(2);
    groundIndices.push_back(3);

    // set groundTextureCoords
    groundTextureCoords.push_back(1.0f);
    groundTextureCoords.push_back(1.0f);

    groundTextureCoords.push_back(1.0f);
    groundTextureCoords.push_back(0.0f);

    groundTextureCoords.push_back(0.0f);
    groundTextureCoords.push_back(0.0f);

    groundTextureCoords.push_back(0.0f);
    groundTextureCoords.push_back(1.0f);

    // set VAOs and VBOs
    glGenVertexArrays(1, &groundVAO);
    glGenBuffers(1, &groundVBO);
    glGenBuffers(1,&groundNBO);
    glGenBuffers(1, &groundEBO);
    glGenBuffers(1, &groundTBO);
    glBindVertexArray(groundVAO);

    // set VBO
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, groundVertices.size() * sizeof(float), &groundVertices[0], GL_STATIC_DRAW);
    // set NBO
    glBindBuffer(GL_ARRAY_BUFFER, groundNBO);
    glBufferData(GL_ARRAY_BUFFER, groundNormals.size() * sizeof(float), &groundNormals[0], GL_STATIC_DRAW);
    // set EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, groundIndices.size() * sizeof(int), &groundIndices[0], GL_STATIC_DRAW);
    // set TBO
    glBindBuffer(GL_ARRAY_BUFFER, groundTBO);
    glBufferData(GL_ARRAY_BUFFER, groundTextureCoords.size() * sizeof(float), &groundTextureCoords[0], GL_STATIC_DRAW);
    // position attribute
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // normal attribute
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, groundNBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // texture attribute
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, groundTBO);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    // unbind VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GroundSurface::draw(Shader &shader_ ,glm::mat4 & projection, glm::mat4 &view, glm::mat4 &model,unsigned int texture){
    shader_.use();
    shader_.set_3trans_matrix(glm::value_ptr(projection),glm::value_ptr(view),glm::value_ptr(model));
    glBindVertexArray(groundVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

GroundSurface::~GroundSurface(){
    glDeleteVertexArrays(1, &groundVAO);
    glDeleteBuffers(1, &groundVBO);
    glDeleteBuffers(1,&groundNBO);
    glDeleteBuffers(1, &groundEBO);
    glDeleteBuffers(1, &groundTBO);
}