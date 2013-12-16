#include "Camera.h"

using namespace Dodgem;

Camera::Camera(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window)
{
	camera = sceneManager->createCamera("MAIN_CAMERA");
	camera->setFarClipDistance(100000);
	camera->setNearClipDistance(0.1f);
	window->addViewport(camera);

	this->SetPosition(Ogre::Vector3(3000, 2000, 5000));
	this->SetLookAt(Ogre::Vector3(3000, 0, 1500));
}

void Camera::SetPosition(Ogre::Vector3 pos)
{
	camera->setPosition(pos); 
}

void Camera::SetLookAt(Ogre::Vector3 pos)
{
	camera->lookAt(pos);
}

void Camera::Yaw(Ogre::Radian amount)
{
	camera->yaw(amount);
}

void Camera::Pitch(Ogre::Radian amount)
{
	camera->pitch(amount);
}

void Camera::Rotate(Ogre::Vector3 axis, Ogre::Radian amount)
{
	camera->rotate(axis, amount);
}

Ogre::Vector3 Camera::GetPosition()
{
	return camera->getPosition();
}

Ogre::Vector3 Camera::GetDirection()
{
	return camera->getDirection();
}

Ogre::Vector3 Camera::GetRight()
{
	return camera->getRight();
}

Ogre::Vector3 Camera::GetUp()
{
	return camera->getUp();
}