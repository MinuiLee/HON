/******************************************************************************/
/*!
\file   Texture.cpp
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2018/12/14

\copyright
All content 2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Texture.h"
#include "stb_image.h"

Texture::~Texture() 
{
	glDeleteTextures(1, &m_textureID);
}


bool Texture::loadFromFile(const std::string & path, bool is_near)
{
    int width, height, nr_channels;
	stbi_set_flip_vertically_on_load(1);
    unsigned char* pixel = stbi_load(path.c_str(), &width, &height, &nr_channels, STBI_rgb_alpha);

    m_size.x = width;
    m_size.y = height;

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	if (is_near)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
	glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(pixel);
	return true;
}