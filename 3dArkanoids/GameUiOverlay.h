#pragma once
#include <memory>
#include "GameFramework.h"
#include "GameToUiMessage.h"

class IRenderer;
class GameUiOverlay
	:public DrawableLayerBase,
	public GameFrameworkMessageRecipientBase<GameToUiMessage>
{
public:
	GameUiOverlay(const std::shared_ptr<IRenderer>& renderer);
	// Inherited via DrawableLayerBase
	virtual void Draw(const Camera& camera) const override;
	virtual bool MasksPreviousDrawableLayer() const override;
	virtual std::string GetDrawableLayerName() const override;
private:
	std::shared_ptr<IRenderer> m_renderer;

	// Inherited via GameFrameworkMessageRecipientBase
	virtual void RecieveMessage(const GameToUiMessage& message) override;
};

