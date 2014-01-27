#pragma once

#include "Camera\Camera.h"
#include "Arena\Arena.h"
#include "ArenaFragment\ArenaFragment.h"
#include "Meteor\Meteor.h"
#include "TestBall\TestBall.h"
#include "Skate\Skate.h"
#include "..\InputHandler\InputHandler.h"
#include "..\PhysicsHandler\PhysicsHandler.h"

namespace Dodgem
{

	class World : public PhysicsTickCallback
	{
	private:
		Dodgem::Arena* arena;
		Dodgem::Camera* camera;
		Dodgem::Meteor* meteor;
		//Dodgem::TestBall* testBall1, *testBall2;
		Dodgem::Skate* skate1, *skate2;
		Dodgem::InputHandler* ih;
		Dodgem::PhysicsHandler* physics;

		void PhysicsTick();

		Ogre::Light* light;

		Ogre::RenderWindow* window;
		Ogre::SceneManager* sm;

		double lastDelta;
		double timeLeftToNextMeteor;
		double timeLeftToNextSecond;
		int timeLeftToEndgame;
	public:
		World(Ogre::RenderWindow* renderWindow, Ogre::SceneManager* sceneManager, Dodgem::InputHandler* inputHandler);
		~World(void);

		bool StepSimulation(Ogre::Real dt);
		OgreDebugDrawer* GetDebugger();
	};

}