#ifndef C_AUDIO_H
#define C_AUDIO_H
/*!*******************************************************************
\headerfile C_Audio.h <>
\author     Kim HyungSeob
\par        email: hn02415 \@ gmail.com
\date		2018/11/17
\brief  
            Header file for Audio Component Class.

\copyright
All content 2018 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include <unordered_map>
#include "C_Base.h"

/*!*******************************************************************
\class C_Audio
\brief 
       Holds the list of the names of the sounds those will be used
       for each entity and has some methods for that.
********************************************************************/
class C_Audio : public C_Base
{
public:
    /*!*******************************************************************
    \brief Default Constructor with base class constructor.
    ********************************************************************/
    C_Audio() : C_Base(Component::Audio) {}

    /*!*******************************************************************
    \brief
	    Overrided method from C_Base class that reads configuration
            file for audio component.

    \param sstream
	   stringstream that has information of the names of the sounds 
           to be used for each entitiy
    ********************************************************************/
    void ReadIn(std::stringstream& sstream) override
    {
        std::string name;
        while (sstream >> name)
        {
	    m_soundList[name] = false;
        }   
    }

    /*!*******************************************************************
    \brief 
	   Getter method for list of sounds.

    \return 
	    List of the sounds that each entity uses.
    ********************************************************************/
    const std::unordered_map<std::string, bool>& GetSoundList() const { return m_soundList; }

    /*!*******************************************************************
    \brief 
	   Setter method makes sound on.

    \param sound
	   The name of the sound to mark on.
    ********************************************************************/
    void SetSoundOn(std::string sound)
    {
	if (m_soundList.find(sound) == m_soundList.end()) return;
        m_soundList[sound] = true; 
    }
    /*!*******************************************************************
    \brief
	Setter method makes sound off.

    \param sound
	The name of the sound to mark off.
    ********************************************************************/
    void SetSoundOff(std::string sound)
    {
	if (m_soundList.find(sound) == m_soundList.end()) return;
	m_soundList[sound] = false;
    }

private:
    std::unordered_map<std::string, bool> m_soundList; //!< The list of the sounds those each entity uses.
};
#endif