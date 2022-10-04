#include "VisualEffectsManager.h"

void VisualEffectsManager::OnEvent(BallComboEventArgs e)
{
	std::cout << "Ball combo! " << e.numBounces << "\n";
}
