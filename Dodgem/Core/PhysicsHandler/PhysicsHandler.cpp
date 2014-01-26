#include "stdafx.h"
#include "PhysicsHandler.h"

using namespace Dodgem;

PhysicsHandler::PhysicsHandler(PhysicsTickCallback* tickCallback = NULL)
{
	this->tcb = tickCallback;

	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	pairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, pairCache, solver, collisionConfig);
	dynamicsWorld->setGravity(btVector3(0 , -9.81f, 0));
	dynamicsWorld->setInternalTickCallback(&PhysicsHandler::TickCallback, static_cast<void *>(this));

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
	this->dynamicsWorld->stepSimulation(btScalar(dt), 10);

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

void PhysicsHandler::TickCallback(btDynamicsWorld *world, btScalar timeStep)
{
	PhysicsHandler* physics = static_cast<PhysicsHandler*>(world->getWorldUserInfo());
	
	if (physics->tcb == NULL) return;

	physics->tcb->PhysicsTick();
}

bool PhysicsHandler::CheckCollision(btRigidBody* body1, btRigidBody* body2)
{
	int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i=0;i<numManifolds;i++)
	{
		btPersistentManifold* contactManifold =  dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* objA = contactManifold->getBody0();
		const btCollisionObject* objB = contactManifold->getBody1();
		const int numContacts = contactManifold->getNumContacts();
		
		if ((objA == body1) && (objB == body2) && (numContacts > 0))
		{
			return true;
		}
	}

	return false;
}