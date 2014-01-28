#pragma once

#include "..\..\PhysicsHandler\PhysicsHandler.h"

namespace Dodgem
{
	class Box
	{
	private:
		Ogre::SceneManager* sm;
		Ogre::SceneNode* node;

		Dodgem::PhysicsHandler* physics;

		btMotionState* boxMotionState;
		btRigidBody* boxRigidBody;
		btCollisionShape* boxShape;
		btScalar mass;
		btVector3 inertia;
		btQuaternion quaternion;

		bool created;
	public:
		Box(Ogre::SceneManager* sceneManager, Dodgem::PhysicsHandler* physicsHandler);
		~Box(void);

		void Create(Ogre::Vector3 position);
		void Update();
		void Kill();
		void ApplyImpulse(Ogre::Vector3 f);
		Ogre::Vector3 GetPosition();

		bool IsAlive();

		btRigidBody* GetRigidBody();
	};
}