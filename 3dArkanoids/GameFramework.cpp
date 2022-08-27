#include <functional>
#include "GameFramework.h"
#include "Camera.h"
#include "GameInput.h"
#include <iostream>
static size_t m_inputStackSize = 0;
static size_t m_drawableStackSize = 0;
static size_t m_updateableStackSize = 0;

RecieveInputLayerBase* GameFramework::m_inputStack[FRAMEWORK_STACKS_SIZE];
DrawableLayerBase* GameFramework::m_drawableStack[FRAMEWORK_STACKS_SIZE];
UpdateableLayerBase* GameFramework::m_updateableStack[FRAMEWORK_STACKS_SIZE];

size_t GameFramework::m_inputStackSize;
size_t GameFramework::m_drawableStackSize;
size_t GameFramework::m_updateableStackSize;

void GameFramework::Update(double deltaT)
{
	auto topOfStackIndex = m_updateableStackSize - 1;
	do {
		m_updateableStack[topOfStackIndex]->Update(deltaT);
	} while (!m_updateableStack[topOfStackIndex--]->MasksPreviousUpdateableLayer());
}

void GameFramework::Draw(const Camera& camera)
{
	auto topOfStackIndex = m_drawableStackSize - 1;
	do {
		m_drawableStack[topOfStackIndex]->Draw(camera);
	} while (!m_drawableStack[topOfStackIndex--]->MasksPreviousDrawableLayer());
}

void GameFramework::RecieveInput(const GameInput& input)
{
	auto topOfStackIndex = m_inputStackSize - 1;
	do {
		m_inputStack[topOfStackIndex]->ReceiveInput(input);
	} while (!m_inputStack[topOfStackIndex--]->MasksPreviousInputLayer());
}


bool GameFramework::PushLayers(std::string name, GameLayerType whichLayers)
{
	// TODO: refactor to reduce duplication
	if (((unsigned int)whichLayers & (unsigned int)GameLayerType::Input) > 0) {
		bool hasFoundName = false;
		const auto& list = AutoList<RecieveInputLayerBase>::GetList();
		for (const auto layer : list) {
			if (layer->GetInputLayerName() == name) {
				if (!hasFoundName) {
					hasFoundName = true;
					PushInputLayer(layer);
				}
				else {
					std::cerr << "[GameFramework] Requested input layer " << name << " was found more than once in the auto list - this isn't allowed" << std::endl;
					return false;
				}
			}
		}
		if (!hasFoundName) {
			std::cerr << "[GameFramework] Requested input layer " << name << " was not found" << std::endl;
			return false;
		}
	}
	if (((unsigned int)whichLayers & (unsigned int)GameLayerType::Draw) > 0) {
		bool hasFoundName = false;
		const auto& list = AutoList<DrawableLayerBase>::GetList();
		for (const auto layer : list) {
			if (layer->GetDrawableLayerName() == name) {
				if (!hasFoundName) {
					hasFoundName = true;
					PushDrawableLayer(layer);
				}
				else {
					std::cerr << "[GameFramework] Requested draw layer " << name << " was found more than once in the auto list - this isn't allowed" << std::endl;
					return false;
				}
			}
		}
		if (!hasFoundName) {
			std::cerr << "[GameFramework] Requested draw layer " << name << " was not found" << std::endl;
			return false;
		}
	}
	if (((unsigned int)whichLayers & (unsigned int)GameLayerType::Update) > 0) {
		bool hasFoundName = false;
		const auto& list = AutoList<UpdateableLayerBase>::GetList();
		for (const auto layer : list) {
			if (layer->GetUpdateableLayerName() == name) {
				if (!hasFoundName) {
					hasFoundName = true;
					PushUpdatableLayer(layer);
				}
				else {
					std::cerr << "[GameFramework] Requested update layer " << name << " was found more than once in the auto list - this isn't allowed" << std::endl;
					return false;
				}
			}
		}
		if (!hasFoundName) {
			std::cerr << "[GameFramework] Requested update layer " << name << " was not found" << std::endl;
			return false;
		}
	}
	return true;
}

bool GameFramework::PopLayers(GameLayerType whichLayers)
{
	// TODO: refactor to reduce duplication
	if (((unsigned int)whichLayers & (unsigned int)GameLayerType::Input) > 0) {
		PopInputLayer();
	}
	if (((unsigned int)whichLayers & (unsigned int)GameLayerType::Draw) > 0) {
		PopDrawableLayer();
	}
	if (((unsigned int)whichLayers & (unsigned int)GameLayerType::Update) > 0) {
		PopUpdatableLayer();
	}
	return true;
}

RecieveInputLayerBase* GameFramework::PopInputLayer()
{
	auto top = m_inputStack[--m_inputStackSize];
	return top;
}

const DrawableLayerBase* GameFramework::PopDrawableLayer()
{
	auto top = m_drawableStack[--m_drawableStackSize];
	return top;
}

UpdateableLayerBase* GameFramework::PopUpdatableLayer()
{
	auto top = m_updateableStack[--m_updateableStackSize];
	return top;
}
void GameFramework::PushInputLayer(RecieveInputLayerBase* input)
{
	m_inputStack[m_inputStackSize++] = input;
}

void GameFramework::PushDrawableLayer(DrawableLayerBase* drawable)
{
	m_drawableStack[m_drawableStackSize++] = drawable;
}

void GameFramework::PushUpdatableLayer(UpdateableLayerBase* updatable)
{
	m_updateableStack[m_updateableStackSize++] = updatable;
}
