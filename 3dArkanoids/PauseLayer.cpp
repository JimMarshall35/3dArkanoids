#include "PauseLayer.h"
#include "IRenderer.h"
#include "GameInput.h"

PauseLayer::PauseLayer(std::shared_ptr<IRenderer>& renderer, ToggleCursorFunc toggleCursor)
    :m_renderer(renderer), m_toggleCursor(toggleCursor)
{
}

void PauseLayer::Update(float deltaT)
{
}

bool PauseLayer::MasksPreviousUpdateableLayer() const
{
    return true;
}

std::string PauseLayer::GetUpdateableLayerName() const
{
    return "Pause";
}

void PauseLayer::OnUpdatePush()
{
}

void PauseLayer::OnUpdatePop()
{
}

void PauseLayer::ReceiveInput(const GameInput& input)
{
    if (input.PauseGame) {
        GameFramework::PopLayers(GameLayerType::Draw | GameLayerType::Input | GameLayerType::Update);
    }
    else if (input.ExitGame) {
        GameFramework::PopLayers(GameLayerType::Draw);
        GameFramework::PopLayers(GameLayerType::Draw | GameLayerType::Input | GameLayerType::Update);
        GameFramework::PopLayers(GameLayerType::Draw | GameLayerType::Input | GameLayerType::Update);
    }
}

bool PauseLayer::MasksPreviousInputLayer() const
{
    return true;
}

std::string PauseLayer::GetInputLayerName() const
{
    return "Pause";
}

void PauseLayer::OnInputPush()
{
    m_toggleCursor(CursorType::CURSOR_ENABLED);
}

void PauseLayer::OnInputPop()
{
    m_toggleCursor(CursorType::CURSOR_DISABLED);

}

void PauseLayer::Draw(const Camera& camera) const
{
    m_renderer->DrawTextAnchoredToCenter("Paused", 0, 0, 2.0f, { 0,0,0 });
}

bool PauseLayer::MasksPreviousDrawableLayer() const
{
    return false;
}

std::string PauseLayer::GetDrawableLayerName() const
{
    return "Pause";
}

void PauseLayer::OnDrawablePush()
{
}

void PauseLayer::OnDrawablePop()
{
}
