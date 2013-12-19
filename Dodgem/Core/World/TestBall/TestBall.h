#pragma once

#include "..\..\PhysicsHandler\PhysicsHandler.h"

namespace Dodgem
{

	class TestBall
	{
	private:
		Ogre::SceneManager* sm;
		Ogre::SceneNode* node;

		Dodgem::PhysicsHandler* physics;

		btMotionState* ballMotionState;
		btRigidBody* ballRigidBody;
		btCollisionShape* ballShape;
		btScalar mass;
		btVector3 inertia;
		btQuaternion quaternion;

		bool created;
	public:
		TestBall(Ogre::SceneManager* sceneManager, Dodgem::PhysicsHandler* physicsHandler);
		~TestBall(void);

		void Create(Ogre::Vector3 position);
		void Update();
		void Kill();
		void ApplyForce(Ogre::Vector3 f);
		void ApplyImpulse(Ogre::Vector3 f);
		Ogre::Vector3 GetPosition();

		bool IsAlive();

		btRigidBody* GetRigidBody();
	};

}