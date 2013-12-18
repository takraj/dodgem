#pragma once

#include "..\..\..\stdafx.h"
#include "..\..\PhysicsHandler\PhysicsHandler.h"

namespace Dodgem
{
	const double ARENA_FRAGMENT_W = 200.0;
	const double ARENA_FRAGMENT_H = 200.0;

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

		ArenaTriangle tri_full_1;
		ArenaTriangle tri_full_2;

		ArenaTriangle tri_upper_left_1;
		ArenaTriangle tri_upper_left_2;

		ArenaTriangle tri_upper_right_1;
		ArenaTriangle tri_upper_right_2;

		ArenaTriangle tri_lower_left_1;
		ArenaTriangle tri_lower_left_2;

		ArenaTriangle tri_lower_right_1;
		ArenaTriangle tri_lower_right_2;
	public:
		ArenaFragment(size_t col, size_t row);

		bool IsDamaged();

		size_t GetColumn();
		size_t GetRow();

		std::vector<ArenaTriangle*> GetTriangles();
		std::vector<ArenaTriangle*> GetAvailableTriangles();
		std::vector<ArenaTriangle*> GetRepresentingTriangles();
	};

}