#pragma once

#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "IRenderer.h"
#include "Shader.h"
#include "Sphere.h"

struct RendererInitialisationData {
    int screenWidth;
    int screenHeight;
    std::string blocksTexturePath;
    int numBlocksInTexture;
};

class Renderer :
    public IRenderer
{
public:
    Renderer();
    Renderer(const RendererInitialisationData& initData);
    // Inherited via IRenderer
    virtual void DrawCuboid(const glm::vec3& centeredAt, const glm::vec3& dimensions, const Camera& camera, const glm::vec3& colour) const override;
    virtual void DrawSphere(const glm::vec3& centeredAt, const glm::vec3& dimensions, const Camera& camera, const glm::vec4& colour) const override;
    virtual void DrawInstancedBlocks(const size_t numberToDraw, const Camera& camera) override;
    virtual void DrawTextAnchoredToBottomLeft(std::string text, float x, float y, float scale, glm::vec3 colour) const override;
    virtual void DrawTextAnchoredToTopLeft(std::string text, float xOffset, float yOffset, float scale, glm::vec3 colour) const override;
    virtual void DrawTextAnchoredToTopRight(std::string text, float xOffset, float yOffset, float scale, glm::vec3 colour) const override;
    virtual void DrawTextAnchoredToBottomRight(std::string text, float xOffset, float yOffset, float scale, glm::vec3 colour) const override;

    virtual void SetInstancedBlocksUbo(const BlockInstanceRenderData* instances, const size_t numberToSet) override;
    virtual void SetLightPos(const glm::vec3& value) override;
    virtual void SetLightColour(const glm::vec3& value) override;
    virtual void SetScreenDims(const glm::ivec2& value) override;
    virtual void SetCubeShouldRender(size_t indexCubeIsAt, bool newValue) override;
    virtual void SetCubePos(size_t indexCubeIsAt, const glm::vec3& newPos) override;
    virtual void SetCubeColour(size_t indexCubeIsAt, const glm::vec3& newColour) override;

    virtual void LoadOneByTwoBlocksTexture(std::string blocksTextureFilePath, int numBlocks) override;

private:
    void Initialize();
    void InitFT();
    void LoadFont(std::string ttfFilePath);
    void InitializeSphereVertices();
    void InitializeTexturedOneByTwoCubeVertices();
    void InitializeColouredCubeVertices();

private:
    /// Holds all state information relevant to a character as loaded using FreeType
    struct Character {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };

private:
    Sphere m_sphere = Sphere(1.0f); // class stolen from http://www.songho.ca/opengl/gl_sphere.html

    const unsigned int m_baseTextSize = 48;
    unsigned int m_freeTypeVAO;
    unsigned int m_freeTypeVBO;

    unsigned int m_unitCubeVAO;
    unsigned int m_unitSphereVAO;
    unsigned int m_unitSphereEBO;

    unsigned int m_oneByTwoBlockVBO;
    unsigned int m_oneByTwoBlockVAO;

    Shader m_colouredShader;
    Shader m_colouredInstancedShader;
    Shader m_texturedInstancedShader;
    Shader m_textShader;

    glm::vec3 m_lightPos;
    glm::vec3 m_lightColour;
    unsigned int m_scrWidth = 800;
    unsigned int m_scrHeight = 1200;
    unsigned int m_blockInstanceDataUboHandle = 0;

    unsigned int m_coloursArrayUboOffset;
    unsigned int m_positionsArrayUboOffset;
    unsigned int m_scalesArrayUboOffset;

    FT_Library m_ft;
    Character m_characters[256];
    glm::mat4 m_uiProjectionMatrix;

    unsigned int m_blocksDiffuseTextureAtlas;
    unsigned int m_blocksDiffuseTextureAtlasNumberOfBlocks;

};

