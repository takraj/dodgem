#include "stdafx.h"
#include "World.h"

using namespace Dodgem;

World::World(Ogre::RenderWindow* renderWindow, Ogre::SceneManager* sceneManager, Dodgem::InputHandler* inputHandler)
{
	this->sm = sceneManager;
	this->window = renderWindow;
	this->ih = inputHandler;

	this->physics = new Dodgem::PhysicsHandler();

	sceneManager->setAmbientLight(Ogre::ColourValue(0.2f, 0.2f, 0.2f, 1.0f));
	sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
	sceneManager->setShadowColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

	light = sceneManager->createLight("pointlight1");
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setAttenuation(1000, 1, 0, 0);
	light->setPowerScale(1);
	light->setCastShadows(true);
	light->setDirection(-1, -1, -1);

	this->camera = new Camera(this->sm, this->window);
	this->arena = new Arena(this->sm, this->physics, 30, 15);
	this->meteor = new Meteor(this->sm, this->arena);
	this->testBall = new TestBall(this->sm, this->physics);
}


World::~World(void)
{
	delete this->arena;
	delete this->camera;
	delete this->meteor;
	delete this->testBall;
	delete this->physics;
}

OgreDebugDrawer* World::GetDebugger()
{
	return this->physics->Debug(this->sm);
}

bool World::StepSimulation(Ogre::Real dt)
{
	ih->CaptureState(dt);

	ih->ControlMeteor(this->meteor);
	this->meteor->StepAnimation(dt);

	this->physics->StepSimulation(dt);

	ih->ControlTestBall(this->camera, this->testBall);

	this->testBall->Update();

	if (this->testBall->GetPosition().y < -5000)
	{
		this->testBall->Kill();
	}

	if (!ih->ControlCamera(this->camera))
	{
		return false;
	}

	return true;
}