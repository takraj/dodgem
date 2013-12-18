#include "stdafx.h"
#include "Arena.h"

using namespace Dodgem;

Arena::Arena(Ogre::SceneManager* sceneManager, Dodgem::PhysicsHandler* physicsHandler, size_t arena_w, size_t arena_h)
{
	this->sm = sceneManager;
	this->physics = physicsHandler;
	this->root = this->sm->getRootSceneNode();

	width = arena_w;
	height = arena_h;

	this->CreateArena();
	//this->CreateArenaPhysics();
}


Arena::~Arena(void)
{
	/*delete arenaMotionState;
	delete arenaRigidBody;
	delete arenaTriangleMesh;
	delete arenaShape;*/
}


void Arena::CreateArena()
{
	for (size_t i = 0; i < width; i++)
	{
		for (size_t j = 0; j < height; j++)
		{
			fragments.push_back(ArenaFragment(this->physics, i, j));
		}
	}

	arenaObject = sm->createManualObject("ARENA");
	arenaObject->setDynamic(true);
	arenaObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
	this->UpdateArena();
	arenaObject->end();
	
	arenaObject->setCastShadows(false);

	auto arenaNode = root->createChildSceneNode();
	arenaNode->attachObject(arenaObject);
	arenaNode->setPosition(0, 0, 0);
	arenaNode->setScale(1, 1, 1);
	arenaNode->setVisible(false);
}

void Arena::UpdateArena()
{
	for (auto& fragment : fragments)
	{
		fragment.UpdatePhysics();
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

void Arena::CreateArenaPhysics()
{
	this->arenaTriangleMesh = new btTriangleMesh();
	
	for (auto& fragment : fragments)
	{
		auto representingTriangles = fragment.GetRepresentingTriangles();
		for (auto& tri : representingTriangles)
		{
			auto vertex1 = btVector3(tri.v1.x, tri.v1.y, tri.v1.z);
			auto vertex2 = btVector3(tri.v2.x, tri.v2.y, tri.v2.z);
			auto vertex3 = btVector3(tri.v3.x, tri.v3.y, tri.v3.z);
			arenaTriangleMesh->addTriangle(vertex3, vertex2, vertex1);
		}
	}

	arenaShape = new btBvhTriangleMeshShape(arenaTriangleMesh, true);
	arenaMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	arenaRigidBody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(0, arenaMotionState, arenaShape, btVector3(0, 0, 0)));
	arenaRigidBody->setRestitution(btScalar(0.1));

	physics->AddRigidBody(arenaRigidBody);
}

void Arena::UpdateArenaPhysics()
{
	this->physics->RemoveRigidBody(arenaRigidBody);

	delete arenaTriangleMesh;
	arenaTriangleMesh = new btTriangleMesh(false, false);
	
	for (auto& fragment : fragments)
	{
		auto representingTriangles = fragment.GetRepresentingTriangles();
		for (auto& tri : representingTriangles)
		{
			auto vertex1 = btVector3(tri.v1.x, tri.v1.y, tri.v1.z);
			auto vertex2 = btVector3(tri.v2.x, tri.v2.y, tri.v2.z);
			auto vertex3 = btVector3(tri.v3.x, tri.v3.y, tri.v3.z);
			arenaTriangleMesh->addTriangle(vertex3, vertex2, vertex1);
		}
	}

	delete arenaShape;
	arenaShape = new btBvhTriangleMeshShape(arenaTriangleMesh, true);

	arenaRigidBody->setCollisionShape(arenaShape);
	physics->AddRigidBody(arenaRigidBody);
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
				fragment.SetChanged(true);
			}

			if (impactPoint2D.squaredDistance(tri->v2) < effect_radius_sqrd)
			{
				tri->isDestroyed = true;
				fragment.SetChanged(true);
			}

			if (impactPoint2D.squaredDistance(tri->v3) < effect_radius_sqrd)
			{
				tri->isDestroyed = true;
				fragment.SetChanged(true);
			}
		}
	}

	arenaObject->beginUpdate(0);
	this->UpdateArena();
	arenaObject->end();

	//this->UpdateArenaPhysics();
}