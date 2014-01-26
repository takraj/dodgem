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
	
	this->arenaRigidBody = NULL;
	this->CreateArena();
}


Arena::~Arena(void)
{
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
	arenaObject->setDynamic(true);
	arenaObject->begin("Ground", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	this->UpdateArena();
	arenaObject->end();
	
	arenaObject->setCastShadows(false);

	auto arenaNode = root->createChildSceneNode();
	arenaNode->attachObject(arenaObject);
	arenaNode->setPosition(0, 0, 0);
	arenaNode->setScale(1, 1, 1);
	arenaNode->setVisible(true);
}

void Arena::UpdateArena()
{
	bool needToCreate = true;
	bool destroy = true;

	if (this->arenaRigidBody != NULL)
	{
		this->physics->RemoveRigidBody(arenaRigidBody);
		delete arenaTriangleMesh;
		delete arenaShape;
		needToCreate = false;
	}

	this->arenaTriangleMesh = new btTriangleMesh(false, false);
	size_t do_not_check = this->height;

	for (auto& fragment : fragments)
	{
		auto representingTriangles = fragment.GetRepresentingTriangles();
		for (auto tri : representingTriangles)
		{
			arenaObject->position(tri->v3);
			arenaObject->normal(Ogre::Vector3::UNIT_Y);
			arenaObject->textureCoord(tri->v3.x * 0.1, tri->v3.z * 0.1);
			arenaObject->position(tri->v2);
			arenaObject->normal(Ogre::Vector3::UNIT_Y);
			arenaObject->textureCoord(tri->v2.x * 0.1, tri->v2.z * 0.1);
			arenaObject->position(tri->v1);
			arenaObject->normal(Ogre::Vector3::UNIT_Y);
			arenaObject->textureCoord(tri->v1.x * 0.1, tri->v1.z * 0.1);

			auto vertex1 = physics->AsBulletVector(tri->v1);
			auto vertex2 = physics->AsBulletVector(tri->v2);
			auto vertex3 = physics->AsBulletVector(tri->v3);
			arenaTriangleMesh->addTriangle(vertex3, vertex2, vertex1);

			destroy = false;
		}
	}

	if (destroy)
	{
		this->arenaRigidBody = NULL;
		delete this->arenaTriangleMesh;
		return;
	}

	arenaShape = new btBvhTriangleMeshShape(arenaTriangleMesh, true);

	if (needToCreate)
	{
		arenaMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
		arenaRigidBody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(0, arenaMotionState, arenaShape, btVector3(0, 0, 0)));
		arenaRigidBody->setRestitution(btScalar(0.1));
		arenaRigidBody->setFriction(1);
	}

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
		auto tris = fragment.GetAvailableTriangles();
		for (auto tri : tris)
		{
			if ((impactPoint2D.squaredDistance(tri->v1) < effect_radius_sqrd)
				|| (impactPoint2D.squaredDistance(tri->v2) < effect_radius_sqrd)
				|| (impactPoint2D.squaredDistance(tri->v3) < effect_radius_sqrd))
			{
				tri->isDestroyed = true;
			}
		}
	}

	arenaObject->beginUpdate(0);
	this->UpdateArena();
	arenaObject->end();
}
