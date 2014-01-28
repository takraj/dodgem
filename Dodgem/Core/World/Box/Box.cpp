#include "stdafx.h"
#include "Box.h"

using namespace Dodgem;

Box::Box(Ogre::SceneManager* sceneManager, Dodgem::PhysicsHandler* physicsHandler)
{
	static int id = 0;

	this->sm = sceneManager;
	this->node = sm->getRootSceneNode()->createChildSceneNode();
	this->physics = physicsHandler;

	auto box = sm->createEntity("box"+Ogre::StringConverter::toString(id++), "doboz.mesh");

	this->node->attachObject(box);
	this->node->setScale(2, 2, 2);
	this->node->setVisible(false);

	this->boxShape = new btBoxShape(physics->AsBulletVector(box->getBoundingBox().getHalfSize() * 2));

	this->mass = btScalar(300);
	this->inertia = btVector3(0, 0, 0);
	this->boxShape->calculateLocalInertia(mass, inertia);

	this->created = false;
}


Box::~Box(void)
{
	// ??
}

bool Box::IsAlive()
{
	return this->created;
}

btRigidBody* Box::GetRigidBody()
{
	return this->boxRigidBody;
}

void Box::Kill()
{
	if (!this->created) return;

	this->physics->RemoveRigidBody(this->boxRigidBody);
	delete this->boxRigidBody;
	delete this->boxMotionState;

	this->created = false;
	this->node->setVisible(false);
	this->node->setPosition(0, 0, 0);
}


void Box::Create(Ogre::Vector3 position)
{
	if (this->created)
	{
		physics->RemoveRigidBody(this->boxRigidBody);
		delete this->boxRigidBody;
		delete this->boxMotionState;
	}

	this->boxMotionState = new btDefaultMotionState(btTransform(this->quaternion, physics->AsBulletVector(position)));
	this->boxRigidBody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(this->mass, this->boxMotionState, this->boxShape, this->inertia));
	this->boxRigidBody->setDamping(0.2, 0);
	this->boxRigidBody->setRestitution(1.6);
	this->boxRigidBody->setFriction(10);
	this->boxRigidBody->applyGravity();

	this->physics->AddRigidBody(this->boxRigidBody);
	this->node->setVisible(true);
	this->created = true;
}


void Box::Update()
{
	if (!this->created) return;

	btTransform worldTrans;
	boxMotionState->getWorldTransform(worldTrans);
	btQuaternion rot = worldTrans.getRotation();
	btVector3 pos = worldTrans.getOrigin();

	this->node->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
	this->node->setPosition(pos.x(), pos.y(), pos.z());
}

Ogre::Vector3 Box::GetPosition()
{
	return this->node->getPosition();
}

void Box::ApplyImpulse(Ogre::Vector3 f)
{
	if (!this->created) return;

	this->boxRigidBody->activate(true);
	this->boxRigidBody->applyCentralImpulse(this->physics->AsBulletVector(f));
}
