#include "Renderer.h"
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include "Camera.h"
#include "BlockInstanceRenderData.h"
#include "ErrorHandling.h"
#define DRAW_DISTANCE 10000.0f

#include "PlayfieldDefs.h"

std::string colourVertGlsl =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"

"out vec3 FragPos;\n"
"out vec3 Normal;\n"

"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"

"void main()\n"
"{\n"
"    FragPos = vec3(model * vec4(aPos, 1.0));\n"
"    Normal = mat3(transpose(inverse(model))) * aNormal;\n"

"    gl_Position = projection * view * vec4(FragPos, 1.0);\n"
"}\n";

std::string colourFragGlsl =
"#version 330 core\n"
"out vec4 FragColor;\n"

"in vec3 Normal;\n"
"in vec3 FragPos;\n"

"uniform vec3 lightPos;\n"
"uniform vec3 viewPos;\n"
"uniform vec3 lightColor;\n"
"uniform vec3 objectColor;\n"

"void main()\n"
"{\n"
"    // ambient\n"
"    float ambientStrength = 0.1;\n"
"    vec3 ambient = ambientStrength * lightColor;\n"
"    // diffuse\n"
"    vec3 norm = normalize(Normal);\n"
"    vec3 lightDir = normalize(lightPos - FragPos);\n"
"    float diff = max(dot(norm, lightDir), 0.0);\n"
"    vec3 diffuse = diff * lightColor;\n"
"    // specular\n"
"    float specularStrength = 0.5;\n"
"    vec3 viewDir = normalize(viewPos - FragPos);\n"
"    vec3 reflectDir = reflect(-lightDir, norm);\n"
"    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
"    vec3 specular = specularStrength * spec * lightColor;\n"

"    vec3 result = (ambient + diffuse + specular) * objectColor;\n"
"    FragColor = vec4(result, 1.0);\n"
"}\n";



std::string instancedColourVertGlsl = 
"#version 460 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"
"#define MAX_INSTANCES " MAX_INSTANCES_STRING "\n"

"out vec3 FragPos;\n"
"out vec3 Normal;\n"
"out vec3 Colour;\n"

"layout(binding = 0) uniform InstanceData\n"
"{\n"
"    vec4 positions[MAX_INSTANCES];\n"
"    vec4 scales[MAX_INSTANCES];\n"
"    vec4 colours[MAX_INSTANCES];\n"
"};\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"

"void main()\n"
"{\n"
"   vec3 pos = positions[gl_InstanceID].xyz;\n"
"   vec3 scale = scales[gl_InstanceID].xyz;\n"
"   mat4 model = mat4(scale.x, 0.0,     0.0,     pos.x,  // 1. column\n"
"                     0.0,     scale.y, 0.0,     pos.y,  // 2. column\n"
"                     0.0,     0.0,     scale.z, pos.z,  // 3. column\n"
"                     0.0,     0.0,     0.0,     1.0);   // 4. column\n"

"    FragPos = vec3((scales[gl_InstanceID] * vec4(aPos, 1.0)) + positions[gl_InstanceID]);\n"
"    Normal = mat3(transpose(inverse(model))) * aNormal;\n"
"    Colour = colours[gl_InstanceID].xyz;\n"
"    // use the fourth element of the colour vector as a flag, if < 0 then draw outside the screen\n"
"    if(colours[gl_InstanceID].a < 0.0f){\n"
"       gl_Position = vec4(2.0, 2.0, 2.0, 1.0);\n"
"    }else{\n"
"       gl_Position = projection * view * vec4(FragPos, 1.0);\n"
"    }\n"
"}\n";

std::string instancedColourFragGlsl =
"#version 330 core\n"
"out vec4 FragColor;\n"

"in vec3 Normal;\n"
"in vec3 FragPos;\n"
"in vec3 Colour;\n"

"uniform vec3 lightPos;\n"
"uniform vec3 viewPos;\n"
"uniform vec3 lightColor;\n"

"void main()\n"
"{\n"
"    // ambient\n"
"    float ambientStrength = 0.1;\n"
"    vec3 ambient = ambientStrength * lightColor;\n"
"    // diffuse\n"
"    vec3 norm = normalize(Normal);\n"
"    vec3 lightDir = normalize(lightPos - FragPos);\n"
"    float diff = max(dot(norm, lightDir), 0.0);\n"
"    vec3 diffuse = diff * lightColor;\n"
"    // specular\n"
"    float specularStrength = 0.5;\n"
"    vec3 viewDir = normalize(viewPos - FragPos);\n"
"    vec3 reflectDir = reflect(-lightDir, norm);\n"
"    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
"    vec3 specular = specularStrength * spec * lightColor;\n"

"    vec3 result = (ambient + diffuse + specular) * Colour;\n"
"    FragColor = vec4(result, 1.0);\n"
"}\n";

Renderer::Renderer()
{
    Initialize();
}

Renderer::Renderer(int screenWidth, int screenHeight)
    :m_scrWidth(screenWidth), m_scrHeight(screenHeight)
{
    Initialize();
}



void Renderer::SetScreenDims(const glm::ivec2& value)
{
    m_scrWidth = value.x;
    m_scrHeight = value.y;
}

void PrintInfo() {
    GLint maxUBOSize;
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUBOSize);
    std::cout << "Max UBO Size : " << maxUBOSize << std::endl;
    float spaceForHowManyBlocksApprox = maxUBOSize / (sizeof(glm::vec4) + sizeof(glm::mat4));
    float spaceWithPosAndScale = maxUBOSize / (sizeof(glm::vec4) + sizeof(glm::vec4));
    float spaceWithJustPos = maxUBOSize / (sizeof(glm::vec4));
    std::cout << "Space for approximately " << spaceForHowManyBlocksApprox << " blocks with model matrix" << std::endl;
    std::cout << "Space for approximately " << spaceWithPosAndScale << " blocks with Pos and Scale" << std::endl;
    std::cout << "Space for approximately " << spaceWithJustPos << " blocks with Pos" << std::endl;


}

void Renderer::Initialize()
{
    PrintInfo();
    static const float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    unsigned int VBO;
    glGenVertexArrays(1, &m_unitCubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(m_unitCubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    m_colouredShader.LoadFromString(colourVertGlsl, colourFragGlsl);
    m_colouredInstancedShader.LoadFromString(instancedColourVertGlsl, instancedColourFragGlsl);
}

glm::mat4 PositionAndScaleToModelMatrix(const glm::vec3& pos, const glm::vec3& dimensions) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    //model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, dimensions);
    return model;
}

/// <summary>
/// Helper function to convert the array of BlockInstanceRenderData
/// to separate buffers of the format which the shaders will use.
/// Calculates model matrix at this stage, would be more memory efficient to do 
/// this on the shader and keep the instance position data as a vec3
/// </summary>
/// <param name="models"></param>
/// <param name="colours"></param>
/// <param name="instances"></param>
/// <param name="numberToDraw"></param>
void DeInterleaveInstanceDataArray(std::vector<glm::vec4>& positions, std::vector<glm::vec4>& scales, std::vector<glm::vec4>& coloursAndShouldDraw, const BlockInstanceRenderData* instances, const size_t numberToDraw) {
    for (size_t i = 0; i < numberToDraw; i++) {
        const auto& instance = instances[i];
        positions[i] = glm::vec4(instance.worldPos, 1.0);
        scales[i] = glm::vec4(instance.dims, 1.0);
        // use the fourth element of the colour vector as a boolean 
        // for the shader to save adding an extra element to the uniform block
        coloursAndShouldDraw[i] = glm::vec4(instance.colour, 
            instance.shouldRender ? 1.0 : -1.0); 
    }
}

/// <summary>
/// draws a basic cuboid (non instanced)
/// </summary>
/// <param name="pos"></param>
/// <param name="dimensions"></param>
/// <param name="camera"></param>
/// <param name="colour"></param>
void Renderer::DrawCuboid(const glm::vec3& pos, const glm::vec3& dimensions, const Camera& camera, const glm::vec3& colour)
{
    glm::mat4 model = PositionAndScaleToModelMatrix(pos, dimensions);
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)m_scrWidth / (float)m_scrHeight, 0.1f, DRAW_DISTANCE);

    // vert
    m_colouredShader.use();
    m_colouredShader.setMat4("model", model);
    m_colouredShader.setMat4("view", camera.GetViewMatrix());
    m_colouredShader.setMat4("projection", projection);

    // frag
    m_colouredShader.setVec3("viewPos", camera.Position);
    m_colouredShader.setVec3("lightPos", m_lightPos);
    m_colouredShader.setVec3("lightColor", m_lightColour);
    m_colouredShader.setVec3("objectColor", colour);

    glBindVertexArray(m_unitCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

/// <summary>
/// Draws instanced blocks from the data in m_blockInstanceDataUboHandle.
/// </summary>
/// <param name="numberToDraw">number to draw - should be constant</param>
/// <param name="camera"></param>
void Renderer::DrawInstancedBlocks(const size_t numberToDraw, const Camera& camera)
{
    GLClearErrors();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)m_scrWidth / (float)m_scrHeight, 0.1f, DRAW_DISTANCE);

    // vert
    m_colouredInstancedShader.use();
    m_colouredInstancedShader.setMat4("view", camera.GetViewMatrix());
    m_colouredInstancedShader.setMat4("projection", projection);
    // frag
    m_colouredInstancedShader.setVec3("viewPos", camera.Position);
    m_colouredInstancedShader.setVec3("lightPos", m_lightPos);
    m_colouredInstancedShader.setVec3("lightColor", m_lightColour);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_blockInstanceDataUboHandle);
    glBindVertexArray(m_unitCubeVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, numberToDraw);
}

/// <summary>
/// Sets an initial openGL uniform buffer of instanced rendering data for
/// the blocks that will be drawn when DrawInstancedBlocks is called.
/// </summary>
/// <param name="instances"></param>
/// <param name="numberToDraw"></param>
void Renderer::SetInstancedBlocksUbo(const BlockInstanceRenderData* instances, const size_t numberToDraw)
{
    // position and scale for each block, to be set by DeInterleaveInstanceDataArray
    auto positions = std::vector<glm::vec4>(numberToDraw);
    auto scales = std::vector<glm::vec4>(numberToDraw);

    // I can only get it to work properly with colour as a vec4 I think because of some gpu memory peculiarity- 
    // but we use the fourth element as a boolean to specify whether the block should be drawn
    // if > 0 draw else don't
    auto coloursAndShouldDraw = std::vector<glm::vec4>(numberToDraw);

    // sets models, coloursAndShouldDraw from the array of instances
    DeInterleaveInstanceDataArray(positions, scales, coloursAndShouldDraw, instances, numberToDraw);

    m_colouredInstancedShader.use();

    // ideally this ubo setting process would be a helper method in the Shader class... one day
    auto programHandle = m_colouredInstancedShader.ID;

    // get the total size of the InstanceData uniform block
    GLuint blockIndex = glGetUniformBlockIndex(programHandle, "InstanceData");
    GLint blockSize;
    glGetActiveUniformBlockiv(programHandle, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

    // setup temporary buffer that we will eventually 'glBufferData' into the ubo
    auto blockBuffer = std::vector<GLbyte>(blockSize);
    static const int numInstanceAttributes = 3;

    // get indices of the variables in the uniform buffer (both are arrays)
    const GLchar* names[] = { "positions", "scales", "colours" };
    GLuint indices[numInstanceAttributes];
    glGetUniformIndices(programHandle, numInstanceAttributes, names, indices);

    // get the starting offset for each array in the uniform buffer and cache
    // for use when changing the buffer during play
    GLint offset[numInstanceAttributes];
    glGetActiveUniformsiv(programHandle, numInstanceAttributes, indices, GL_UNIFORM_OFFSET, offset);
    m_positionsArrayUboOffset = offset[0];
    m_scalesArrayUboOffset = offset[1];
    m_coloursArrayUboOffset = offset[2];

    // copy the data to the staging array using the offsets we've gotten
    memcpy(blockBuffer.data() + m_positionsArrayUboOffset, positions.data(), numberToDraw * sizeof(glm::vec4));
    memcpy(blockBuffer.data() + m_scalesArrayUboOffset, scales.data(), numberToDraw * sizeof(glm::vec4));
    memcpy(blockBuffer.data() + m_coloursArrayUboOffset, coloursAndShouldDraw.data(), numberToDraw * sizeof(glm::vec4));

    // generate a new buffer to be the UBO and bind it to GL_UNIFORM_BUFFER
    if (!glIsBuffer(m_blockInstanceDataUboHandle)) {
        glGenBuffers(1, &m_blockInstanceDataUboHandle);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, m_blockInstanceDataUboHandle);

    // glBufferData the data from the staging array to the UBO 
    glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_blockInstanceDataUboHandle);
}

void Renderer::SetLightPos(const glm::vec3& value)
{
    m_lightPos = value;
}

void Renderer::SetLightColour(const glm::vec3& value)
{
    m_lightColour = value;
}

void Renderer::SetCubeShouldRender(size_t indexCubeIsAt, bool newValue)
{
    float ShouldDrawFlag = newValue ? 1.0f : -1.0f;
    glBindBuffer(GL_UNIFORM_BUFFER, m_blockInstanceDataUboHandle);
    glBufferSubData(GL_UNIFORM_BUFFER,
        m_coloursArrayUboOffset + (sizeof(glm::vec4) * indexCubeIsAt) + sizeof(float) * 3, // destination offset
        sizeof(float),                                                                     // bytes to copy
        &ShouldDrawFlag                                                                    // source
    );
}

void Renderer::SetCubePos(size_t indexCubeIsAt, const glm::vec3& newPos)
{
    auto newPosAsVec4 = glm::vec4(newPos, 1.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, m_blockInstanceDataUboHandle);
    glBufferSubData(
        GL_UNIFORM_BUFFER,
        m_positionsArrayUboOffset + (sizeof(glm::vec4) * indexCubeIsAt), // destination offset
        sizeof(glm::vec4),                                                 // bytes to copy
        &newPosAsVec4                                                             // source
    );
}

void Renderer::SetCubeColour(size_t indexCubeIsAt, const glm::vec3& newColour)
{
}
