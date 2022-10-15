#pragma once
#include "GameFramework.h"
#include "SystemAbstractionTypedefs.h"
#include <vector>
#include <memory>

class IRenderer;
class Game;

class LevelSelectScreen
	:public UpdateableLayerBase,
	public DrawableLayerBase,
	public RecieveInputLayerBase
{
public:
	LevelSelectScreen(const std::shared_ptr<IRenderer>& renderer, ToggleCursorFunc toggleCursor, Game* game);

	// Inherited via UpdateableLayerBase
	virtual void Update(float deltaT) override;
	virtual bool MasksPreviousUpdateableLayer() const override;
	virtual std::string GetUpdateableLayerName() const override;

	// Inherited via DrawableLayerBase
	virtual void Draw(const Camera& camera) const override;
	virtual bool MasksPreviousDrawableLayer() const override;
	virtual std::string GetDrawableLayerName() const override;

	// Inherited via RecieveInputLayerBase
	virtual void ReceiveInput(const GameInput& input) override;
	virtual bool MasksPreviousInputLayer() const override;
	virtual std::string GetInputLayerName() const override;

	// Inherited via UpdateableLayerBase
	virtual void OnUpdatePush() override;
	virtual void OnUpdatePop() override;

	// Inherited via DrawableLayerBase
	virtual void OnDrawablePush() override;
	virtual void OnDrawablePop() override;

	// Inherited via RecieveInputLayerBase
	virtual void OnInputPush() override;
	virtual void OnInputPop() override;
private:
	std::map<std::string, std::string> m_levelIdentifierToLevelFilePath;
	std::shared_ptr<IRenderer> m_renderer;
	ToggleCursorFunc m_toggleCursor;
	std::string m_hovveredLevel;
	bool m_shouldLoadNewLevelNow = false;
	Game* m_game;

	
};

