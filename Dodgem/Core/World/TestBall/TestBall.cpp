#include "stdafx.h"
#include "TestBall.h"

using namespace Dodgem;

TestBall::TestBall(Ogre::SceneManager* sceneManager, Dodgem::PhysicsHandler* physicsHandler)
{
	this->sm = sceneManager;
	this->node = sm->getRootSceneNode()->createChildSceneNode();
	this->physics = physicsHandler;

	auto sphere = sm->createEntity("testball", Ogre::SceneManager::PT_SPHERE);
	sphere->setMaterialName("BaseWhiteNoLighting");

	this->node->attachObject(sphere);
	this->node->setScale(1, 1, 1);
	this->node->setVisible(false);

	this->ballShape = new btSphereShape(btScalar(sphere->getBoundingRadius()));

	this->mass = btScalar(1);
	this->inertia = btVector3(0, 0, 0);
	this->ballShape->calculateLocalInertia(mass, inertia);

	this->created = false;
}


TestBall::~TestBall(void)
{
	// ??
}

void TestBall::Kill()
{
	if (!this->created) return;

	this->physics->RemoveRigidBody(this->ballRigidBody);
	delete this->ballRigidBody;
	delete this->ballMotionState;

	this->created = false;
	this->node->setVisible(false);
	this->node->setPosition(0, 0, 0);
}


void TestBall::Create(Ogre::Vector3 position, Ogre::Vector3 direction)
{
	if (this->created)
	{
		physics->RemoveRigidBody(this->ballRigidBody);
		delete this->ballRigidBody;
		delete this->ballMotionState;
	}

	this->ballMotionState = new btDefaultMotionState(btTransform(this->quaternion, physics->AsBulletVector(position)));
	this->ballRigidBody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(this->mass, this->ballMotionState, this->ballShape, this->inertia));
	this->ballRigidBody->setDamping(0.2, 0);
	this->ballRigidBody->setRestitution(1.6);
	this->ballRigidBody->setFriction(1000);
	this->ballRigidBody->applyGravity(),
	this->ballRigidBody->applyCentralImpulse(physics->AsBulletVector(direction * Ogre::Real(1000)));

	this->physics->AddRigidBody(this->ballRigidBody);
	this->node->setVisible(false);
	this->created = true;
}


void TestBall::Update()
{
	if (!this->created) return;

	btTransform worldTrans;
	ballMotionState->getWorldTransform(worldTrans);
	btQuaternion rot = worldTrans.getRotation();
	btVector3 pos = worldTrans.getOrigin();

	this->node->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
	this->node->setPosition(pos.x(), pos.y(), pos.z());
}

Ogre::Vector3 TestBall::GetPosition()
{
	return this->node->getPosition();
}

void TestBall::ApplyForce(Ogre::Vector3 f)
{
	if ((!this->created) || (this->node->getPosition().y <= 0)) return;

	this->ballRigidBody->activate(true);
	this->ballRigidBody->applyTorque(this->physics->AsBulletVector(f));
}