#include "Arena.h"

using namespace Dodgem;

Arena::Arena(Ogre::SceneManager* sceneManager, size_t arena_w, size_t arena_h)
{
	this->sm = sceneManager;
	this->root = sm->getRootSceneNode();

	width = arena_w;
	height = arena_h;

	this->CreateArena();
}


Arena::~Arena(void)
{
	//delete arenaObject;
}


void Arena::CreateArena()
{
	for (size_t i = 0; i < width; i++)
	{
		for (size_t j = 0; j < height; j++)
		{
			fragments.push_back(ArenaFragment(i, j));
		}
	}

	arenaObject = sm->createManualObject("ARENA");
	arenaObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
	this->UpdateArena();
	arenaObject->end();

	arenaObject->setCastShadows(false);

	auto arenaNode = root->createChildSceneNode();
	arenaNode->attachObject(arenaObject);
	arenaNode->setPosition(0, 0, 0);
	arenaNode->setScale(1, 1, 1);
}

void Arena::UpdateArena()
{
	for (auto& fragment : fragments)
	{
		auto representingTriangles = fragment.GetRepresentingTriangles();
		for (auto& tri : representingTriangles)
		{
			arenaObject->position(tri.v3);
			arenaObject->position(tri.v2);
			arenaObject->position(tri.v2);
			arenaObject->position(tri.v1);
			arenaObject->position(tri.v1);
			arenaObject->position(tri.v3);
		}
	}
}

Ogre::Vector3 Arena::GetBounds()
{
	return arenaObject->getBoundingBox().getSize();
}

void Arena::Impact(Ogre::Vector3 impactPoint, Ogre::Real impactRadius)
{
	const auto effect_radius = impactRadius * 1.5f;
	const auto effect_radius_sqrd = effect_radius * effect_radius;
	const auto impactPoint2D = Ogre::Vector3(impactPoint.x, 0, impactPoint.z);

	for (auto& fragment : fragments)
	{
		auto tris = fragment.GetTriangleReferences();
		for (auto& tri : tris)
		{
			if (impactPoint2D.squaredDistance(tri->v1) < effect_radius_sqrd)
			{
				tri->isDestroyed = true;
			}

			if (impactPoint2D.squaredDistance(tri->v2) < effect_radius_sqrd)
			{
				tri->isDestroyed = true;
			}

			if (impactPoint2D.squaredDistance(tri->v3) < effect_radius_sqrd)
			{
				tri->isDestroyed = true;
			}
		}
	}

	arenaObject->beginUpdate(0);
	this->UpdateArena();
	arenaObject->end();
}