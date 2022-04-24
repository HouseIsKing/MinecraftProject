//
// Created by amit on 4/21/2022.
//

#include "TessellationHelper.h"
#include "../Entities/Player/PlayerController.h"

TessellationHelper::~TessellationHelper() {
    glDeleteBuffers(1, &vbo);
    for(const auto& [key,ebo]:elementBuffers)
        glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

bool TessellationHelper::HasInit() const {
    return hasInit;
}

TessellationHelper::TessellationHelper(Shader* shader) : theShader(shader), vao(0), vbo(0) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    hasInit = false;
}

Transform TessellationHelper::getTransform() const {
    return transform;
}

uint16_t TessellationHelper::addVertex(Vertex vertex) {
    vertices.push_back(vertex);
    return (uint16_t)vertices.size() - 1;
}

void TessellationHelper::addTriangle(Texture textureID, uint16_t triangle) {
    if(!textureIDToTriangles.contains(textureID))
    {
        GLuint helper;
        glGenBuffers(1, &helper);
        elementBuffers.emplace(textureID, helper);
        textureIDToTriangles.emplace(textureID, vector<uint16_t>());
    }
    textureIDToTriangles[textureID].push_back(triangle);
}

void TessellationHelper::changeVertex(uint16_t vertexID, Vertex vertex) {
    vertices.at(vertexID) = vertex;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)(vertexID * sizeof(Vertex)), sizeof(Vertex), &vertices[vertexID]);
}

void TessellationHelper::draw() {
    if(vertices.empty()) {
        hasInit = true;
        return;
    }
    theShader->use();
    theShader->setMat4("transformationMatrix", transform.getTransformMatrix());
    theShader->setMat4("view", CameraController::getActiveCamera().getViewMatrix());
    theShader->setMat4("projection", CameraController::getActiveCamera().getProjectionMatrix());
    glBindVertexArray(vao);
    if(!hasInit)
    {
        hasInit = true;
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, (GLintptr)(vertices.size() * sizeof(Vertex)), &vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        if(((int)vertices[0].getVertexUsage() & 0x1) == 1)
        {
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
        }
        if(((int)vertices[0].getVertexUsage() & 0x2) == 0x2)
        {
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        }
        for(const auto& [key,ebo]:elementBuffers)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLintptr)(textureIDToTriangles[key].size() * sizeof(uint16_t)), &textureIDToTriangles[key][0], GL_STATIC_DRAW);
        }
    }
    glEnableVertexAttribArray(0);
    if(((int)vertices[0].getVertexUsage() & 0x1) == 1)
        glEnableVertexAttribArray(1);
    if(((int)vertices[0].getVertexUsage() & 0x2) == 0x2)
        glEnableVertexAttribArray(2);
    for(const auto& [key,ebo]:elementBuffers)
    {
        key.use();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, (GLint)textureIDToTriangles[key].size(), GL_UNSIGNED_SHORT, nullptr);
    }
    glDisableVertexAttribArray(0);
    if(((int)vertices[0].getVertexUsage() & 0x1) == 1)
        glDisableVertexAttribArray(1);
    if(((int)vertices[0].getVertexUsage() & 0x2) == 0x2)
        glDisableVertexAttribArray(2);
}

void TessellationHelper::reset() {
    glDeleteBuffers(1, &vbo);
    for(const auto& [key,ebo]:elementBuffers)
        glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
    vertices.clear();
    textureIDToTriangles.clear();
    elementBuffers.clear();
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    hasInit = false;
}

TessellationHelper::TessellationHelper(Shader* shader, float x, float y, float z) : TessellationHelper(shader) {
    transform.setPosition(x, y, z);
}
