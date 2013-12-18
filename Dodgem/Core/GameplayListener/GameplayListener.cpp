#include "stdafx.h"
#include "GameplayListener.h"

using namespace Dodgem;

GameplayListener::GameplayListener(World* subjectWorld)
{
	this->world = subjectWorld;
}

bool GameplayListener::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	return this->world->StepSimulation(evt.timeSinceLastFrame);
}