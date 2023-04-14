/******************************************************************************/
/*!
\file   Mesh.cpp
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Mesh.h"
#include "gtc/constants.inl"


unsigned __int64 Mesh::Buffer_Size() const { return (vertices.size() * sizeof(Stride)); }

Mesh::Mesh(const Vector2f & size)
{
    Square(size.x, size.y);
    Initialize_VAO_VBO();
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::ObjectsSetting()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // VAO
	
    glBindVertexArray(VAO);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Buffer_Size(), &vertices[0], GL_STATIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0],
	    GL_STATIC_DRAW);

    // VAO
    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Stride), reinterpret_cast<void *>(0));

    // Texture coordinate
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Stride),reinterpret_cast<void *>(sizeof(Position)));

}

void Mesh::Initialize_VAO_VBO()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Buffer_Size(), &vertices[0], GL_STATIC_DRAW);

	// EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0],
		GL_STATIC_DRAW);

	// VAO
	// Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Stride), reinterpret_cast<void *>(0));

	// Texture coordinate
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Stride), reinterpret_cast<void *>(sizeof(Position)));
}


void Mesh::Update_Buffers()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Buffer_Size(), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0],GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Stride), reinterpret_cast<void *>(0));

    // Texture coordinate
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Stride), reinterpret_cast<void *>(sizeof(Position)));
}

void Mesh::debug_triangle()
{
    auto bottom_right = Stride(0.25f, -0.25f, 0.0f);
    auto bottom_left  = Stride(-0.25f, -0.25f, 0.0f);
    auto top_right    = Stride(0.25f, 0.25f, 0.0f);

    vertices.push_back(bottom_left);
    vertices.push_back(bottom_right);
    vertices.push_back(top_right);

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    primitive_	= GL_TRIANGLES;
    number_of_vertex_ = 3;
}

void Mesh::Square(float x, float y, int vertice_num)
{
    poly_vertices.clear();
    vertices.clear();
    indices.clear();

    float width  = x / 2.f;
    float height = y / 2.f;

    for (int i = 0; i < vertice_num; i++)
    {
		float angle   = 2.0f * glm::pi<float>() * ((float)i / vertice_num);
		glm::vec3 point = glm::vec3{ cosf(angle), sinf(angle), 1.0f} * glm::mat3{width, 0.f, 0.f, 0.f, height, 0.f, 0.f, 0.f, 1.f};
		poly_vertices.push_back({point.x, point.y});
    }
    number_of_poly_vertex = vertice_num;

    auto bottom_right = Stride(width, -height, 0.0f);
    auto bottom_left  = Stride(-width, -height, 0.0f);
    auto top_right    = Stride(width, height, 0.0f);
    auto top_left     = Stride(-width, height, 0.0f);

    bottom_right.textureCoordinates[0] = 1.f;
    bottom_right.textureCoordinates[1] = 0.0f;

    bottom_left.textureCoordinates[0] = 0.0f;
    bottom_left.textureCoordinates[1] = 0.0f;

    top_right.textureCoordinates[0] = 1.f;
    top_right.textureCoordinates[1] = 1.f;

    top_left.textureCoordinates[0] = 0.0f;
    top_left.textureCoordinates[1] = 1.f;

    vertices.push_back(bottom_left);
    vertices.push_back(bottom_right);
    vertices.push_back(top_right);
    vertices.push_back(top_left);

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(3);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(3);

    primitive_	= GL_TRIANGLES;
    number_of_vertex_ = 6;
}

GLsizei Mesh::Get_NumOfVertices()
{
	return number_of_vertex_;
}

void  Mesh::SetTextureRect(const FloatRect & rect, const Vector2f & texture_size)
{
	//bottom_left
	auto itr = vertices.begin();
	itr->textureCoordinates[0] = rect.m_left / texture_size.x;
	itr->textureCoordinates[1] = 1 - (rect.m_top / texture_size.y) - rect.m_height / texture_size.y;

	//bottom_right
	itr++;
	itr->textureCoordinates[0] = rect.m_left / texture_size.x + rect.m_width / texture_size.x;
	itr->textureCoordinates[1] = 1 - (rect.m_top / texture_size.y) - rect.m_height / texture_size.y;

	//top_right
	itr++;
	itr->textureCoordinates[0] = rect.m_left / texture_size.x + rect.m_width / texture_size.x;
	itr->textureCoordinates[1] = 1 - (rect.m_top / texture_size.y);

	//top_left
	itr++;
	itr->textureCoordinates[0] = rect.m_left / texture_size.x;
	itr->textureCoordinates[1] = 1 - (rect.m_top / texture_size.y);

	Update_Buffers();
}

void Mesh::Debug_Cube()
{
    auto bottom_left0  = Stride(-0.5f, -0.5f, -0.5f);
    auto bottom_right0 = Stride(0.5f, -0.5f, -0.5);
    auto top_right0    = Stride(0.5f, 0.5f, -0.5f);
    auto top_left0     = Stride(0.5f, 0.5f, -0.5f);


    vertices.push_back(bottom_left0);
    vertices.push_back(bottom_left0);
    vertices.push_back(bottom_left0);

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(0);

    primitive_	= GL_TRIANGLES;
    number_of_vertex_ = 6;
}

