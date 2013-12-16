#include "World.h"

using namespace Dodgem;

World::World(Ogre::RenderWindow* renderWindow, Ogre::SceneManager* sceneManager, Dodgem::InputHandler* inputHandler)
{
	this->sm = sceneManager;
	this->window = renderWindow;
	this->ih = inputHandler;

	sceneManager->setAmbientLight(Ogre::ColourValue(0.2f, 0.2f, 0.2f, 1.0f));
	sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
	sceneManager->setShadowColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

	light = sceneManager->createLight("pointlight1");
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setAttenuation(1000, 1, 0, 0);
	light->setPowerScale(1);
	light->setCastShadows(true);
	light->setDirection(-1, -1, -1);

	this->camera = new Camera(sm, window);
	this->arena = new Arena(sm, 30, 15);
	this->meteor = new Meteor(sm, this->arena);
}


World::~World(void)
{
	delete arena;
	delete camera;
}

bool World::StepSimulation(Ogre::Real dt)
{
	ih->CaptureState();

	ih->ControlMeteor(this->meteor, dt);
	this->meteor->StepAnimation(dt);

	if (!ih->ControlCamera(this->camera, dt))
	{
		return false;
	}

	return true;
}