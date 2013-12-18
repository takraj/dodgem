#pragma once

#include "..\..\stdafx.h"

namespace Dodgem
{
	class WindowListener : public Ogre::FrameListener
	{
	private:
		bool frameStarted(const Ogre::FrameEvent& evt);
		const Ogre::RenderWindow* _window;
	public:
		WindowListener(const Ogre::RenderWindow* window);
	};
}