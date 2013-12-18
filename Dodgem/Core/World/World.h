#pragma once

#include "Camera\Camera.h"
#include "Arena\Arena.h"
#include "ArenaFragment\ArenaFragment.h"
#include "Meteor\Meteor.h"
#include "TestBall\TestBall.h"
#include "..\InputHandler\InputHandler.h"
#include "..\PhysicsHandler\PhysicsHandler.h"

namespace Dodgem
{

	class World
	{
	private:
		Dodgem::Arena* arena;
		Dodgem::Camera* camera;
		Dodgem::Meteor* meteor;
		Dodgem::TestBall* testBall;
		Dodgem::InputHandler* ih;
		Dodgem::PhysicsHandler* physics;

		Ogre::Light* light;

		Ogre::RenderWindow* window;
		Ogre::SceneManager* sm;
	public:
		World(Ogre::RenderWindow* renderWindow, Ogre::SceneManager* sceneManager, Dodgem::InputHandler* inputHandler);
		~World(void);

		bool StepSimulation(Ogre::Real dt);
		OgreDebugDrawer* GetDebugger();
	};

}