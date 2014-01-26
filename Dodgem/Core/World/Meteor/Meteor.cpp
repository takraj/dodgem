#include "stdafx.h"
#include "Meteor.h"

using namespace Dodgem;

Meteor::Meteor(Ogre::SceneManager* sceneManager, Dodgem::Arena* dodgemArena)
{
	this->sm = sceneManager;
	this->arena = dodgemArena;

	node = sm->getRootSceneNode()->createChildSceneNode();

	auto sphere = sm->createEntity("meteor", "striped_ball.mesh");

	node->attachObject(sphere);
	node->setScale(1, 1, 1);
	node->setVisible(false);
	this->active = false;

	light = sceneManager->createLight("meteor_light");
	light->setType(Ogre::Light::LT_POINT);
	light->setAttenuation(10000000, 0, 0.1, 0);
	light->setPowerScale(100000);
	light->setCastShadows(true);
	light->setPosition(node->getPosition());
	light->setVisible(false);

	fireNode = node->createChildSceneNode();

	psysFire = sm->createParticleSystem("psysFire", "FireSystem");
}


Meteor::~Meteor(void)
{
}


void Meteor::Create()
{
	if (this->active)
	{
		fireNode->detachObject(psysFire);
	}

	auto arenaSize = this->arena->GetBounds();

	std::random_device rd;
	std::mt19937 e2(rd());
	std::uniform_real_distribution<> dist(0, 1);
	
	this->birthPoint.x = (dist(e2) * (arenaSize.x * 7)) - (arenaSize.x * 3);
	this->birthPoint.y = 500.0;
	this->birthPoint.z = (dist(e2) * (arenaSize.z * 7)) - (arenaSize.z * 3);

	this->targetPoint.x = dist(e2) * arenaSize.x;
	this->targetPoint.y = 0.0;
	this->targetPoint.z = dist(e2) * arenaSize.z;

	this->node->setPosition(birthPoint);
	this->node->setVisible(true);

	this->active = true;
	this->impacted = false;
	this->timeElapsedSinceCreation = 0.0;

	this->effectDispatched = false;

	light->setVisible(true);
	light->setPosition(node->getPosition() + ((targetPoint - birthPoint).normalise() * 1.5));

	fireNode->attachObject(psysFire);
}

void Meteor::StepAnimation(Ogre::Real dt)
{
	if (!this->active) return;

	const Ogre::Real total_lifetime = 5.0;
	const Ogre::Real impact_time = 2.0;
	const auto direction = this->targetPoint - this->birthPoint;

	this->timeElapsedSinceCreation += dt;
	this->node->setPosition(this->birthPoint + (direction * (this->timeElapsedSinceCreation / impact_time)));

	light->setPosition(node->getPosition());

	if (!this->impacted && (this->node->getPosition().y < 1))
	{
		light->setVisible(false);
		this->impacted = true;
		this->arena->Impact(this->node->getPosition(), 1.0f);
	}

	if (this->timeElapsedSinceCreation > total_lifetime)
	{
		if (this->active)
		{
			fireNode->detachObject(psysFire);
		}

		light->setVisible(false);
		this->active = false;
		this->node->setVisible(false);
	}
}

bool Meteor::isActive()
{
	return active;
}

Ogre::Vector3 Meteor::GetPosition()
{
	return this->node->getPosition();
}

bool Meteor::IsEffectDispatched()
{
	return this->effectDispatched;
}

void Meteor::SetEffectDispatched(bool state)
{
	this->effectDispatched = state;
}