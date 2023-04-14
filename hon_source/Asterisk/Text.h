#ifndef TEXT_H
#define TEXT_H
/*!*******************************************************************
\headerfile Text.h <>
\author     Kim HyungSeob
\par        email: hn02415 \@ gmail.com
\brief      Header file for Text Class.

\copyright
All content 2018 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include <string>
#include <unordered_map>

#include "Font.h"
#include "Window.h"
#include "Vector.h"
#include "Mesh.h"

/*!*******************************************************************
\class Text
\brief
	   Holds characters to print and has methods to draw.
********************************************************************/
class Text
{
public:
	/*!*******************************************************************
	\brief Default Constructor.
	********************************************************************/
	Text() = default;
	/*!*******************************************************************
	\brief
	   Constructor set string and font in once.

	\param text_string
	   Characters to print.

	\param text_font
	   Pointer to font to use for this text.
	********************************************************************/
	Text(std::string text_string, Font & text_font);

	/*!*******************************************************************
	\brief
	   Prints text by getting information from font and setting to a
		   sprite and then passing to draw function.

	\param window
	   Pointer to window that has draw function that will be used.
	********************************************************************/
	void Draw(Window * window, const float & depth);
	void DirectProjectionDraw(Window * window, const float & depth);

	/*!*******************************************************************
	\brief
		   Getter method for string.

	\return
		Characters to print.
	********************************************************************/
	std::string GetString() const { return m_string; }
	/*!*******************************************************************
	\brief
	   Set string to print.

	\param text_string
	   Characters to print.
	********************************************************************/
	void SetString(const std::string & text_string);
	/*!*******************************************************************
	\brief
	   Getter method for font.

	\return
		Pointer to font.
	********************************************************************/
	const Font * GetFont() const { return m_font; }
	/*!*******************************************************************
	\brief
	   Set pointer to font.

	\param text_font
	   Pointer to font.
	********************************************************************/
	void SetFont(Font & text_font);

	/*!*******************************************************************
	\brief
	   Set position of text.

	\param x
	   x position.

	\param y
	   y position.
	********************************************************************/
	void SetPosition(float x, float y);
	/*!*******************************************************************
	\brief
	   Set position of text.

	\param position
	   Position to set.
	********************************************************************/
	void SetPosition(Vector2f position) { m_position = position; }

	/*!*******************************************************************
	\brief
	   move position of text.

	\param position
	   Position to move.
	********************************************************************/
	void Move(Vector2f position) { m_position += position; }

	/*!*******************************************************************
	\brief
	   get position of text.

	 \return
		position of the text.
	********************************************************************/
	Vector2f GetPosition() { return m_position; }

	/*!*******************************************************************
	\brief
	Set scale of text.

	\param x
	   x scale.

	\param y
	   y scale.

	\param z
	   z scale.
	********************************************************************/
	void SetScale(float x, float y, float z)
	{
		m_sprite.GetTransform().SetScale(Vector3f(x, y, z));
	}
	/*!*******************************************************************
	\brief
		   Set scale of text.

	\param scale
	   Scale to set.
	********************************************************************/
	void SetScale(Vector3f scale) { m_sprite.GetTransform().SetScale(scale); }

	void SetShader(Shader * input_shader) { m_pShader = input_shader; }
	void SetDepth(float depth) { m_sprite.SetDepth(depth); }

private:
	std::string m_string{};  //!< Characters to print.
	Font * m_font = nullptr; //!< Pointer to font.
	Sprite m_sprite;		 //!< Holds information of each character. Used for drawing function.
	Vector2f m_position;	 //!< Position of text.

	Shader * m_pShader;
	int m_lineLimit = 0; //!< Maximum length of each line. 0 is assumed as no limit.

	Vector2f UpdateText(Vector2f & position, char & current_char);
};

#endif