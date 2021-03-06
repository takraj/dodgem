#include "stdafx.h"
#include "TestBall.h"

using namespace Dodgem;

TestBall::TestBall(Ogre::SceneManager* sceneManager, Dodgem::PhysicsHandler* physicsHandler)
{
	static int id = 0;

	this->sm = sceneManager;
	this->node = sm->getRootSceneNode()->createChildSceneNode();
	this->physics = physicsHandler;

	auto sphere = sm->createEntity("testball"+Ogre::StringConverter::toString(id++), "striped_ball.mesh");

	this->node->attachObject(sphere);
	this->node->setScale(1, 1, 1);
	this->node->setVisible(false);

	this->ballShape = new btSphereShape(btScalar(sphere->getBoundingRadius() * 0.5));

	this->mass = btScalar(100);
	this->inertia = btVector3(0, 0, 0);
	this->ballShape->calculateLocalInertia(mass, inertia);

	this->created = false;
}


TestBall::~TestBall(void)
{
	// ??
}

bool TestBall::IsAlive()
{
	return this->created;
}

btRigidBody* TestBall::GetRigidBody()
{
	return this->ballRigidBody;
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


void TestBall::Create(Ogre::Vector3 position)
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
	this->ballRigidBody->setFriction(10);
	this->ballRigidBody->applyGravity();

	this->physics->AddRigidBody(this->ballRigidBody);
	this->node->setVisible(true);
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

void TestBall::ApplyImpulse(Ogre::Vector3 f)
{
	if (!this->created) return;

	this->ballRigidBody->activate(true);
	this->ballRigidBody->applyCentralImpulse(this->physics->AsBulletVector(f));
}