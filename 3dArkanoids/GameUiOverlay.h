#pragma once
#include "GameFramework.h"
#include <memory>

class IRenderer;
class GameUiOverlay
	:public DrawableLayerBase
{
public:
	GameUiOverlay(const std::shared_ptr<IRenderer>& renderer);
	// Inherited via DrawableLayerBase
	virtual void Draw(const Camera& camera) const override;
	virtual bool MasksPreviousDrawableLayer() const override;
	virtual std::string GetDrawableLayerName() const override;
private:
	std::shared_ptr<IRenderer> m_renderer;
};

