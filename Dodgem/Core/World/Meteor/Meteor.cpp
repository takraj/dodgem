#include "stdafx.h"
#include "Meteor.h"

using namespace Dodgem;

Meteor::Meteor(Ogre::SceneManager* sceneManager, Dodgem::Arena* dodgemArena)
{
	this->sm = sceneManager;
	this->arena = dodgemArena;

	node = sm->getRootSceneNode()->createChildSceneNode();

	auto sphere = sm->createEntity("meteor", Ogre::SceneManager::PT_SPHERE);
	sphere->setMaterialName("BaseWhiteNoLighting");

	node->attachObject(sphere);
	node->setScale(2, 2, 2);
	node->setVisible(false);
	this->active = false;
}


Meteor::~Meteor(void)
{
}


void Meteor::Create()
{
	auto arenaSize = this->arena->GetBounds();

	std::random_device rd;
	std::mt19937 e2(rd());
	std::uniform_real_distribution<> dist(0, 1);
	
	this->birthPoint.x = (dist(e2) * (arenaSize.x * 7)) - (arenaSize.x * 3);
	this->birthPoint.y = 50000.0;
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
}

void Meteor::StepAnimation(Ogre::Real dt)
{
	if (!this->active) return;

	const Ogre::Real total_lifetime = 5.0;
	const Ogre::Real impact_time = 2.0;
	const auto direction = this->targetPoint - this->birthPoint;

	this->timeElapsedSinceCreation += dt;
	this->node->setPosition(this->birthPoint + (direction * (this->timeElapsedSinceCreation / impact_time)));

	if (!this->impacted && (this->node->getPosition().y < 100))
	{
		this->impacted = true;
		this->arena->Impact(this->node->getPosition(), 100.0f);
	}

	if (this->timeElapsedSinceCreation > total_lifetime)
	{
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