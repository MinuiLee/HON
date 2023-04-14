#ifndef FONT_H
#define FONT_H
/*!*******************************************************************
\headerfile Font.h <>
\author     Kim HyungSeob
\par        email: hn02415 \@ gmail.com
\brief      Header file for Font Class.

\copyright
All content 2018 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include <string>
#include <unordered_map>
#include <vector>

#include "Sprite.h"

/*!*******************************************************************
\class Font
\brief
       Holds texture of characters and information of that.
********************************************************************/
class Font
{
public:
    /*!*******************************************************************
    \struct Character
    \brief
           Information of each character.
    ********************************************************************/
    struct Character
    {
        char id = char(-1); // The character id.

        unsigned short x = 0; // The left position of the character image in the texture.
        unsigned short y = 0; // The top position of the character image in the texture.

        unsigned short width = 0; // The width of the character image
        unsigned short height = 0; // The height of the character image

        short xoffset = 0; // How much the current position should be moved to right from origin.
        short yoffset = 0; // How much the current position should be moved to down from origin.

        short xadvance = 0; // How much the origin should be advanced after drawing the character.
        unsigned char page = 0; // The texture page where the character image is found.
    };

    /*!*******************************************************************
    \struct Information
    \brief
           Information of image texture.
    ********************************************************************/
    struct Information
    {
        short fontSize = 0;
        std::string fontName{};

        unsigned short lineHeight = 0; // The distance in pixels between each line of text.

        unsigned short imageWidth = 0;
        unsigned short imageHeight = 0;

        unsigned short pagesCount = 0; // The number of texture pages included in the font.
        std::string pageNames{};
    };

public:
    /*!*******************************************************************
    \brief
           Load font image texture and fnt file that has information of
           texture.

    \param fnt_filepath
           Path to fnt file.

    \return
            Return false when load failed.
    ********************************************************************/
    bool LoadFromFile(const std::string & fnt_filepath);
    /*!*******************************************************************
    \brief Clear of containers and release texture.
    ********************************************************************/
    void Clear();

    /*!*******************************************************************
    \brief
           Getter method for character information.

    \param character_id
           Character that want to know information.

    \return
            Information of character.
    ********************************************************************/
    Character GetCharacter(char character_id) const;
    /*!*******************************************************************
    \brief
           Getter method for mesh of each character.

    \param character_id
           Character that want to get mesh.

    \return
            Pointer to mesh.
    ********************************************************************/
    Mesh* GetMesh(char character_id);
    /*!*******************************************************************
    \brief
           Getter method for texture.

    \return
            Reference to texture.
    ********************************************************************/
    Texture & GetTexture() { return m_fontImage; }

    /*!*******************************************************************
    \brief
           Getter method for information of texture.

    \return
            Information of texture.
    ********************************************************************/
    const Information & GetInformation() const { return m_information; }
    /*!*******************************************************************
    \brief
           Getter method for line height.

    \return
            Height of line.
    ********************************************************************/
    unsigned short GetLineHeight() const { return m_information.lineHeight; }

private:
    /*!*******************************************************************
    \brief
           Read fnt file and get save all of information.

    \param fnt_filepath
           Path to fnt file.

    \return
            Return false when read failed.
    ********************************************************************/
    bool CanParseFile(const std::string & fnt_filepath);
    /*!*******************************************************************
    \brief Save information of each character in mesh.
    ********************************************************************/
    void SetTextureRect_for_EachMesh();

private:
    Information m_information{};                        //!< Information of texture image.
    std::unordered_map<char, Character> m_characters{}; //!< List of information of each character.
    std::unordered_map<char, Mesh> m_meshes{};          //!< List information of image texture.
    Texture m_fontImage;                                //!< Texture image of this font.
};

#endif