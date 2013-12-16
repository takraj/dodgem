#include "WindowListener.h"

using namespace Dodgem;

WindowListener::WindowListener(const Ogre::RenderWindow* window)
{
	_window = window;
}

bool WindowListener::frameStarted(const Ogre::FrameEvent& evt)
{
	return !_window->isClosed();
}