/*!***********************************************************************
 * @file			font.h
 * @author			Tan Wei Fong
 * @par DP email:	weifong.tan@digipen.edu
 * @par Course:		CSD2401F23-B
 * @par				Milestone 2
 * @date			2-11-2023
 * @brief
 *
 * This file provides the declaration needed for font.cpp
*************************************************************************/

#ifndef FONT_H
#define FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include <map>

namespace Night {
    struct TextChar {
        unsigned int TextureID; //texture id for character
        glm::ivec2 Size;    //size of character
        glm::ivec2 Bearing; //bearing of character
        unsigned int Advance; //advance value for chacter
    };

    class Font {
    public:
        /*!***********************************************************************
        \brief
        Constructor for class Font

        *************************************************************************/
        Font() = default;

        /*!***********************************************************************
        \brief
        Destructor for AudioManager

        *************************************************************************/
        ~Font();

        /*!***********************************************************************
        \brief
        Initialize the font

        \param fontFile path to fone file

        \param fontSize size of font to render

        \return true if success

        *************************************************************************/
        static void Init();

        static void Load(const char* fontFile, unsigned int fontSize);

        /*!***********************************************************************
        \brief
        Renders the font

        \param text Text string to render

        \param x x-coordinate for the text to render

        \param y y-coordinate for the text to render

        \param scale scale for text rendering

        \param colour color for text rendering

        *************************************************************************/
        static void RenderText(std::string text, glm::vec2 pos, float scale, Camera& cam, glm::vec3 color);

        /*!***********************************************************************
        \brief
        Error detection
        *************************************************************************/
        void checkTextures();
    private:
        
    };
}

#endif