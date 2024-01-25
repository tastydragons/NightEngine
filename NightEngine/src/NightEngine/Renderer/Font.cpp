/*!***********************************************************************
 * @file			Font.cpp
 * @author			Tan Wei Fong
 * @par DP email:	weifong.tan@digipen.edu
 * @par Course:		CSD2401F23-B
 * @par				Milestone 2
 * @date			2-11-2023
 * @brief
 *
 * This file provides the definiton needed for font.cpp
*************************************************************************/

#include "Font.h"

namespace Night {
    std::map<char, TextChar> Characters;
    Ref<Shader> textShader;
    unsigned int VAO, VBO;

    /*!***********************************************************************
    \brief
    Initialize the font

    \param fontFile path to fone file

    \param fontSize size of font to render

    \return true if success

    *************************************************************************/
    void Font::Init() {
        textShader = CreateRef<Shader>("Assets/shaders/Font.glsl");
        Load("Assets/Fonts/ocr.ttf", 50);
        Load("Assets/Fonts/Nbp.ttf", 50);

        // Initialize VAO and VBO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Font::Load(const char* fontFile, unsigned int fontSize)
    {
        // Initialize FreeType library
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            std::cout << "Could not init FreeType Library" << std::endl;
        }
        else {
            std::cout << "Could  init FreeType Library" << std::endl;
        }

        // Load font
        FT_Face face;
        if (FT_New_Face(ft, fontFile, 0, &face)) {
            std::cout << "Failed to load font" << std::endl;
        }
        else {
            std::cout << "Could  init font" << std::endl;
        }

        //set size to load glyph
        FT_Set_Pixel_Sizes(face, 0, fontSize);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Load each ASCII character
        for (unsigned char c = 0; c < 128; c++) {
            // Load character glyph

            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cout << "Failed to load glyph" << std::endl;
                continue;
            }

            // Generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);

            GLenum error = glGetError();
            if (error != GL_NO_ERROR) {
                std::cerr << "Error binding ascii texture: " << error << std::endl;
            }

            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Store character
            TextChar character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, TextChar>(c, character));
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "Error unbinding ascii texture: " << error << std::endl;
        }

        // Destroy FreeType resources
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    Font::~Font() {
        // Clean up the OpenGL resources
        for (auto& character : Characters) {
            glDeleteTextures(1, &(character.second.TextureID));
        }
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    /*!***********************************************************************
    \brief
    Renders the font

    \param text Text string to render

    \param x x-coordinate for the text to render

    \param y y-coordinate for the text to render

    \param scale scale for text rendering

    \param colour color for text rendering

    *************************************************************************/
    void Font::RenderText(std::string text, glm::vec2 pos, float scale, Camera& cam, glm::vec3 color) {
        // Activate corresponding render state
        textShader->Bind();
        textShader->SetUniform("u_ViewProjection", cam.GetViewProjectionMatrix());
        textShader->SetUniform("textColor", color);

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // Iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) {
            TextChar ch = Characters[*c];
            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            // Update VBO for each character
            float vertices[6][4] = {
                { pos.x,     pos.y + h,   0.0f, 0.0f },
                { pos.x,     pos.y,       0.0f, 1.0f },
                { pos.x + w, pos.y,       1.0f, 1.0f },

                { pos.x,     pos.y + h,   0.0f, 0.0f },
                { pos.x + w, pos.y,       1.0f, 1.0f },
                { pos.x + w, pos.y + h,   1.0f, 0.0f }
            };
            // Render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);

            // Update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // Render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // Now advance cursors for next glyph
            pos.x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
    }


    /*!***********************************************************************
    \brief
    Error detection
    *************************************************************************/
    void Font::checkTextures() {
        // Iterate over the Characters map
        for (auto const& pair : Characters) {
            // Get the texture ID
            GLuint textureID = pair.second.TextureID;

            // Check if it's a valid texture
            if (glIsTexture(textureID) == GL_TRUE) {
                std::cout << "Texture ID " << textureID << " is valid." << std::endl;
            }
            else {
                std::cout << "Texture ID " << textureID << " is invalid." << std::endl;
            }
        }
    }
}