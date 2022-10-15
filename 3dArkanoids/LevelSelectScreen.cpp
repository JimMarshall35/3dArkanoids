#include "LevelSelectScreen.h"
#include "TextFileResourceListParser.h"
#include "IRenderer.h"
#include <iostream>
#include <glm/glm.hpp>
#include "GameInput.h"
#include "SerializationFunctions.h"
#include "game.h"

LevelSelectScreen::LevelSelectScreen(const std::shared_ptr<IRenderer>& renderer, ToggleCursorFunc toggleCursor, Game* game)
    :m_renderer(renderer), m_toggleCursor(toggleCursor), m_game(game)
{
    using namespace std;
    ParseResourcesTextFile("levels.txt", [this](string path, string identifier) {
        m_levelIdentifierToLevelFilePath[identifier] = path;
    });
}

void LevelSelectScreen::Update(float deltaT)
{
    if (m_shouldLoadNewLevelNow) {
        m_shouldLoadNewLevelNow = false;
        if (m_levelIdentifierToLevelFilePath.find(m_hovveredLevel) != m_levelIdentifierToLevelFilePath.end()) {
            LoadSerializableFromSingleBigBinary(m_levelIdentifierToLevelFilePath[m_hovveredLevel]);
            m_game->Init();
            GameFramework::PushLayers("Gameplay",
                GameLayerType::Draw |
                GameLayerType::Update |
                GameLayerType::Input);

            GameFramework::PushLayers("GameplayUI",
                GameLayerType::Draw);
        }
    }
}

bool LevelSelectScreen::MasksPreviousUpdateableLayer() const
{
    return true;
}

std::string LevelSelectScreen::GetUpdateableLayerName() const
{
    return "LevelSelect";
}

void LevelSelectScreen::Draw(const Camera& camera) const
{
    glm::vec2 start = { 0,0 };
    auto baseTextSize = m_renderer->GetBaseTextSize();
    for (const auto& pair : m_levelIdentifierToLevelFilePath) {
        glm::vec3 colour = pair.first == m_hovveredLevel ? glm::vec3{ 1.0, 1.0, 0.0 } : glm::vec3{ 0,0,0 };

        m_renderer->DrawTextAnchoredToTopLeft(
            pair.first,
            start.x,
            start.y,
            1.0,
            colour);
        start.y += baseTextSize;
    }
}

bool LevelSelectScreen::MasksPreviousDrawableLayer() const
{
    return true;
}

std::string LevelSelectScreen::GetDrawableLayerName() const
{
    return "LevelSelect";
}

void LevelSelectScreen::ReceiveInput(const GameInput& input)
{
    if (input.Firing) {
        m_shouldLoadNewLevelNow = true;
        return;
    }
    glm::vec2 start = { 0,0 };
    auto baseTextSize = m_renderer->GetBaseTextSize();
    m_hovveredLevel = "";
    for (const auto& pair : m_levelIdentifierToLevelFilePath) {
        //auto topLeft = pair.
        if (input.yPos >= start.y && input.yPos <= start.y + baseTextSize) {
            if (input.xPos >= start.x && input.xPos <= start.x + m_renderer->GetTextWidth(1.0, pair.first)) {
                m_hovveredLevel = pair.first;
                break;
            }
        }
        start.y += baseTextSize;
    }

}

bool LevelSelectScreen::MasksPreviousInputLayer() const
{
    return true;
}

std::string LevelSelectScreen::GetInputLayerName() const
{
    return "LevelSelect";
}

void LevelSelectScreen::OnUpdatePush()
{
}

void LevelSelectScreen::OnUpdatePop()
{
}

void LevelSelectScreen::OnDrawablePush()
{
}

void LevelSelectScreen::OnDrawablePop()
{
}

void LevelSelectScreen::OnInputPush()
{
    m_toggleCursor(CursorType::CURSOR_ENABLED);
}

void LevelSelectScreen::OnInputPop()
{
}
