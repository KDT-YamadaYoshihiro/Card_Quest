#include "RenderManager.h"

void RenderManager::Begin(float arg_dt)
{
	CameraManager::Instance().ViewUpdate(arg_dt);
}

void RenderManager::DrawWorld(std::shared_ptr<sf::Drawable> arg_drawble)
{
}

void RenderManager::DrawUi(std::shared_ptr<sf::Drawable> arg_drawble)
{
}

void RenderManager::Render()
{

}
