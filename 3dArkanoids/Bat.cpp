#include "Bat.h"
#include "GameInput.h"
#include "IRenderer.h"
#include "PlayfieldDefs.h"
#include <iostream>
#include <format>

Bat::Bat(float batwWidth)
	:m_batWidth(batwWidth)
{

}

void Bat::RecieveInput(const GameInput& input)
{
	//std::cout << "position change: " << input.XChange << std::endl;

	m_xPos += input.XChange * m_sensitivity;
	if (m_xPos > m_xMaxPos) {
		m_xPos = m_xMaxPos;
	}
	else if (m_xPos < m_xMinPos) {
		m_xPos = m_xMinPos;
	}
}

void Bat::SetMinAndMaxXPos(double xMin, double xMax)
{
	m_xMinPos = xMin + m_batWidth * 0.5f;
	m_xMaxPos = xMax - m_batWidth * 0.5f;
	m_xPos = m_xMinPos;
}

void Bat::Draw(const IRenderer* renderer, const Camera& cam) const
{
	renderer->DrawCuboid(
		{ m_xPos,-(m_distanceFromFirstRow + BLOCK_WIDTH_UNITS * 0.5f),0 },
		{ m_batWidth,3,3 },
		cam,
		{ 0,0,0 });
}
