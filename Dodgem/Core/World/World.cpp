#include "stdafx.h"
#include "World.h"

using namespace Dodgem;

World::World(Ogre::RenderWindow* renderWindow, Ogre::SceneManager* sceneManager, Dodgem::InputHandler* inputHandler)
{
	this->sm = sceneManager;
	this->window = renderWindow;
	this->ih = inputHandler;
	this->sm->setSkyBox(true, "Sky", 1000, true);

	this->physics = new Dodgem::PhysicsHandler(this);

	sceneManager->setAmbientLight(Ogre::ColourValue(0.2f, 0.2f, 0.2f, 1.0f));
	sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
	sceneManager->setShadowColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

	light = sceneManager->createLight("dirlight");
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setAttenuation(100, 1, 0, 0);
	light->setPowerScale(10000);
	light->setCastShadows(true);
	light->setDirection(-1, -1, -1);

	this->camera = new Camera(this->sm, this->window);
	this->arena = new Arena(this->sm, this->physics, 30, 15);
	this->meteor = new Meteor(this->sm, this->arena);

	//this->testBall1 = new TestBall(this->sm, this->physics);
	//this->testBall2 = new TestBall(this->sm, this->physics);

	this->skate1 = new Skate(this->sm, this->physics);
	this->skate2 = new Skate(this->sm, this->physics);

	auto arenaBounds = this->arena->GetBounds();
	auto ballPosMargin = arenaBounds * 0.1f;
	auto ballPosMaximum = arenaBounds - (ballPosMargin * 2);

	std::random_device rd;
	std::mt19937 e2(rd());
	std::uniform_real_distribution<> dist(0, 1);

	auto pos1 = Ogre::Vector3(0, 3, 0);
	auto pos2 = Ogre::Vector3(0, 3, 0);

	while (pos1.squaredDistance(pos2) < (5 * 5))
	{
		pos1.x = ballPosMaximum.x * dist(e2) + ballPosMargin.x;
		pos1.z = ballPosMaximum.z * dist(e2) + ballPosMargin.z;

		pos2.x = ballPosMaximum.x * dist(e2) + ballPosMargin.x;
		pos2.z = ballPosMaximum.z * dist(e2) + ballPosMargin.z;
	}

	//this->testBall1->Create(pos1);
	//this->testBall2->Create(pos2);

	this->skate1->Create(pos1);
	this->skate2->Create(pos2);

	lastDelta = 0;
	timeLeftToEndgame = 60;
	timeLeftToNextMeteor = 15;
	timeLeftToNextSecond = 1;
}


World::~World(void)
{
	delete this->arena;
	delete this->camera;
	delete this->meteor;
	delete this->skate1;
	delete this->skate2;
	delete this->physics;
}

void World::PhysicsTick()
{
	if (this->physics->CheckCollision(this->skate1->GetRigidBody(), this->skate2->GetRigidBody()))
	{
		printf("!! COLLISION !!\n");
		this->timeLeftToEndgame = 60;
	}
}

OgreDebugDrawer* World::GetDebugger()
{
	return this->physics->Debug(this->sm);
}

bool World::StepSimulation(Ogre::Real dt)
{
	this->lastDelta = dt;
	this->timeLeftToNextSecond -= dt;
	this->timeLeftToNextMeteor -= dt;

	while (this->timeLeftToNextSecond < 0)
	{
		this->timeLeftToNextSecond += 1.0;
		this->timeLeftToEndgame -= 1;
		printf("Endgame in %d s.\n", this->timeLeftToEndgame);
	}

	if (this->timeLeftToNextMeteor < 0)
	{
		std::random_device rd;
		std::mt19937 e2(rd());
		std::uniform_real_distribution<> dist(0, 1);
		
		this->timeLeftToNextMeteor = 3.0 + (10.0 * dist(e2));
		this->meteor->Create();
	}

	this->ih->CaptureState(dt);

	this->meteor->StepAnimation(dt);

	auto meteorPos = this->meteor->GetPosition();
	if (!this->meteor->IsEffectDispatched() && (meteorPos.y < 0))
	{
		auto skate1PointVector = this->skate1->GetPosition() - meteorPos;
		auto skate2PointVector = this->skate2->GetPosition() - meteorPos;

		this->skate1->ApplyImpulse((skate1PointVector.normalisedCopy() * 50000) / skate1PointVector.squaredLength());
		this->skate2->ApplyImpulse((skate2PointVector.normalisedCopy() * 50000) / skate2PointVector.squaredLength());
		this->meteor->SetEffectDispatched(true);
	}

	this->physics->StepSimulation(dt);

	this->ih->ControlSkates(this->skate1, this->skate2);

	this->skate1->Update();
	this->skate2->Update();

	if (this->skate1->GetPosition().y < -50)
	{
		this->skate1->Kill();
	}

	if (this->skate2->GetPosition().y < -50)
	{
		this->skate2->Kill();
	}

	if (!ih->ControlQuit())
	{
		return false;
	}

	if (this->skate1->IsAlive() && this->skate2->IsAlive())
	{
		auto midpoint = (this->skate1->GetPosition() + this->skate2->GetPosition()) * 0.5f;

		this->camera->SetLookAt(midpoint);

		auto distance = this->skate1->GetPosition().distance(this->skate2->GetPosition());
		auto heaven = midpoint + Ogre::Vector3(0, 0, 8);
		heaven.y = std::max<double>(distance * 1.2, 20);

		this->camera->AnimateToPosition(heaven, dt);
	}
	else if (this->skate1->IsAlive())
	{
		this->camera->SetLookAt(this->skate1->GetPosition());
		
		auto heaven = this->skate1->GetPosition();
		heaven.y = 25;

		this->camera->AnimateToPosition(heaven, dt);
	}
	else if (this->skate2->IsAlive())
	{
		this->camera->SetLookAt(this->skate2->GetPosition());
		
		auto heaven = this->skate2->GetPosition();
		heaven.y = 25;

		this->camera->AnimateToPosition(heaven, dt);
	}

	return true;
}