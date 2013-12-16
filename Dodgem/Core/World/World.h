#pragma once

#include <OGRE\OgreRenderWindow.h>
#include <OGRE\OgreSceneManager.h>
#include <OGRE\OgreLight.h>
#include <OGRE\OgreMath.h>
#include <OGRE\OgreColourValue.h>
#include <OGRE\OgreEntity.h>

#include "Camera\Camera.h"
#include "Arena\Arena.h"
#include "ArenaFragment\ArenaFragment.h"
#include "Meteor\Meteor.h"
#include "..\InputHandler\InputHandler.h"

namespace Dodgem
{

	class World
	{
	private:
		Dodgem::Arena* arena;
		Dodgem::Camera* camera;
		Dodgem::Meteor* meteor;
		Dodgem::InputHandler* ih;

		Ogre::Light* light;

		Ogre::RenderWindow* window;
		Ogre::SceneManager* sm;
	public:
		World(Ogre::RenderWindow* renderWindow, Ogre::SceneManager* sceneManager, Dodgem::InputHandler* inputHandler);
		~World(void);

		bool StepSimulation(Ogre::Real dt);
	};

}