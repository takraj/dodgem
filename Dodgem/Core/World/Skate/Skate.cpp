#include "stdafx.h"
#include "Skate.h"

using namespace Dodgem;

Skate::Skate(Ogre::SceneManager* sceneManager, Dodgem::PhysicsHandler* physicsHandler)
{
	static int id = 0;

	this->sm = sceneManager;
	this->node = sm->getRootSceneNode()->createChildSceneNode();
	this->physics = physicsHandler;

	auto skate = sm->createEntity("skate"+Ogre::StringConverter::toString(id++), "skate.mesh");

	auto objectNode = this->node->createChildSceneNode();
	objectNode->attachObject(skate);
	objectNode->roll(Ogre::Radian(Ogre::Degree(-90).valueRadians()));

	this->node->setScale(0.7, 0.7, 0.7);
	this->node->setVisible(false);

	objectNode->_update(true, true);

	const auto _AABB = objectNode->_getWorldAABB();
	const auto _HalfSize = _AABB.getHalfSize();
	const auto _AlignedHalfSize = Ogre::Vector3(_HalfSize.x, _HalfSize.y, _HalfSize.z);

	objectNode->setPosition(_HalfSize.x, _HalfSize.y * 1.9, _HalfSize.z * -1.5);

	btTransform localTrans;
	localTrans.setIdentity();
	localTrans.setOrigin(btVector3(_HalfSize.x, 0, 0));

	auto compoundShape = new btCompoundShape();
	compoundShape->addChildShape(localTrans, new btBoxShape(physics->AsBulletVector(_AlignedHalfSize)));

	this->skateShape = compoundShape;

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
	this->skateRigidBody->setRestitution(1);
	this->skateRigidBody->setAnisotropicFriction(btVector3(5, 10, 0.1));
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
	if ((!this->created) || (this->node->getPosition().y <= -5) || (this->node->getPosition().y >= 5)) return;

	auto forceVector = this->node->_getDerivedOrientation() * f;
	auto upVector = this->node->_getDerivedOrientation() * Ogre::Vector3::UNIT_X;
	auto forceScale = std::max<double>(upVector.y - 0.6, 0) / 0.4;

	this->skateRigidBody->activate(true);
	this->skateRigidBody->applyCentralForce(this->physics->AsBulletVector(forceVector * forceScale));
}

void Skate::ApplyImpulse(Ogre::Vector3 f)
{
	if (!this->created) return;

	this->skateRigidBody->activate(true);
	this->skateRigidBody->applyCentralImpulse(this->physics->AsBulletVector(f));
}

void Skate::Steer(Ogre::Vector3 f)
{
	if ((!this->created) || (this->node->getPosition().y <= -5) || (this->node->getPosition().y >= 5)) return;

	auto forceVector = this->node->_getDerivedOrientation() * f;
	auto upVector = this->node->_getDerivedOrientation() * Ogre::Vector3::UNIT_X;
	auto forceScale = std::max<double>(upVector.y - 0.6, 0) / 0.4;

	this->skateRigidBody->activate(true);

	auto speed = skateRigidBody->getLinearVelocity().length();

	this->skateRigidBody->applyTorque(this->physics->AsBulletVector(forceVector * forceScale * speed));
	//this->skateRigidBody->applyForce(this->physics->AsBulletVector(forceVector * forceScale * speed), btVector3(0, 0, -300));
}