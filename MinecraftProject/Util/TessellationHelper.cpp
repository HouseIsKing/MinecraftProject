//
// Created by amit on 4/21/2022.
//

#include "TessellationHelper.h"
#include "../Entities/Player/PlayerController.h"
#include <ranges>

TessellationHelper::~TessellationHelper()
{
    glDeleteBuffers(1, &Vbo);
	glDeleteBuffers(1, &Ebo);
    glDeleteVertexArrays(1, &Vao);
}

bool TessellationHelper::HasInitialized() const
{
	return HasInit;
}

TessellationHelper::TessellationHelper(Shader* shader) : Vbo(0), Vao(0), Ebo(0), TheShader(shader), HasInit(false)
{
    glGenVertexArrays(1, &Vao);
    glGenBuffers(1, &Vbo);
	glGenBuffers(1, &Ebo);
    TheShader->Use();
    PositionUniform = TheShader->GetUniformInt("transformationMatrix");
}

Transform& TessellationHelper::GetTransform()
{
	return TessellationTransform;
}

uint16_t TessellationHelper::AddVertex(const Vertex& vertex)
{
	Vertices.push_back(vertex);
	return static_cast<uint16_t>(Vertices.size() - 1);
}

void TessellationHelper::AddTriangle(const uint16_t triangle)
{
	TriangleIndices.push_back(triangle);
}

/*
void TessellationHelper::changeVertex(uint16_t vertexID, Vertex vertex) {
    vertices.at(vertexID) = vertex;
    glBindBuffer(GL_ARRAY_BUFFER, Vbo);
    glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)(vertexID * sizeof(Vertex)), sizeof(Vertex), &vertices[vertexID]);
}
*/
void TessellationHelper::Draw()
{
	if (Vertices.empty())
	{
		return;
	}
	Shader::SetMat4(PositionUniform, TessellationTransform.GetTransformMatrix());
	glBindVertexArray(Vao);
	if (!HasInit)
	{
		HasInit = true;
		glBindBuffer(GL_ARRAY_BUFFER, Vbo);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLintptr>(Vertices.size() * sizeof(Vertex)), Vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, TexCoords)));
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Color)));
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, IndexTexture)));
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
		glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Brightness)));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLintptr>(TriangleIndices.size() * sizeof(GLushort)), TriangleIndices.data(), GL_STATIC_DRAW);
	}
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(TriangleIndices.size()), GL_UNSIGNED_SHORT, nullptr);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
}

void TessellationHelper::Reset()
{
    glDeleteBuffers(1, &Vbo);
	glDeleteBuffers(1, &Ebo);
    glDeleteVertexArrays(1, &Vao);
    Vertices.clear();
    TriangleIndices.clear();
    glGenVertexArrays(1, &Vao);
    glGenBuffers(1, &Vbo);
	glGenBuffers(1, &Ebo);
    HasInit = false;
}

TessellationHelper::TessellationHelper(Shader* shader, const float x, const float y, const float z) : TessellationHelper(shader)
{
	TessellationTransform.SetPosition(x, y, z);
}
