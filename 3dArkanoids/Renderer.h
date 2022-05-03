#pragma once
#include "IRenderer.h"
#include "Shader.h"

class Renderer :
    public IRenderer
{
public:
    Renderer();
    Renderer(int screenWidth, int screenHeight);
    // Inherited via IRenderer
    virtual void DrawInstancedBlocks(const size_t numberToDraw, const Camera& camera) override;
    virtual void SetInstancedBlocksUbo(const BlockInstanceRenderData* instances, const size_t numberToSet) override;
    virtual void SetLightPos(const glm::vec3& value) override;
    virtual void SetLightColour(const glm::vec3& value) override;
    virtual void DrawCuboid(const glm::vec3& centeredAt, const glm::vec3& dimensions, const Camera& camera, const glm::vec3& colour) override;
    virtual void SetScreenDims(const glm::ivec2& value) override;
    virtual void SetCubeShouldRender(size_t indexCubeIsAt, bool newValue) override;
    virtual void SetCubePosAndScale(size_t indexCubeIsAt, const glm::vec3& newPos) override;
private:
    void Initialize();
private:
    unsigned int m_unitCubeVAO;
    Shader m_colouredShader;
    Shader m_colouredInstancedShader;
    glm::vec3 m_lightPos;
    glm::vec3 m_lightColour;
    unsigned int m_scrWidth;
    unsigned int m_scrHeight;
    unsigned int m_blockInstanceDataUboHandle;

    unsigned int m_coloursArrayUboOffset;
    unsigned int m_modelMatrixArrayUboOffset;
};

