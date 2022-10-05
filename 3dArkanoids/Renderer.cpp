#include "Renderer.h"
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include "Camera.h"
#include "BlockInstanceRenderData.h"
#include "ErrorHandling.h"
#define DRAW_DISTANCE 10000.0f
#define NUM_CHANNELS 4
#include "PlayfieldDefs.h"
#include "stb_image.h"
#include "GLTextureHelper.h"

#include <string.h>
#include "TextFileResourceListParser.h"

#pragma region colour shader

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
"uniform vec4 objectColor;\n"

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

"    vec3 result = (ambient + diffuse + specular) * objectColor.xyz;\n"
"    FragColor = vec4(result, objectColor[3]);\n"
"}\n";

#pragma endregion

#pragma region instanced colour shader

#pragma region vert

std::string instancedColourVertGlsl =
"#version 460 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"
"#define MAX_INSTANCES " MAX_INSTANCES_STRING "\n"

"out vec3 FragPos;\n"
"out vec3 Normal;\n"
"out vec4 Colour;\n"

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
"    Colour = colours[gl_InstanceID];\n"
"    // use the fourth element of the colour vector as a flag, if < 0 then draw outside the screen\n"
"    if(positions[gl_InstanceID][3] < 0.0f){\n"
"       gl_Position = vec4(2.0, 2.0, 2.0, 1.0);\n"
"    }else{\n"
"       gl_Position = projection * view * vec4(FragPos, 1.0);\n"
"    }\n"
"}\n";

#pragma endregion

#pragma region frag

std::string instancedColourFragGlsl =
"#version 330 core\n"
"out vec4 FragColor;\n"

"in vec3 Normal;\n"
"in vec3 FragPos;\n"
"in vec4 Colour;\n"

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

"    vec3 result = (ambient + diffuse + specular) * Colour.xyz;\n"
"    FragColor = vec4(result, Colour.a);\n"
"}\n";

#pragma endregion


#pragma endregion

#pragma region instanced textured shader

#pragma region vert

std::string instancedTexturedVertGlsl =
"#version 460 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"
"layout(location = 2) in vec2 aUv;\n"
"#define MAX_INSTANCES " MAX_INSTANCES_STRING "\n"

"out vec3 FragPos;\n"
"out vec3 Normal;\n"
"out vec2 Uv;\n"

"layout(binding = 0) uniform InstanceData\n"
"{\n"
"    vec4 positions[MAX_INSTANCES];\n"
"    vec4 scales[MAX_INSTANCES];\n"
"    vec4 uvOffsets[MAX_INSTANCES];\n"
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
"    Uv = aUv + uvOffsets[gl_InstanceID].xy;\n"
"    // use the fourth element of the colour vector as a flag, if < 0 then draw outside the screen\n"
"    if(positions[gl_InstanceID][3] < 0.0f){\n"
"       gl_Position = vec4(2.0, 2.0, 2.0, 1.0);\n"
"    }else{\n"
"       gl_Position = projection * view * vec4(FragPos, 1.0);\n"
"    }\n"
"}\n";

#pragma endregion

#pragma region frag

std::string instancedTexturedFragGlsl =
"#version 330 core\n"
"out vec4 FragColor;\n"

"in vec3 Normal;\n"
"in vec3 FragPos;\n"
"in vec2 Uv;\n"

"uniform vec3 lightPos;\n"
"uniform vec3 viewPos;\n"
"uniform vec3 lightColor;\n"
"uniform sampler2D diffuseAtlas;\n"

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
"    vec4 colour = texture(diffuseAtlas, Uv);\n"
"    FragColor = vec4((ambient + diffuse + specular),1.0) * colour;\n"
"}\n";

#pragma endregion

#pragma endregion

#pragma region text shader

std::string textVertGlsl =
"#version 330 core\n"
"layout(location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>\n"
"out vec2 TexCoords;\n"

"uniform mat4 projection;\n"

"void main()\n"
"{\n"
"    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
"    TexCoords = vertex.zw;\n"
"}\n";

std::string textFragGlsl =
"#version 330 core\n"
"in vec2 TexCoords;\n"
"out vec4 color;\n"

"uniform sampler2D text;\n"
"uniform vec3 textColor;\n"

"void main()\n"
"{\n"
"    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
"    color = vec4(textColor, 1.0) * sampled;\n"
"}\n";

#pragma endregion

#pragma region BillBoardShader

std::string billboardVertGlsl =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec2 aUv;\n"

"out vec2 Uv;\n"

"uniform vec3 CameraRight_worldspace;\n"
"uniform vec3 CameraUp_worldspace;\n"
"uniform vec3 particleCenter_wordspace;\n"
"uniform vec2 BillboardSize;\n"
"uniform mat4 VP; // Model-View-Projection matrix, but without the Model (the position is in BillboardPos; the orientation depends on the camera)\n"

"void main()\n"
"{\n"
    "vec4 vertexPosWorldSpace =\n"
    "vec4(particleCenter_wordspace\n"
    "+ CameraRight_worldspace * aPos.x * BillboardSize.x\n"
    "+ CameraUp_worldspace * aPos.y * BillboardSize.y, 1.0);\n"
    "gl_Position = VP * vertexPosWorldSpace;"
    "Uv = aUv;\n"
"}\n"
;

std::string billboardFragGlsl =
"#version 330 core\n"
"in vec2 Uv;\n"
"out vec4 color;\n"

"uniform sampler2D texture;\n"

"void main()\n"
"{\n"
"    vec4 sampled = vec4(texture(texture, Uv));\n"
"    color = vec4(sampled);\n"
"}\n"
;

#pragma endregion

Renderer::Renderer()
{
    Initialize();
}

#define use_CRT_SECURE_NO_WARNINGS 1
Renderer::Renderer(const RendererInitialisationData& initData)
    :m_scrWidth(initData.screenWidth), m_scrHeight(initData.screenHeight)
{
    LoadSpriteFromFile(initData.spritesFilePath);
    DesignateOneByTwoBlocksTexture(initData.oneByTwoBlocksTextureIdentifier, initData.numBlocksInTexture);
    Initialize();
    
}


/// <summary>
/// should be called by screen sized change callback
/// </summary>
/// <param name="value"></param>
void Renderer::SetScreenDims(const glm::ivec2& value)
{
    m_scrWidth = value.x;
    m_scrHeight = value.y;
    m_uiProjectionMatrix = glm::ortho(0.0f, static_cast<float>(m_scrWidth), 0.0f, static_cast<float>(m_scrHeight));
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

struct TexturedVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

/// <summary>
/// get ready to render!
/// </summary>
void Renderer::Initialize()
{
    PrintInfo();

    InitializeColouredCubeVertices();
    InitializeSphereVertices();
    InitializeTexturedOneByTwoCubeVertices();
    InitialiseBillboardVertices();

    // load all shaders
    m_colouredShader.LoadFromString(colourVertGlsl, colourFragGlsl);
    m_colouredInstancedShader.LoadFromString(instancedColourVertGlsl, instancedColourFragGlsl);
    m_textShader.LoadFromString(textVertGlsl, textFragGlsl);
    m_texturedInstancedShader.LoadFromString(instancedTexturedVertGlsl, instancedTexturedFragGlsl);
    m_billboardShader.LoadFromString(billboardVertGlsl, billboardFragGlsl);


    // load fonts
    InitFT();
    LoadFont("C:\\Users\\james.marshall\\source\\repos\\3dArkanoids\\3dArkanoids\\fonts\\OpenSans-Bold.ttf"); // todo - make file system class

    // initialise the projection matrix for the ui
    SetScreenDims({ m_scrWidth, m_scrHeight });
}

/// <summary>
/// initialise freetype library and vertices used
/// for text drawing.
/// </summary>
void Renderer::InitFT()
{
    if (FT_Init_FreeType(&m_ft))
    {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    // configure VAO/VBO for texture quads
    // -----------------------------------
    glGenVertexArrays(1, &m_freeTypeVAO);
    glGenBuffers(1, &m_freeTypeVBO);
    glBindVertexArray(m_freeTypeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_freeTypeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

/// <summary>
/// load a ttf format font
/// </summary>
/// <param name="ttfFilePath">path to font</param>
void Renderer::LoadFont(std::string ttfFilePath)
{
    FT_Face face;
    if (FT_New_Face(m_ft, ttfFilePath.c_str(), 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, m_baseTextSize);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
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
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            m_characters[c] = character;
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(m_ft);
}

/// <summary>
/// Load the sphere vertices into openGL memory
/// and set m_unitSphereVAO in the process.
/// </summary>
void Renderer::InitializeSphereVertices()
{
    unsigned int VBO;
    glGenVertexArrays(1, &m_unitSphereVAO);
    glGenBuffers(1, &VBO);

    glGenBuffers(1, &m_unitSphereEBO);


    glBindVertexArray(m_unitSphereVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_sphere.getInterleavedVertexSize(), m_sphere.getInterleavedVertices(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unitSphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_sphere.getIndexSize(), m_sphere.getIndices(), GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_sphere.getInterleavedStride(), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, m_sphere.getInterleavedStride(), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


}

void Renderer::InitializeTexturedOneByTwoCubeVertices()
{
    const auto smallSquareTopLeftU = 0.0f;
    const auto smallSquareTopLeftV = 1.0f;

    const auto smallSquareTopRightU = 1.0f / 3.0f;
    const auto smallSquareTopRightV = 1.0f;

    const auto smallSquareBottomLeftU = 0.0f;
    const auto smallSquareBottomLeftV = 1.0f - (1.0f / (float)m_blocksDiffuseTextureAtlasNumberOfBlocks);

    const auto smallSquareBottomRightU = 1.0f / 3.0f;
    const auto smallSquareBottomRightV = 1.0f - (1.0f / (float)m_blocksDiffuseTextureAtlasNumberOfBlocks);

    const auto largeSquareTopLeftU = smallSquareTopRightU;
    const auto largeSquareTopLeftV = smallSquareTopRightV;

    const auto largeSquareBottomLeftU = smallSquareBottomRightU;
    const auto largeSquareBottomLeftV = smallSquareBottomRightV;

    const auto largeSquareTopRightU = 1.0f;
    const auto largeSquareTopRightV = 1.0f;

    const auto largeSquareBottomRightU = 1.0f;
    const auto largeSquareBottomRightV = 1.0f - (1.0f / (float)m_blocksDiffuseTextureAtlasNumberOfBlocks);





    static const TexturedVertex texturedVerts[] = {
        /*
        *  (texture)                                     front
        *                                                top
        *                    ______ ____________         ^
        *                   |      |            |  front |       back
        *                   |      |            |   left |       right
        *                   |______|____________|        |______>
        *                                             bottom
        *                                             back
        */
        // bottom face
        {{-0.5f, -0.5f, -0.5f},  { 0.0f,  0.0f, -1.0f}, {largeSquareBottomLeftU,  largeSquareBottomLeftV}},  // left,  back,  bottom
        {{ 0.5f, -0.5f, -0.5f},  { 0.0f,  0.0f, -1.0f}, {largeSquareBottomRightU, largeSquareBottomRightV}}, // right, back,  bottom
        {{ 0.5f,  0.5f, -0.5f},  { 0.0f,  0.0f, -1.0f}, {largeSquareTopRightU,    largeSquareTopRightV}},    // right, front, bottom

        {{ 0.5f,  0.5f, -0.5f},  { 0.0f,  0.0f, -1.0f}, {largeSquareTopRightU,   largeSquareTopRightV}},     // right, front, bottom
        {{-0.5f,  0.5f, -0.5f},  { 0.0f,  0.0f, -1.0f}, {largeSquareTopLeftU,    largeSquareTopLeftV}},      // left,  front, bottom
        {{-0.5f, -0.5f, -0.5f},  { 0.0f,  0.0f, -1.0f}, {largeSquareBottomLeftU, largeSquareBottomLeftV}},   // left,  back,  bottom

        // top face
        {{-0.5f, -0.5f,  0.5f},  { 0.0f,  0.0f,  1.0f}, {largeSquareBottomLeftU,  largeSquareBottomLeftV}},  // left,  back,  top
        {{ 0.5f, -0.5f,  0.5f},  { 0.0f,  0.0f,  1.0f}, {largeSquareBottomRightU, largeSquareBottomRightV}}, // right, back,  top
        {{ 0.5f,  0.5f,  0.5f},  { 0.0f,  0.0f,  1.0f}, {largeSquareTopRightU,    largeSquareTopRightV}},    // right, front, top

        {{ 0.5f,  0.5f,  0.5f},  { 0.0f,  0.0f,  1.0f}, {largeSquareTopRightU,    largeSquareTopRightV}},     // right, front, top
        {{-0.5f,  0.5f,  0.5f},  { 0.0f,  0.0f,  1.0f}, {largeSquareTopLeftU,     largeSquareTopLeftV}},      // left,  front, top
        {{-0.5f, -0.5f,  0.5f},  { 0.0f,  0.0f,  1.0f}, {largeSquareBottomLeftU,  largeSquareBottomLeftV}},   // left,  back,  top

        // left face
        {{-0.5f,  0.5f,  0.5f},  {-1.0f,  0.0f,  0.0f}, {smallSquareTopLeftU,     smallSquareTopLeftV}},     // left,  front, top
        {{-0.5f,  0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f}, {smallSquareBottomLeftU,  smallSquareBottomLeftV}},  // left,  front, bottom
        {{-0.5f, -0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f}, {smallSquareBottomRightU, smallSquareBottomRightV}}, // left,  back,  bottom

        {{-0.5f, -0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f}, {smallSquareBottomRightU, smallSquareBottomRightV}}, // left,  back,  bottom
        {{-0.5f, -0.5f,  0.5f},  {-1.0f,  0.0f,  0.0f}, {smallSquareTopRightU,    smallSquareTopRightV}},    // left,  back,  top
        {{-0.5f,  0.5f,  0.5f},  {-1.0f,  0.0f,  0.0f}, {smallSquareTopLeftU,     smallSquareTopLeftV}},     // left,  front, top

        // right face
        {{ 0.5f,  0.5f,  0.5f},  { 1.0f,  0.0f,  0.0f}, {smallSquareTopLeftU,     smallSquareTopLeftV}},     // right, front, top
        {{ 0.5f,  0.5f, -0.5f},  { 1.0f,  0.0f,  0.0f}, {smallSquareBottomLeftU,  smallSquareBottomLeftV}},  // right, front, bottom
        {{ 0.5f, -0.5f, -0.5f},  { 1.0f,  0.0f,  0.0f}, {smallSquareBottomRightU, smallSquareBottomRightV}}, // right, back,  bottom

        {{ 0.5f, -0.5f, -0.5f},  { 1.0f,  0.0f,  0.0f}, {smallSquareBottomRightU, smallSquareBottomRightV}}, // right, back,  bottom
        {{ 0.5f, -0.5f,  0.5f},  { 1.0f,  0.0f,  0.0f}, {smallSquareTopRightU,    smallSquareTopRightV}},    // right, back,  top
        {{ 0.5f,  0.5f,  0.5f},  { 1.0f,  0.0f,  0.0f}, {smallSquareTopLeftU,     smallSquareTopLeftV}},     // right, front, top

        // back face
        {{-0.5f, -0.5f, -0.5f},  { 0.0f, -1.0f,  0.0f}, {largeSquareBottomLeftU,  largeSquareBottomLeftV}},  // left,  back,  bottom
        {{ 0.5f, -0.5f, -0.5f},  { 0.0f, -1.0f,  0.0f}, {largeSquareBottomRightU, largeSquareBottomRightV}}, // right, back,  bottom
        {{ 0.5f, -0.5f,  0.5f},  { 0.0f, -1.0f,  0.0f}, {largeSquareTopRightU,    largeSquareTopRightV}},    // right, back, top

        {{ 0.5f, -0.5f,  0.5f},  { 0.0f, -1.0f,  0.0f}, {largeSquareTopRightU,    largeSquareTopRightV}},    // right, back, top
        {{-0.5f, -0.5f,  0.5f},  { 0.0f, -1.0f,  0.0f}, {largeSquareTopLeftU,     largeSquareTopLeftV}},     // left,  back, top
        {{-0.5f, -0.5f, -0.5f},  { 0.0f, -1.0f,  0.0f}, {largeSquareBottomLeftU, largeSquareBottomLeftV}},   // left,  back, bottom

        // front face
        {{-0.5f,  0.5f, -0.5f},  { 0.0f,  1.0f,  0.0f}, {largeSquareBottomLeftU,  largeSquareBottomLeftV}},  // left,  front, bottom
        {{ 0.5f,  0.5f, -0.5f},  { 0.0f,  1.0f,  0.0f}, {largeSquareBottomRightU, largeSquareBottomRightV}}, // right, front, bottom
        {{ 0.5f,  0.5f,  0.5f},  { 0.0f,  1.0f,  0.0f}, {largeSquareTopRightU,    largeSquareTopRightV}},    // right, front, top

        {{ 0.5f,  0.5f,  0.5f},  { 0.0f,  1.0f,  0.0f}, {largeSquareTopRightU,    largeSquareTopRightV}},    // right, front, top
        {{-0.5f,  0.5f,  0.5f},  { 0.0f,  1.0f,  0.0f}, {largeSquareTopLeftU,     largeSquareTopLeftV}},     // left,  front, top
        {{-0.5f,  0.5f, -0.5f},  { 0.0f,  1.0f,  0.0f}, {largeSquareBottomLeftU, largeSquareBottomLeftV}},   // left,  front, bottom
    };

    //m_oneByTwoBlockVBO
    glGenVertexArrays(1, &m_oneByTwoBlockVAO);
    glGenBuffers(1, &m_oneByTwoBlockVBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_oneByTwoBlockVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texturedVerts), texturedVerts, GL_STATIC_DRAW);

    glBindVertexArray(m_oneByTwoBlockVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)offsetof(TexturedVertex, position));
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)offsetof(TexturedVertex, normal));
    glEnableVertexAttribArray(1);

    // uv attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)offsetof(TexturedVertex, uv));
    glEnableVertexAttribArray(2);
}

void Renderer::InitializeColouredCubeVertices()
{
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
}

void Renderer::InitialiseBillboardVertices()
{
    const float unitSquareVerts[30] = {
        -0.5f, 0.5f, 0.0f,    0.0f, 1.0f, // top left
        0.5f, 0.5f, 0.0f,     1.0f, 1.0f, // top right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left

        0.5f, 0.5f, 0.0f,     1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,    1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
    };
    glGenVertexArrays(1, &m_billboardVAO);
    glGenBuffers(1, &m_billboardVBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_billboardVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(unitSquareVerts), unitSquareVerts, GL_STATIC_DRAW);

    glBindVertexArray(m_billboardVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // uv attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    stbi_set_flip_vertically_on_load(true);
    int img_w, img_h, n;
    const auto data = stbi_load("heart-png-31.png", &img_w, &img_h, &n, NUM_CHANNELS);

    OpenGlGPULoadTexture(data, img_w, img_h, &m_testTexture);
    stbi_image_free(data);

}

void Renderer::LoadSpriteFromFile(const std::string& filePath)
{
    using namespace std;
    stbi_set_flip_vertically_on_load(1);
    ParseResourcesTextFile(filePath, [this](string path, string identifier) {
        LoadedSprite sprite;
        int channels;
        auto loadedData = stbi_load(path.c_str(), &sprite.widthPx, &sprite.heightPx, &channels, 4);
        if (loadedData != nullptr) {
            OpenGlGPULoadTexture(loadedData, sprite.widthPx, sprite.heightPx, &sprite.spriteId);
            m_loadedSprites[identifier] = sprite;
        }
        else {
            std::cout << "couldn't load " << path << '\n';
        }
        stbi_image_free(loadedData);
    });
}

#define RUNTIME_CHECK_SPRITE_IDENTIFIER

void Renderer::DrawBillboard(const glm::vec3& woldPos, const glm::vec2& billboardSize, const Camera& camera, const std::string& identifier) const
{
    using namespace glm;

#ifdef RUNTIME_CHECK_SPRITE_IDENTIFIER
    if (m_loadedSprites.find(identifier) == m_loadedSprites.end()) {
        std::cout << "sprite " << identifier << " not found\n";
        return;
    }
#endif // RUNTIME_CHECK_SPRITE_IDENTIFIER

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_loadedSprites.at(identifier).spriteId);

    m_billboardShader.use();
    /*
    uniform vec3 CameraRight_worldspace;\n"
    uniform vec3 CameraUp_worldspace;\n"
    uniform vec3 particleCenter_wordspace;\n"
    uniform vec2 BillboardSize;\n"
    */
    auto ViewMatrix = camera.GetViewMatrix();
    auto CameraRight_worldspace = vec3{ ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0] };
    auto CameraUp_worldspace = vec3{ ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1] };

    m_billboardShader.setVec3("CameraRight_worldspace", CameraRight_worldspace);
    m_billboardShader.setVec3("CameraUp_worldspace", CameraUp_worldspace);

    m_billboardShader.setVec3("particleCenter_wordspace", woldPos);
    m_billboardShader.setVec2("BillboardSize", billboardSize);
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)m_scrWidth / (float)m_scrHeight, 0.1f, DRAW_DISTANCE);
    auto vp = projection * ViewMatrix;
    m_billboardShader.setMat4("VP", vp);
    
    glBindVertexArray(m_billboardVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
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
void DeInterleaveInstanceDataArrayForColouredShader(std::vector<glm::vec4>& positionsAndShouldDraw, std::vector<glm::vec4>& scales, std::vector<glm::vec4>& colours, const BlockInstanceRenderData* instances, const size_t numberToDraw) {
    for (size_t i = 0; i < numberToDraw; i++) {
        const auto& instance = instances[i];
        positionsAndShouldDraw[i] = glm::vec4(instance.worldPos, instance.shouldRender ? 1.0 : -1.0);
        scales[i] = glm::vec4(instance.dims, 1.0);
        // use the fourth element of the colour vector as a boolean 
        // for the shader to save adding an extra element to the uniform block
        colours[i] = instance.colour;
    }
}

void DeInterleaveInstanceDataArrayForTexturexShader(std::vector<glm::vec4>& positionsAndShouldDraw, std::vector<glm::vec4>& scales, std::vector<glm::vec4>& uvOffsets, const BlockInstanceRenderData* instances, const size_t numberToDraw) {
    for (size_t i = 0; i < numberToDraw; i++) {
        const auto& instance = instances[i];
        positionsAndShouldDraw[i] = glm::vec4(instance.worldPos, instance.shouldRender ? 1.0 : -1.0);
        scales[i] = glm::vec4(instance.dims, 1.0);
        // use the fourth element of the colour vector as a boolean 
        // for the shader to save adding an extra element to the uniform block
        uvOffsets[i] = glm::vec4(instance.uvOffset, 0,0);
    }
}

/// <summary>
/// draws a basic cuboid (non instanced)
/// </summary>
/// <param name="pos"></param>
/// <param name="dimensions"></param>
/// <param name="camera"></param>
/// <param name="colour"></param>
void Renderer::DrawCuboid(const glm::vec3& pos, const glm::vec3& dimensions, const Camera& camera, const glm::vec3& colour) const
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
    m_colouredShader.setVec4("objectColor", { colour.x, colour.y, colour.z, 1.0f });

    glBindVertexArray(m_unitCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Renderer::DrawSphere(const glm::vec3& pos, const glm::vec3& dimensions, const Camera& camera, const glm::vec4& colour) const
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
    m_colouredShader.setVec4("objectColor", colour);

    glBindVertexArray(m_unitSphereVAO);
    glDrawElements(GL_TRIANGLES, m_sphere.getIndexCount(), GL_UNSIGNED_INT, 0);

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

void Renderer::DrawTextAnchoredToBottomLeft(std::string text, float x, float y, float scale, glm::vec3 colour) const
{
    m_textShader.use();
    m_textShader.setVec3("textColour", colour);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_scrWidth), 0.0f, static_cast<float>(m_scrHeight));
    m_textShader.setMat4("projection", projection);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_freeTypeVAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = m_characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, m_freeTypeVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

}

void Renderer::DrawTextAnchoredToTopLeft(std::string text, float xOffset, float yOffset, float scale, glm::vec3 colour) const
{
    DrawTextAnchoredToBottomLeft(text, xOffset, m_scrHeight - m_baseTextSize * scale - yOffset, scale, colour);
}

void Renderer::DrawTextAnchoredToTopRight(std::string text, float xOffset, float yOffset, float scale, glm::vec3 colour) const
{
    unsigned int totalAdvance = 0;
    for (char c : text) {
        const auto& character = m_characters[c];
        totalAdvance += character.Advance >> 6;
    }
    DrawTextAnchoredToTopLeft(text, m_scrWidth - totalAdvance * scale, 0, scale, colour);
}

void Renderer::DrawTextAnchoredToBottomRight(std::string text, float xOffset, float yOffset, float scale, glm::vec3 colour) const
{
    unsigned int totalAdvance = 0;
    for (char c : text) {
        const auto& character = m_characters[c];
        totalAdvance += character.Advance >> 6;
    }
    DrawTextAnchoredToBottomLeft(text, m_scrWidth - totalAdvance * scale, 0, scale, colour);
}


/// <summary>
/// Sets an initial openGL uniform buffer of instanced rendering data for
/// the blocks that will be drawn when DrawInstancedBlocks is called.
/// </summary>
/// <param name="instances"></param>
/// <param name="numberToDraw"></param>
void Renderer::SetInstancedBlocksUbo(const BlockInstanceRenderData* instances, const size_t numberToDraw)
{
    // position and scale for each block, to be set by DeInterleaveInstanceDataArray.
    // Fourth element of positions is whether the block should be drawn. If so it is > 0 else < 0.
    auto positionsAndShouldDraw = std::vector<glm::vec4>(numberToDraw);
    auto scales = std::vector<glm::vec4>(numberToDraw);


    auto colours = std::vector<glm::vec4>(numberToDraw);

    // sets models, coloursAndShouldDraw from the array of instances
    DeInterleaveInstanceDataArrayForColouredShader(positionsAndShouldDraw, scales, colours, instances, numberToDraw);

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
    m_instancedColouredPositionsArrayUboOffset = offset[0];
    m_instancedColouredScalesArrayUboOffset = offset[1];
    m_instancedColouredColoursArrayUboOffset = offset[2];

    // copy the data to the staging array using the offsets we've gotten
    memcpy(blockBuffer.data() + m_instancedColouredPositionsArrayUboOffset, positionsAndShouldDraw.data(), numberToDraw * sizeof(glm::vec4));
    memcpy(blockBuffer.data() + m_instancedColouredScalesArrayUboOffset, scales.data(), numberToDraw * sizeof(glm::vec4));
    memcpy(blockBuffer.data() + m_instancedColouredPositionsArrayUboOffset, colours.data(), numberToDraw * sizeof(glm::vec4));

    // generate a new buffer to be the UBO and bind it to GL_UNIFORM_BUFFER
    if (!glIsBuffer(m_blockInstanceDataUboHandle)) {
        glGenBuffers(1, &m_blockInstanceDataUboHandle);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, m_blockInstanceDataUboHandle);

    // glBufferData the data from the staging array to the UBO 
    glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_blockInstanceDataUboHandle);
}

void Renderer::SetInstancedTexturedBlocksUbo(const BlockInstanceRenderData* instances, const size_t numberToDraw)
{
    // position and scale for each block, to be set by DeInterleaveInstanceDataArray.
    // Fourth element of positions is whether the block should be drawn. If so it is > 0 else < 0.
    auto positionsAndShouldDraw = std::vector<glm::vec4>(numberToDraw);
    auto scales = std::vector<glm::vec4>(numberToDraw);


    auto uvOffsets = std::vector<glm::vec4>(numberToDraw);

    // sets models, coloursAndShouldDraw from the array of instances
    DeInterleaveInstanceDataArrayForTexturexShader(positionsAndShouldDraw, scales, uvOffsets, instances, numberToDraw);

    m_texturedInstancedShader.use();

    // ideally this ubo setting process would be a helper method in the Shader class... one day
    auto programHandle = m_texturedInstancedShader.ID;

    // get the total size of the InstanceData uniform block
    GLuint blockIndex = glGetUniformBlockIndex(programHandle, "InstanceData");
    GLint blockSize;
    glGetActiveUniformBlockiv(programHandle, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

    // setup temporary buffer that we will eventually 'glBufferData' into the ubo
    auto blockBuffer = std::vector<GLbyte>(blockSize);
    static const int numInstanceAttributes = 3;

    // get indices of the variables in the uniform buffer (both are arrays)
    const GLchar* names[] = { "positions", "scales", "uvOffsets" };
    GLuint indices[numInstanceAttributes];
    glGetUniformIndices(programHandle, numInstanceAttributes, names, indices);

    // get the starting offset for each array in the uniform buffer and cache
    // for use when changing the buffer during play
    GLint offset[numInstanceAttributes];
    glGetActiveUniformsiv(programHandle, numInstanceAttributes, indices, GL_UNIFORM_OFFSET, offset);
    m_instancedTexturedPositionsArrayUboOffset = offset[0];
    m_instancedTexturedScalesArrayUboOffset = offset[1];
    m_instancedTexturedUvOffsetsArrayUboOffset = offset[2];

    // copy the data to the staging array using the offsets we've gotten
    memcpy(blockBuffer.data() + m_instancedTexturedPositionsArrayUboOffset, positionsAndShouldDraw.data(), numberToDraw * sizeof(glm::vec4));
    memcpy(blockBuffer.data() + m_instancedTexturedScalesArrayUboOffset, scales.data(), numberToDraw * sizeof(glm::vec4));
    memcpy(blockBuffer.data() + m_instancedTexturedUvOffsetsArrayUboOffset, uvOffsets.data(), numberToDraw * sizeof(glm::vec4));

    // generate a new buffer to be the UBO and bind it to GL_UNIFORM_BUFFER
    if (!glIsBuffer(m_texturedBlockInstanceDataUboHandle)) {
        glGenBuffers(1, &m_texturedBlockInstanceDataUboHandle);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, m_texturedBlockInstanceDataUboHandle);

    // glBufferData the data from the staging array to the UBO 
    glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_texturedBlockInstanceDataUboHandle);
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
    glBindBuffer(GL_UNIFORM_BUFFER, m_texturedBlockInstanceDataUboHandle);
    glBufferSubData(GL_UNIFORM_BUFFER,
        m_instancedTexturedPositionsArrayUboOffset + (sizeof(glm::vec4) * indexCubeIsAt) + sizeof(float) * 3, // destination offset
        sizeof(float),                                                                     // bytes to copy
        &ShouldDrawFlag                                                                    // source
    );
}

void Renderer::SetCubePos(size_t indexCubeIsAt, const glm::vec3& newPos)
{
    auto newPosAsVec4 = glm::vec4(newPos, 1.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, m_texturedBlockInstanceDataUboHandle);
    glBufferSubData(
        GL_UNIFORM_BUFFER,
        m_instancedTexturedPositionsArrayUboOffset + (sizeof(glm::vec4) * indexCubeIsAt), // destination offset
        sizeof(glm::vec4),                                                 // bytes to copy
        &newPosAsVec4                                                             // source
    );
}

void Renderer::SetCubeColour(size_t indexCubeIsAt, const glm::vec3& newColour)
{
}

void Renderer::DesignateOneByTwoBlocksTexture(const std::string& identifier, int numBlocks)
{
    if (m_loadedSprites.find(identifier) == m_loadedSprites.end()) {
        std::cout << "One by two block texture " << identifier << "not found\n";
        return;
    }
    const auto& sprite = m_loadedSprites[identifier];
    if (sprite.spriteId == 0) {
        std::cout << "sprite with identifier " << identifier << " not properly loaded\n";
    }
    m_blocksDiffuseTextureAtlasNumberOfBlocks = numBlocks;
    m_blocksDiffuseTextureAtlas = sprite.spriteId;

}

void Renderer::DrawTexturedOneByTwoInstancedBlocks(const size_t numberToDraw, const Camera& camera)
{
    GLClearErrors();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)m_scrWidth / (float)m_scrHeight, 0.1f, DRAW_DISTANCE);

    // vert
    m_texturedInstancedShader.use();
    m_texturedInstancedShader.setMat4("view", camera.GetViewMatrix());
    m_texturedInstancedShader.setMat4("projection", projection);
    // frag
    m_texturedInstancedShader.setVec3("viewPos", camera.Position);
    m_texturedInstancedShader.setVec3("lightPos", m_lightPos);
    m_texturedInstancedShader.setVec3("lightColor", m_lightColour);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_blocksDiffuseTextureAtlas);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_texturedBlockInstanceDataUboHandle);
    glBindVertexArray(m_oneByTwoBlockVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, numberToDraw);
}

glm::vec2 Renderer::getUvOffsetToNextOneByTwoBlock()
{
    // this is needed because the renderer knows the number of blocks in the 
    // texture atlas and this is needed by the game class in the form
    // of an offset that when added to a uv coord sets it exactly one block down
    return glm::vec2(0.0f,-(1.0f/(float)m_blocksDiffuseTextureAtlasNumberOfBlocks));
}



