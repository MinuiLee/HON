#ifndef DOORUI
#define DOORUI
/*!*******************************************************************
\headerfile   DoorUI.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/03/03
\brief
			  Header file for UI for Door objects.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include <string>

#include "../Text.h"
#include "../Sprite.h"

class Window;
class TextureManager;

/*!*******************************************************************
\class DoorUI
\brief
	   Define UIs for Door.
********************************************************************/
class DoorUI
{
public:
	/*!*******************************************************************
	\brief
		   Singleton pattern method.

	\return DoorUI*
		    Returns pointer to instance of DoorUI.
	********************************************************************/
	static DoorUI * GetInstance();

	/*!*******************************************************************
	\brief
		   Draw all UIs.

	\param window
		   Pointer to Window that provides drawing interface.
	********************************************************************/
	void Draw(Window * window);

	/*!*******************************************************************
	\brief
		   Set textures for Sprite UIs.

	\param textureMgr
		   Pointer to TextureManager to get soul textures.
	********************************************************************/
	void Initialize(TextureManager* textureMgr);

	/*!*******************************************************************
	\brief
		   Set standard position.

	\param x
		   Position's x value.

	\param y
		   Position's y value.
	********************************************************************/
	void SetPosition(float x, float y);
	/*!*******************************************************************
	\brief
		   Set standard position.

	\param position
		   New position to set.
	********************************************************************/
	void SetPosition(Vector2f position);

	/*!*******************************************************************
	\brief
		   Set text for keys.

	\param x
		   Position's x value.

	\param y
		   Position's y value.
	********************************************************************/
	void SetText(SDL_Scancode purple, SDL_Scancode blue);

	/*!*******************************************************************
	\brief Toggle condition whether drawing UI or not.
	********************************************************************/
	void ToggleDraw();

	/*!*******************************************************************
	\brief
		   Set standard position.

	\re position
		   New position to set.
	********************************************************************/
	bool IsDrawing();

private: // Private methods
	/*!*******************************************************************
	\brief Priavte default constructor.
	********************************************************************/
	DoorUI();
	/*!*******************************************************************
	\brief
		   Private copy constructor.

	\param
		   Reference of this class.
	********************************************************************/
	DoorUI(const DoorUI &);
	/*!*******************************************************************
	\brief
		   Private assignment operator.

	\param
		   Reference of this class.
	********************************************************************/
	DoorUI & operator=(const DoorUI &);

	/*!*******************************************************************
	\brief Adjusts position of all UIs by m_position.
	********************************************************************/
	void AdjustPosition();

private: // Private member variables
	Vector2f m_position;  //!< Standard position of all UIs.
						  
	Text m_purple_key;    //!< Text UI represents Key for using purple soul.
	Text m_blue_key;      //!< Text UI represents Key for using blue soul.
						  
	Sprite m_purple_soul; //!< Sprite UI for purple soul.
	Sprite m_blue_soul;   //!< Sprite UI for blue soul.

	bool m_draw = true;   //!< Decide whether drawing UI or not.
};

#endif //DOORUI