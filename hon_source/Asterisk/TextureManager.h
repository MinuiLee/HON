#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
/******************************************************************************/
/*!
\headerfile   TextureManager.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\brief
		Header file for TextureManager class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "ResourceManager.h"

class TextureManager : public ResourceManager<Texture, TextureManager>
{
public:
    TextureManager() : ResourceManager(fs::current_path().string() + "\\Media\\Textures") {}

    Texture * Load(const std::string & path)
    {
		Texture * texture = new Texture;
		if (!texture->loadFromFile(path))
		{
			delete texture;
			texture = nullptr;
		}
		return texture;
    }
};

#endif