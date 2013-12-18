#pragma once

#include "..\..\..\stdafx.h"

namespace Dodgem
{

	class Camera
	{
	private:
		Ogre::Camera* camera;
	public:
		Camera(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window);

		void SetPosition(Ogre::Vector3 pos);
		void SetLookAt(Ogre::Vector3 pos);

		void Yaw(Ogre::Radian amount);
		void Pitch(Ogre::Radian amount);
		void Rotate(Ogre::Vector3 axis, Ogre::Radian amount);

		Ogre::Vector3 GetPosition();
		Ogre::Vector3 GetDirection();
		Ogre::Vector3 GetRight();
		Ogre::Vector3 GetUp();
	};

}