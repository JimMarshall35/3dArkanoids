#include <iostream>
#include "GameUiOverlay.h"
#include "IRenderer.h"
#include <string>

GameUiOverlay::GameUiOverlay(const std::shared_ptr<IRenderer>& renderer)
    :m_renderer(renderer)
{
}


static std::string s_ballAngle;

void GameUiOverlay::Draw(const Camera& camera) const
{
    if (m_editorConnected) {
        m_renderer->DrawTextAnchoredToTopLeft("Ball angle with y: " + s_ballAngle, 0, 0, 0.5, { 0,0,0 });
        m_renderer->DrawTextAnchoredToTopRight("Editor Connected", 0, 0, 0.5, { 0,0,0 });
    }
    else {
        m_renderer->DrawTextAnchoredToTopLeft("Jim Marshall", 0, 0, 0.5, { 0,0,0 });
    }
}

bool GameUiOverlay::MasksPreviousDrawableLayer() const
{
    return false;
}

std::string GameUiOverlay::GetDrawableLayerName() const
{
    return "GameplayUI";
}

void GameUiOverlay::RecieveMessage(const GameToUiMessage& message)
{
    if (message.ballAngleWithYAxis != 0.0) {
        s_ballAngle = std::to_string(message.ballAngleWithYAxis);
        return; // temporary hack so that both editor connected and this work in harmony
    }
    m_editorConnected = message.editorConnected;
    
}

void GameUiOverlay::OnDrawablePush()
{
}

void GameUiOverlay::OnDrawablePop()
{
}
