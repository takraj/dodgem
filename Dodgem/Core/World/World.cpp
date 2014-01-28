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

	this->skate1 = new Skate(this->sm, this->physics);
	this->skate2 = new Skate(this->sm, this->physics);

	this->Init();
	this->player1Score = 0;
	this->player2Score = 0;
}


World::~World(void)
{
	delete this->arena;
	delete this->camera;
	delete this->meteor;
	delete this->skate1;
	delete this->skate2;
	delete this->physics;

	for (auto box : this->boxes)
	{
		delete box;
	}
}

void World::Init()
{
	for (auto box : this->boxes)
	{
		box->Kill();
		delete box;
	}

	this->boxes.clear();

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

	this->skate1->Create(pos1);
	this->skate2->Create(pos2);

	const int box_count_x = 5;
	const int box_count_z = 5;

	int current_box_x = 0;
	int current_box_z = 0;

	for (double x = (arenaBounds.x * 0.05); x < arenaBounds.x; x += (arenaBounds.x / box_count_x) * 1.1)
	{
		current_box_x++;

		for (double z = (arenaBounds.z * 0.05); z < arenaBounds.z; z += (arenaBounds.z / box_count_z) * 1.1)
		{
			current_box_z++;

			if (!((current_box_x == 1) || (current_box_z == 1) || (current_box_x == box_count_x) || (current_box_z == box_count_z)))
			{
				continue;
			}

			auto box = new Box(this->sm, this->physics);
			box->Create(Ogre::Vector3(x, 3, z));
			this->boxes.push_back(box);
		}

		current_box_z = 0;
	}

	this->lastDelta = 0;
	this->timeLeftToEndgame = 60;
	this->timeLeftToNextMeteor = 15;
	this->timeLeftToNextSecond = 1;
	this->isPaused = false;
	this->isEnded = false;
	this->winner = 0;
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

void World::AnimateCamera(Ogre::Real dt)
{
	if (this->skate1->IsAlive() && this->skate2->IsAlive())
	{
		auto midpoint = (this->skate1->GetPosition() + this->skate2->GetPosition()) * 0.5f;

		this->camera->SetLookAt(midpoint);

		auto distance = this->skate1->GetPosition().distance(this->skate2->GetPosition());
		auto heaven = midpoint + Ogre::Vector3(0, 0, 8);
		heaven.y = std::max<double>(distance * 1.4, 20);

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
}

bool World::StepSimulation(Ogre::Real dt)
{
	this->AnimateCamera(dt);
	this->ih->CaptureState(dt);

	if (!ih->ControlQuit())
	{
		return false;
	}

	if (this->isEnded)
	{
		if (!ih->ControlRestart())
		{
			this->isEnded = false;
			this->winner = 0;
			// TODO: hide winner
			this->arena->ResetArena();
			this->Init();
			printf("** RESTARTED **\n");
		}

		return true;
	}

	if (!ih->ControlPause())
	{
		if (this->isPaused)
		{
			this->isPaused = false;
			printf("** UNPAUSED **\n");
		}
		else
		{
			this->isPaused = true;
			printf("** PAUSED **\n");
		}
	}

	if (this->isPaused || this->isEnded)
	{
		return true;
	}

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

	this->meteor->StepAnimation(dt);

	auto meteorPos = this->meteor->GetPosition();
	if (!this->meteor->IsEffectDispatched() && (meteorPos.y < 0))
	{
		auto skate1PointVector = this->skate1->GetPosition() - meteorPos;
		auto skate2PointVector = this->skate2->GetPosition() - meteorPos;

		this->skate1->ApplyImpulse((skate1PointVector.normalisedCopy() * 50000) / skate1PointVector.squaredLength());
		this->skate2->ApplyImpulse((skate2PointVector.normalisedCopy() * 50000) / skate2PointVector.squaredLength());

		for (auto box : this->boxes)
		{
			auto boxPointVector = box->GetPosition() - meteorPos;

			auto f = (boxPointVector.normalisedCopy() * 150000) / boxPointVector.squaredLength();

			if (f.squaredLength() > 100000)
			{
				box->ApplyImpulse(f);
			}
		}

		this->meteor->SetEffectDispatched(true);
	}

	this->physics->StepSimulation(dt);

	this->ih->ControlSkates(this->skate1, this->skate2);

	this->skate1->Update();
	this->skate2->Update();

	for (auto box : this->boxes)
	{
		box->Update();

		if (box->GetPosition().y < -50)
		{
			box->Kill();
		}
	}

	if (this->skate1->GetPosition().y < -50)
	{
		this->skate1->Kill();
		this->player2Score++;
		this->winner = 2;
		this->isEnded = true;
	}

	if (this->skate2->GetPosition().y < -50)
	{
		this->skate2->Kill();
		this->player1Score++;
		this->winner++; // 2->3: draw, 0->1: p1 wins
		this->isEnded = true;
	}

	if (this->winner == 1)
	{
		printf("** Player 1 wins, scores %d:%d **\n", this->player1Score, this->player2Score);
	}

	if (this->winner == 2)
	{
		printf("** Player 2 wins, scores %d:%d **\n", this->player1Score, this->player2Score);
	}

	if (this->winner == 3)
	{
		printf("** DRAW, scores %d:%d **\n", this->player1Score, this->player2Score);
	}

	return true;
}