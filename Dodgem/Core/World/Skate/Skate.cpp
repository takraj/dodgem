#include "stdafx.h"
#include "Skate.h"

using namespace Dodgem;

Skate::Skate(Ogre::SceneManager* sceneManager, Dodgem::PhysicsHandler* physicsHandler)
{
	static int id = 0;

	this->sm = sceneManager;
	this->node = sm->getRootSceneNode()->createChildSceneNode();
	this->physics = physicsHandler;

	auto skate = sm->createEntity("testball"+Ogre::StringConverter::toString(id++), "skate.mesh");

	this->node->attachObject(skate);
	this->node->setScale(1, 1, 1);
	this->node->setVisible(false);

	const auto _AABB = skate->getBoundingBox();

	this->skateShape = new btBoxShape(physics->AsBulletVector(_AABB.getHalfSize()));

	this->mass = btScalar(100);
	this->inertia = btVector3(0, 0, 0);
	this->skateShape->calculateLocalInertia(mass, inertia);

	this->created = false;
}


Skate::~Skate(void)
{
	// ??
}

bool Skate::IsAlive()
{
	return this->created;
}

btRigidBody* Skate::GetRigidBody()
{
	return this->skateRigidBody;
}

void Skate::Kill()
{
	if (!this->created) return;

	this->physics->RemoveRigidBody(this->skateRigidBody);
	delete this->skateRigidBody;
	delete this->skateMotionState;

	this->created = false;
	this->node->setVisible(false);
	this->node->setPosition(0, 0, 0);
}


void Skate::Create(Ogre::Vector3 position)
{
	if (this->created)
	{
		physics->RemoveRigidBody(this->skateRigidBody);
		delete this->skateRigidBody;
		delete this->skateMotionState;
	}

	this->skateMotionState = new btDefaultMotionState(btTransform(this->quaternion, physics->AsBulletVector(position)));
	this->skateRigidBody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(this->mass, this->skateMotionState, this->skateShape, this->inertia));
	this->skateRigidBody->setDamping(0.2, 0);
	this->skateRigidBody->setRestitution(1.6);
	this->skateRigidBody->setFriction(10);
	this->skateRigidBody->applyGravity();

	this->physics->AddRigidBody(this->skateRigidBody);
	this->node->setVisible(true);
	this->created = true;
}


void Skate::Update()
{
	if (!this->created) return;

	btTransform worldTrans;
	skateMotionState->getWorldTransform(worldTrans);
	btQuaternion rot = worldTrans.getRotation();
	btVector3 pos = worldTrans.getOrigin();

	this->node->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
	this->node->setPosition(pos.x(), pos.y(), pos.z());
}

Ogre::Vector3 Skate::GetPosition()
{
	return this->node->getPosition();
}

void Skate::ApplyForce(Ogre::Vector3 f)
{
	if ((!this->created) || (this->node->getPosition().y <= 0)) return;

	this->skateRigidBody->activate(true);
	this->skateRigidBody->applyTorque(this->physics->AsBulletVector(f));
}

void Skate::ApplyImpulse(Ogre::Vector3 f)
{
	if (!this->created) return;

	this->skateRigidBody->activate(true);
	this->skateRigidBody->applyCentralImpulse(this->physics->AsBulletVector(f));
}