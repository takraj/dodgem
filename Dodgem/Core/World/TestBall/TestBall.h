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

		void Create(Ogre::Vector3 position, Ogre::Vector3 direction);
		void Update();
	};

}