#pragma once

#include "..\..\..\stdafx.h"
#include "..\..\PhysicsHandler\PhysicsHandler.h"

namespace Dodgem
{

	struct ArenaTriangle
	{
		Ogre::Vector3 v1;
		Ogre::Vector3 v2;
		Ogre::Vector3 v3;
		bool isDestroyed;
	};

	class ArenaFragment
	{
	private:
		size_t _col, _row;
		bool isChanged;

		ArenaTriangle tri_upper_left_1;
		ArenaTriangle tri_upper_left_2;

		ArenaTriangle tri_upper_right_1;
		ArenaTriangle tri_upper_right_2;

		ArenaTriangle tri_lower_left_1;
		ArenaTriangle tri_lower_left_2;

		ArenaTriangle tri_lower_right_1;
		ArenaTriangle tri_lower_right_2;

		Dodgem::PhysicsHandler* physics;
		btMotionState* fragmentMotionState;
		btRigidBody* fragmentRigidBody;
		btTriangleMesh* fragmentTriangleMesh;
		btTriangleMeshShape* fragmentShape;
	public:
		ArenaFragment(Dodgem::PhysicsHandler* physicsHandler, size_t col, size_t row);

		bool IsChanged();
		void SetChanged(bool value);
		void UpdatePhysics();

		size_t GetColumn();
		size_t GetRow();

		std::vector<ArenaTriangle*> GetTriangleReferences();
		std::vector<ArenaTriangle> GetAvailableTriangles();
		std::vector<ArenaTriangle> GetRepresentingTriangles();
	};

}