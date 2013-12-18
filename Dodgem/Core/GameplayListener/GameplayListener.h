#pragma once

#include "..\..\stdafx.h"
#include "..\World\World.h"

namespace Dodgem
{

	class GameplayListener : public Ogre::FrameListener
	{
	private:
		Dodgem::World* world;
	public:
		GameplayListener(Dodgem::World* subjectWorld);

		bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	};

}