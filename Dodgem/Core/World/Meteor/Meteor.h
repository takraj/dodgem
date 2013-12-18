#pragma once

#include "..\..\..\stdafx.h"
#include "..\Arena\Arena.h"

namespace Dodgem
{

	class Meteor
	{
	private:
		bool active;
		bool impacted;

		Ogre::Real timeElapsedSinceCreation;
		Ogre::Vector3 birthPoint;
		Ogre::Vector3 targetPoint;

		Ogre::SceneManager* sm;
		Ogre::SceneNode* node;
		Dodgem::Arena* arena;
	public:
		Meteor(Ogre::SceneManager* sceneManager, Dodgem::Arena* dodgemArena);
		~Meteor(void);

		void Create();
		void StepAnimation(Ogre::Real dt);
		bool isActive();
	};

}