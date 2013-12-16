#include "ArenaFragment.h"

using namespace Dodgem;

#define ARENA_FRAGMENT_W 200.0
#define ARENA_FRAGMENT_H 200.0

ArenaFragment::ArenaFragment(size_t col, size_t row)
{
	this->_col = col;
	this->_row = row;

	// ===== SHIFT DEFINITIONS =====

	auto x_half_shift = Ogre::Vector3(ARENA_FRAGMENT_W / 2.0, 0, 0);
	auto z_half_shift = Ogre::Vector3(0, 0, ARENA_FRAGMENT_H / 2.0);

	// ===== VERTEX DEFINITIONS =====

	auto lower_left = Ogre::Vector3(col * ARENA_FRAGMENT_W, 0, row * ARENA_FRAGMENT_H);
	auto lower_center = lower_left + x_half_shift;
	auto lower_right = lower_center + x_half_shift;

	auto middle_left = lower_left + z_half_shift;
	auto center = lower_center + z_half_shift;
	auto middle_right = lower_right + z_half_shift;

	auto upper_left = middle_left + z_half_shift;
	auto upper_center = center + z_half_shift;
	auto upper_right = middle_right + z_half_shift;

	// ===== TRIANGLE DEFINITIONS =====

	// ----- LOWER LEFT -----
	tri_lower_left_1.v1 = lower_left;
	tri_lower_left_1.v2 = center;
	tri_lower_left_1.v3 = middle_left;
	tri_lower_left_1.isDestroyed = false;

	tri_lower_left_2.v1 = lower_left;
	tri_lower_left_2.v2 = lower_center;
	tri_lower_left_2.v3 = center;
	tri_lower_left_2.isDestroyed = false;

	// ----- LOWER RIGHT -----
	tri_lower_right_1.v1 = lower_center;
	tri_lower_right_1.v2 = lower_right;
	tri_lower_right_1.v3 = center;
	tri_lower_right_1.isDestroyed = false;
	
	tri_lower_right_2.v1 = lower_right;
	tri_lower_right_2.v2 = middle_right;
	tri_lower_right_2.v3 = center;
	tri_lower_right_2.isDestroyed = false;

	// ----- UPPER RIGHT -----
	tri_upper_right_1.v1 = center;
	tri_upper_right_1.v2 = upper_right;
	tri_upper_right_1.v3 = upper_center;
	tri_upper_right_1.isDestroyed = false;
	
	tri_upper_right_2.v1 = center;
	tri_upper_right_2.v2 = middle_right;
	tri_upper_right_2.v3 = upper_right;
	tri_upper_right_2.isDestroyed = false;

	// ----- UPPER LEFT -----
	tri_upper_left_1.v1 = middle_left;
	tri_upper_left_1.v2 = center;
	tri_upper_left_1.v3 = upper_left;
	tri_upper_left_1.isDestroyed = false;

	tri_upper_left_2.v1 = center;
	tri_upper_left_2.v2 = upper_center;
	tri_upper_left_2.v3 = upper_left;
	tri_upper_left_2.isDestroyed = false;
}

std::vector<ArenaTriangle*> ArenaFragment::GetTriangleReferences()
{
	std::vector<ArenaTriangle*> result;

	result.push_back(&tri_lower_left_1);
	result.push_back(&tri_lower_left_2);

	result.push_back(&tri_upper_left_1);
	result.push_back(&tri_upper_left_2);

	result.push_back(&tri_lower_right_1);
	result.push_back(&tri_lower_right_2);

	result.push_back(&tri_upper_right_1);
	result.push_back(&tri_upper_right_2);

	return result;
}

std::vector<ArenaTriangle> ArenaFragment::GetAvailableTriangles()
{
	std::vector<ArenaTriangle> result;

	// --- LEFT ---

	if (!tri_lower_left_1.isDestroyed)
	{
		result.push_back(tri_lower_left_1);
	}

	if (!tri_lower_left_2.isDestroyed)
	{
		result.push_back(tri_lower_left_2);
	}

	if (!tri_upper_left_1.isDestroyed)
	{
		result.push_back(tri_upper_left_1);
	}

	if (!tri_upper_left_2.isDestroyed)
	{
		result.push_back(tri_upper_left_2);
	}

	// --- RIGHT ---

	if (!tri_lower_right_1.isDestroyed)
	{
		result.push_back(tri_lower_right_1);
	}

	if (!tri_lower_right_2.isDestroyed)
	{
		result.push_back(tri_lower_right_2);
	}

	if (!tri_upper_right_1.isDestroyed)
	{
		result.push_back(tri_upper_right_1);
	}

	if (!tri_upper_right_2.isDestroyed)
	{
		result.push_back(tri_upper_right_2);
	}

	return result;
}

std::vector<ArenaTriangle> ArenaFragment::GetRepresentingTriangles()
{
	std::vector<ArenaTriangle> result;

	// Ha sérült, akkor részletes háromszög lista kell a fizikai reprezentációhoz

	if (tri_lower_left_1.isDestroyed || tri_lower_left_2.isDestroyed
		|| tri_lower_right_1.isDestroyed || tri_lower_right_2.isDestroyed
		|| tri_upper_left_1.isDestroyed || tri_upper_left_2.isDestroyed
		|| tri_upper_right_1.isDestroyed || tri_upper_right_2.isDestroyed)
	{
		return ArenaFragment::GetAvailableTriangles();
	}

	// Ha ép, akkor elnagyolhatjuk...

	auto origin = Ogre::Vector3(_col * ARENA_FRAGMENT_W, 0, _row * ARENA_FRAGMENT_H);
	auto x_full_shift = Ogre::Vector3(ARENA_FRAGMENT_W, 0, 0);
	auto z_full_shift = Ogre::Vector3(0, 0, ARENA_FRAGMENT_H);

	ArenaTriangle result_tri1;
	result_tri1.v1 = origin;
	result_tri1.v2 = origin + x_full_shift + z_full_shift;
	result_tri1.v3 = origin + z_full_shift;

	ArenaTriangle result_tri2;
	result_tri2.v1 = origin;
	result_tri2.v2 = origin + x_full_shift;
	result_tri2.v3 = origin + x_full_shift + z_full_shift;

	result.push_back(result_tri1);
	result.push_back(result_tri2);

	return result;
}