/******************************************************************************/
/*!
\file   Text.cpp
\author Kim Hyungseob
\par    email: hn02415 \@ gmail.com
\date   2018/10/30

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Text.h"

Text::Text(std::string text_string, Font & text_font) 
	: m_string(std::move(text_string)), m_font(&text_font)
{
}

void Text::SetString(const std::string & text_string)
{
    if (m_string != text_string)
    { 
	    m_string = text_string;
    }
}

void Text::SetFont(Font & text_font)
{ 
    if (m_font != &text_font)
    { 
	    m_font = &text_font;
	    m_sprite.SetTexture(m_font->GetTexture());
    }
}

void Text::SetPosition(float x, float y)
{
    m_position.x = x;
    m_position.y = y;
}

Vector2f Text::UpdateText(Vector2f & position, char & current_char)
{
	Vector2f current_position = position;


	float xAdvance;
	if (m_font->GetCharacter(current_char).xadvance) // Get the xadvance of the whitespace character
		xAdvance = m_font->GetCharacter(current_char).xadvance;
	else // If it doesn't exist then use the fontSize
		xAdvance = m_font->GetInformation().fontSize;

	auto scale = m_sprite.GetScale();

	xAdvance *= scale.x;
	float line_height = m_font->GetInformation().lineHeight * scale.y;


	// Test for non-character
	if (current_char == ' ')
	{
		current_position.x += xAdvance;
		return current_position;
	}
	else if (current_char == '\t') // Tab
	{
		current_position.x += xAdvance;
		return current_position;
	}
	else if (current_char == '\n')
	{
		current_position.x = m_position.x;
		current_position.y -= line_height;
		return current_position;
	}

	auto character_description = m_font->GetCharacter(current_char);

	// Set position
	m_sprite.SetPosition(current_position.x +
			(character_description.width / 2 + character_description.xoffset) * scale.x,
		current_position.y - character_description.height / 2 * scale.y -
			character_description.yoffset * scale.y,
		1.0f // HARDCODE
	);

	// Set mesh (has texture rect)
	m_sprite.SetMesh(m_font->GetMesh(current_char));
	m_sprite.SetShader(m_pShader);
		

	// Push position
	current_position.x += xAdvance;

	return current_position;
}

void Text::Draw(Window * window, const float & /*depth*/)
{ 
    Vector2f current_position = m_position;

	for (auto & i : m_string)
	{
		current_position = UpdateText(current_position, i);

		// Draw
		window->GetBaseWindow()->Draw(m_sprite);
	}
}

void Text::DirectProjectionDraw(Window * window, const float & depth)
{
	Vector2f current_position = m_position;

	for (auto & i : m_string)
	{
		current_position = UpdateText(current_position, i);

		// Draw
		window->GetBaseWindow()->CalculateDirectProjectionDraw(m_sprite, depth);
		window->GetBaseWindow()->DirectProjectionDraw(m_sprite);
	}
}
