#pragma once

#include "..\..\stdafx.h"
#include "..\..\BulletUtility\DebugDrawer.h"

namespace Dodgem
{

	class PhysicsHandler
	{
	private:
		btCollisionConfiguration* collisionConfig;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* pairCache;
		btConstraintSolver* solver;
		btDiscreteDynamicsWorld* dynamicsWorld;
		OgreDebugDrawer* debugger;
	public:
		PhysicsHandler(void);
		~PhysicsHandler(void);

		void AddRigidBody(btRigidBody* body);
		void RemoveRigidBody(btRigidBody* body);
		void StepSimulation(Ogre::Real dt);

		OgreDebugDrawer* Debug(Ogre::SceneManager* sm);

		btVector3 AsBulletVector(const Ogre::Vector3& vec);
		Ogre::Vector3 AsOgreVector(const btVector3& vec);
	};

}