#include "TessellationHelper.h"
#include "EngineDefaults.h"

Transform* TessellationHelper::GetTransform(const size_t id) const
{
	return TessellationTransforms[id].get();
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

glm::mat4x4 TessellationHelper::GetTransformationMatrix(const size_t id) const
{
	Transform* transform = GetTransform(id);
	glm::mat4x4 helper = TessellationTransforms.at(id)->GetTransformMatrix();
	while (transform->GetParent() != nullptr)
	{
		transform = transform->GetParent();
		helper = transform->GetTransformMatrix() * helper;
	}
	return helper;
}

TessellationHelper::TessellationHelper(TransformStruct* transform)
{
	TheShader = EngineDefaults::GetShader();
	PositionUniform = TheShader->GetUniformInt("transformationMatrix");
	TessellationTransforms.emplace_back(new Transform(transform));
}

void TessellationHelper::Draw()
{
	Draw(0, 0, TrianglesCount);
}

void TessellationHelper::FreeMemory()
{
	Vertices.shrink_to_fit();
	TriangleIndices.shrink_to_fit();
}

void TessellationHelper::Draw(const size_t transformId, const size_t startPos, size_t count)
{
	if (Vertices.empty() && !HasInit)
	{
		return;
	}
	TheShader->Use();
	Shader::SetMat4(PositionUniform, GetTransformationMatrix(transformId));
	glBindVertexArray(Vao);
	if (!HasInit)
	{
		HasInit = true;
		glBindBuffer(GL_ARRAY_BUFFER, Vbo);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLintptr>(Vertices.size() * sizeof(Vertex)), Vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, TexCoords)));
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Color)));
		glVertexAttribIPointer(3, 1, GL_INT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, IndexTexture)));
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
		glVertexAttribIPointer(5, 1, GL_INT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Brightness)));
		glVertexAttribIPointer(6, 1, GL_UNSIGNED_INT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, SpecialFactors)));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLintptr>(TriangleIndices.size() * sizeof(GLushort)), TriangleIndices.data(), GL_STATIC_DRAW);
		TrianglesCount = TriangleIndices.size();
		Vertices.clear();
		TriangleIndices.clear();
	}
	if (count == 0)
	{
		count = TrianglesCount;
	}
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_SHORT, reinterpret_cast<void*>(startPos * sizeof(GLushort)));
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
	glDisableVertexAttribArray(6);
}

size_t TessellationHelper::AddTransform(Transform* transform)
{
	TessellationTransforms.emplace_back(transform);
	return TessellationTransforms.size() - 1;
}

void TessellationHelper::Reset()
{
    glDeleteBuffers(1, &Vbo);
    glDeleteBuffers(1, &Ebo);
    glDeleteVertexArrays(1, &Vao);
    glGenVertexArrays(1, &Vao);
    glGenBuffers(1, &Vbo);
    glGenBuffers(1, &Ebo);
    Vertices.clear();
    TriangleIndices.clear();
    HasInit = false;
}
