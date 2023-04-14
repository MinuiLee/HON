#ifndef MESH_H
#define MESH_H
/******************************************************************************/
/*!
\headerfile   Mesh.h
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#define GLM_FORCE_CXX03
#include <glm.hpp>
#include <vector>
#include <GL/glew.h>

#include "Stride.h"
#include "FloatRect.h"

#define NUMBER_OF_COMP 2	// position, color
#define NUMBER_OF_ELEMENT 7 // 3 for position, 4 for color

class Shader;
class Mesh
{
	friend class Object;
	friend class Graphics;

	std::vector<Stride> vertices; // Collection of vertices position and color.
	std::vector<glm::vec2> poly_vertices;
	std::vector<unsigned int> indices; // Collection of vertices position and color.

	unsigned int tessellation_factor_ = 3; // default tesselation is 3.

	GLenum primitive_			  = GL_TRIANGLES;
	GLsizei number_of_vertex_	 = 0; // number of vertecies.
	GLsizei number_of_poly_vertex = 0; // for collision check

	GLuint VAO; // Vertex Attribute Object
	GLuint VBO; // Vertex Buffer Object
	GLuint EBO; // Element Buffer Object
	GLuint texture;


#ifdef _DEBUG
	GLuint polyVAO; // Vertex Attribute Object
	GLuint polyVBO; // Vertex Buffer Object
#endif

	Shader * shader_ = nullptr;


public:
	Mesh(const Vector2f & size = Vector2f(7.f, 27.f));
	~Mesh();
	unsigned __int64 Buffer_Size() const;
	void Initialize_VAO_VBO();
	void Update_Buffers();
	void ObjectsSetting();

	void debug_triangle();
	void Debug_Cube();
	void Square(float x, float y, int vertice_num = 4);
	// vertice num is for the shape of collision detection

	GLenum Get_Primitive() { return primitive_; }
	GLsizei Get_NumOfVertices();

	GLsizei Get_NumOfVertices() const { return number_of_vertex_; }
	GLsizei Get_NumOf_PolyVertices() const { return number_of_poly_vertex; }

	GLuint GetEBO() { return EBO; }
	GLuint GetVAO() { return VAO; }
	GLuint GetVAO() const { return VAO; }
	GLuint GetVBO() { return VBO; }
	GLuint GetVBO() const { return VBO; }
	GLuint * GetTexturePtr() { return &texture; }


	GLuint * GetVAOptr() { return &VAO; }
	GLuint * GetVBOptr() { return &VBO; }
	GLuint * GetEBOptr() { return &EBO; }

	std::vector<Stride> & GetStride() { return vertices; }
	std::vector<unsigned int> & GetIndics() { return indices; }

	Stride * GetVerticesPtr() { return &vertices[0]; }

#ifdef _DEBUG
	GLuint GetPolyVAO() { return polyVAO; }
	GLuint GetPolyVAO() const { return polyVAO; }
#endif

	glm::vec3 GetPoint(int i)
	{
		return glm::vec3{ vertices[i].position.x, vertices[i].position.y, vertices[i].position.z };
	}
	glm::vec3 GetPolyPoint(int i)
	{
		return glm::vec3{ poly_vertices[i].x, poly_vertices[i].y, 1.f };
	}

	void SetShader(Shader * input_shader) { shader_ = input_shader; }
	void SetTextureRect(const FloatRect & rect, const Vector2f & texture_size);
	Shader & GetShader(void) { return *shader_; }
};

#endif