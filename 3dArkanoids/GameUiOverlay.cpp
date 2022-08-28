#include <iostream>
#include "GameUiOverlay.h"
#include "IRenderer.h"

GameUiOverlay::GameUiOverlay(const std::shared_ptr<IRenderer>& renderer)
    :m_renderer(renderer)
{
}

void GameUiOverlay::Draw(const Camera& camera) const
{
    m_renderer->DrawTextAnchoredToTopLeft("Jim Marshall", 0, 0, 0.5, { 0,0,0 });
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
    std::cout << "ui recieved message: " << message.newScore << "\n";
}
