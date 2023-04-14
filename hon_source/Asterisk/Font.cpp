/******************************************************************************/
/*!
\file   Font.cpp
\author Kim Hyungseob
\par    email: hn02415 \@ gmail.com
\date   2019/10/30

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <filesystem>
#include <fstream>

#include "Font.h"

namespace fs = std::filesystem;

template <typename Number>
void SearchLineAndSetNumber(const std::string & line, const std::string & key, Number & storage);
void SearchLineAndSetString(const std::string & line, std::string key, std::string & storage);

bool Font::LoadFromFile(const std::string & fnt_filepath)
{
    // Parse the file. Return false if failed to parse the file
    if (!CanParseFile(fnt_filepath)) return false;

    // Get the path to the parent directory of the fnt file path.
    // Image should be in the the same folder/directory as fnt file.
    const auto file_dir = fs::path(fnt_filepath).parent_path();

    // concate the file directory with the image's file name
    const auto image_filepath = file_dir / m_information.pageNames;

    if (!fs::exists(image_filepath)) return false;
    if (!m_fontImage.loadFromFile(image_filepath.string())) return false;

    SetTextureRect_for_EachMesh();

    return true;
}

void Font::Clear()
{
    m_characters.clear();
    m_meshes.clear();
    m_fontImage.ReleaseTexture();
}

Font::Character Font::GetCharacter(char character_id) const
{
    // Find description for the character
    auto match = m_characters.find(character_id);

    if (match == m_characters.end()) // Did not find a description
    {
	    // BMFont uses an id of - 1 for invalid character, Try to use that if it exists
		match = m_characters.find(static_cast<char>(-1));

	    if (match == m_characters.end()) return Font::Character{};
    }

    return match->second;
}

Mesh * Font::GetMesh(char character_id)
{
    if (m_meshes.find(character_id) == m_meshes.end()) return nullptr;
    return &m_meshes[character_id];
}

bool Font::CanParseFile(const std::string & fnt_filepath)
{
    std::ifstream stream(fnt_filepath);
    if (!stream) return false;

    std::string line;
    std::string line_type;
    while (stream)
    {
	    stream >> line_type; // get the fisrt string of each line to know what it is specifying
	    std::getline(stream, line); // get the rest of the line

	    if (line_type == "info")
	    {
		    SearchLineAndSetString(line, "face", m_information.fontName);
		    SearchLineAndSetNumber(line, "size=", m_information.fontSize);
	    }
	    else if (line_type == "common")
	    {
		    SearchLineAndSetNumber(line, "lineHeight=", m_information.lineHeight);
		    SearchLineAndSetNumber(line, "scaleW=", m_information.imageWidth);
		    SearchLineAndSetNumber(line, "scaleH=", m_information.imageHeight);
		    SearchLineAndSetNumber(line, "pages=", m_information.pagesCount);

		    m_information.pageNames.resize(m_information.pagesCount);
	    }
	    else if (line_type == "page")
	    {
		    int offset = 0;
		    std::string name{};

		    SearchLineAndSetNumber(line, "id=", offset);
		    SearchLineAndSetString(line, "file", name);

		    m_information.pageNames = name;
	    }
	    else if (line_type == "char")
	    {
		    Character desc{};

		    SearchLineAndSetNumber(line, "id=", desc.id);
		    SearchLineAndSetNumber(line, "x=", desc.x);
		    SearchLineAndSetNumber(line, "y=", desc.y);
		    SearchLineAndSetNumber(line, "width=", desc.width);
		    SearchLineAndSetNumber(line, "height=", desc.height);
		    SearchLineAndSetNumber(line, "xoffset=", desc.xoffset);
		    SearchLineAndSetNumber(line, "yoffset=", desc.yoffset);
		    SearchLineAndSetNumber(line, "xadvance=", desc.xadvance);
		    SearchLineAndSetNumber(line, "page=", desc.page);

		    m_characters.insert_or_assign(desc.id, desc);
	    }
    }

    stream.clear();
    stream.close();

    return true;
}

void Font::SetTextureRect_for_EachMesh()
{
    for (auto & i : m_characters)
    {
	auto character			   = i.first;
	auto character_description = i.second;

	FloatRect rect;
	rect.m_width  = character_description.width;
	rect.m_height = character_description.height;
	rect.m_top	= character_description.y;
	rect.m_left   = character_description.x;

	Vector2f texture_size = m_fontImage.GetSize();
	//_CrtDumpMemoryLeaks();
	m_meshes[character] = Mesh(Vector2f(character_description.width, character_description.height));
	m_meshes[character].SetTextureRect(rect, texture_size);
    }
}

/*
\brief Search a string for a given key and get it's value, which is assumed to be some Number type.

\tparam Number
	integral type like short, unsigned int, int, etc

\param line
	the string of text to search
\param key
	the key in the string to find
\param storage
	the place to store the value of the key
 */
template <typename Number>
void SearchLineAndSetNumber(const std::string & line, const std::string & key, Number & storage)
{
    auto offset = line.find(key);

    if (offset != std::string::npos) // Success to find
    {
	offset += key.length();
	auto end = line.find(" ", offset);
	storage  = static_cast<Number>(std::stoi(line.substr(offset, end - offset)));
    }
}

/**
\brief Search a string for a given key and get it's value, which is assumed to be a string
surrounded with quote characters

\param line
	the string of text to search
\param key
	the key in the string to find
\param storage
	the place to store the value of the key
 */
void SearchLineAndSetString(const std::string & line, std::string key, std::string & storage)
{
    key += "=\"";
    auto offset = line.find(key);
    if (offset != std::string::npos)
    {
	offset += key.length();
	auto end = line.find("\"", offset);
	storage  = line.substr(offset, end - offset);
    }
}