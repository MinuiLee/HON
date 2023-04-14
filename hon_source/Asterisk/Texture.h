#ifndef TEXTURE_H
#define TEXTURE_H
/******************************************************************************/
/*!
\headerfile   Texture.h
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2018/12/14
\brief
		Header file for texture class.

\copyright
All content 2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <string>

#include "Vector.h"
#include "Mesh.h"

class Texture
{
public:
	Texture() {}
	Texture(const std::string & path, bool is_near = true) { loadFromFile(path, is_near); }
	~Texture();
	void ReleaseTexture() { glDeleteTextures(1, &m_textureID); }

	bool loadFromFile(const std::string & path, bool is_near = true);

	unsigned int GetTextureID() const { return m_textureID; }
	const Vector2u & GetSize() const { return m_size; }
	void SetSize(const Vector2u size) { m_size = size; }

private:
	unsigned int m_textureID = 0;
	Vector2u m_size;
};

#endif