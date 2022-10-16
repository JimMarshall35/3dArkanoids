#pragma once
#include "GameFramework.h"
#include <memory>
#include "SystemAbstractionTypedefs.h"

class IRenderer;

class PauseLayer
	:public DrawableLayerBase,
	public RecieveInputLayerBase,
	public UpdateableLayerBase
{
public:
	PauseLayer(std::shared_ptr<IRenderer>& renderer, ToggleCursorFunc toggleCursor);
	// Inherited via UpdateableLayerBase
	virtual void Update(float deltaT) override;
	virtual bool MasksPreviousUpdateableLayer() const override;
	virtual std::string GetUpdateableLayerName() const override;
	virtual void OnUpdatePush() override;
	virtual void OnUpdatePop() override;

	// Inherited via RecieveInputLayerBase
	virtual void ReceiveInput(const GameInput& input) override;
	virtual bool MasksPreviousInputLayer() const override;
	virtual std::string GetInputLayerName() const override;
	virtual void OnInputPush() override;
	virtual void OnInputPop() override;

	// Inherited via DrawableLayerBase
	virtual void Draw(const Camera& camera) const override;
	virtual bool MasksPreviousDrawableLayer() const override;
	virtual std::string GetDrawableLayerName() const override;
	virtual void OnDrawablePush() override;
	virtual void OnDrawablePop() override;
private:
	std::shared_ptr<IRenderer> m_renderer;
	ToggleCursorFunc m_toggleCursor;
};

