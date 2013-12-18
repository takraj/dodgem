#include "stdafx.h"
#include "PhysicsHandler.h"

using namespace Dodgem;

PhysicsHandler::PhysicsHandler(void)
{
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	pairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, pairCache, solver, collisionConfig);
	dynamicsWorld->setGravity(btVector3(0 , -981 ,0));

	this->debugger = NULL;
}


PhysicsHandler::~PhysicsHandler(void)
{
	delete this->dynamicsWorld;
	delete this->solver;
	delete this->pairCache;
	delete this->dispatcher;
	delete this->collisionConfig;
}

void PhysicsHandler::StepSimulation(Ogre::Real dt)
{
	this->dynamicsWorld->stepSimulation(btScalar(dt), 100);

	if (this->debugger != NULL)
	{
		this->dynamicsWorld->debugDrawWorld();
	}
}

void PhysicsHandler::AddRigidBody(btRigidBody* body)
{
	this->dynamicsWorld->addRigidBody(body);
}

void PhysicsHandler::RemoveRigidBody(btRigidBody* body)
{
	this->dynamicsWorld->removeRigidBody(body);
}

btVector3 PhysicsHandler::AsBulletVector(const Ogre::Vector3& vec)
{
	return btVector3(vec.x, vec.y, vec.z);
}

Ogre::Vector3 PhysicsHandler::AsOgreVector(const btVector3& vec)
{
	return Ogre::Vector3(vec.x(), vec.y(), vec.z());
}

OgreDebugDrawer* PhysicsHandler::Debug(Ogre::SceneManager* sm)
{
	this->debugger = new OgreDebugDrawer(sm);
	this->debugger->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	dynamicsWorld->setDebugDrawer(debugger);
	return this->debugger;
}
