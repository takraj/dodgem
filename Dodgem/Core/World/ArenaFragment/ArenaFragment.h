#pragma once

#include <vector>
#include <OGRE\OgreVector3.h>

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

		std::vector<ArenaTriangle*> GetTriangleReferences();
		std::vector<ArenaTriangle> GetAvailableTriangles();
		std::vector<ArenaTriangle> GetRepresentingTriangles();
	};

}