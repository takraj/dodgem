#pragma once

#include "..\..\PhysicsHandler\PhysicsHandler.h"

namespace Dodgem
{

	class Skate
	{
	private:
		Ogre::SceneManager* sm;
		Ogre::SceneNode* node;

		Dodgem::PhysicsHandler* physics;

		btMotionState* skateMotionState;
		btRigidBody* skateRigidBody;
		btCollisionShape* skateShape;
		btScalar mass;
		btVector3 inertia;
		btQuaternion quaternion;

		bool created;
	public:
		Skate(Ogre::SceneManager* sceneManager, Dodgem::PhysicsHandler* physicsHandler);
		~Skate(void);

		void Create(Ogre::Vector3 position);
		void Update();
		void Kill();
		void ApplyForce(Ogre::Vector3 f);
		void ApplyImpulse(Ogre::Vector3 f);
		void Steer(Ogre::Vector3 f);
		Ogre::Vector3 GetPosition();

		bool IsAlive();

		btRigidBody* GetRigidBody();
	};

}