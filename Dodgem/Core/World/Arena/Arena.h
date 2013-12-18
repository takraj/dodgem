#pragma once

#include "..\ArenaFragment\ArenaFragment.h"
#include "..\..\PhysicsHandler\PhysicsHandler.h"

namespace Dodgem
{

	class Arena
	{
	private:
		Ogre::ManualObject* arenaObject;
		Ogre::SceneNode* root;
		Ogre::SceneManager* sm;

		Dodgem::PhysicsHandler* physics;
		btMotionState* arenaMotionState;
		btRigidBody* arenaRigidBody;
		btTriangleMesh* arenaTriangleMesh;
		btTriangleMeshShape* arenaShape;

		std::vector<Dodgem::ArenaFragment> fragments;

		size_t width, height;

		void CreateArena();
		void UpdateArena();
	public:
		Arena(Ogre::SceneManager* sceneManager, Dodgem::PhysicsHandler* physicsHandler, size_t arena_w, size_t arena_h);
		~Arena(void);

		Ogre::Vector3 GetBounds();
		void Impact(Ogre::Vector3 impactPoint, Ogre::Real impactRadius);
	};

}