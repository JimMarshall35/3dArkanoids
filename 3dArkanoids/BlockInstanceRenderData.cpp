#include "BlockInstanceRenderData.h"
#include "IRenderer.h"

void BlockInstanceRenderData::SetNewWorldPos(const glm::vec3& val) {
	myRenderer->SetCubePos(renderDataArrayIndex, val);
	worldPos = val;
}

void BlockInstanceRenderData::SetShouldRender(bool newval) {
	myRenderer->SetCubeShouldRender(renderDataArrayIndex, newval);
	shouldRender = newval;
}