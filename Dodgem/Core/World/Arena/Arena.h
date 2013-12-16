#pragma once

#include <OGRE\OgreSceneNode.h>
#include <OGRE\OgreSceneManager.h>
#include <OGRE\OgreManualObject.h>
#include <OGRE\OgreRenderOperation.h>
#include <OGRE\OgreMath.h>

#include "..\ArenaFragment\ArenaFragment.h"

namespace Dodgem
{

	class Arena
	{
	private:
		Ogre::ManualObject* arenaObject;
		Ogre::SceneNode* root;
		Ogre::SceneManager* sm;

		std::vector<Dodgem::ArenaFragment> fragments;

		size_t width, height;

		void CreateArena();
		void UpdateArena();
	public:
		Arena(Ogre::SceneManager* sceneManager, size_t arena_w, size_t arena_h);
		~Arena(void);

		Ogre::Vector3 GetBounds();
		void Impact(Ogre::Vector3 impactPoint, Ogre::Real impactRadius);
	};

}