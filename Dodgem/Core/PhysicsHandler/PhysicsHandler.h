#pragma once

#include "..\..\stdafx.h"
#include "..\..\BulletUtility\DebugDrawer.h"

namespace Dodgem
{
	class PhysicsTickCallback
	{
	public:
		virtual void PhysicsTick() = 0;
	};

	class PhysicsHandler
	{
	private:
		btCollisionConfiguration* collisionConfig;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* pairCache;
		btConstraintSolver* solver;
		btDiscreteDynamicsWorld* dynamicsWorld;
		OgreDebugDrawer* debugger;
		PhysicsTickCallback* tcb;

		static void TickCallback(btDynamicsWorld *world, btScalar timeStep);
	public:
		PhysicsHandler(PhysicsTickCallback* tickCallback);
		~PhysicsHandler(void);

		void AddRigidBody(btRigidBody* body);
		void RemoveRigidBody(btRigidBody* body);
		void StepSimulation(Ogre::Real dt);

		bool CheckCollision(btRigidBody* body1, btRigidBody* body2);

		OgreDebugDrawer* Debug(Ogre::SceneManager* sm);

		btVector3 AsBulletVector(const Ogre::Vector3& vec);
		Ogre::Vector3 AsOgreVector(const btVector3& vec);
	};

}