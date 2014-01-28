#include "stdafx.h"
#include "InputHandler.h"

using namespace Dodgem;

InputHandler::InputHandler(Ogre::RenderWindow* renderWindow)
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");

	OIS::ParamList pl;

	size_t windowHnd = 0;
	renderWindow->getCustomAttribute("WINDOW", &windowHnd);
	std::ostringstream windowHndStr;
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	OISInputManager = OIS::InputManager::createInputSystem( pl );

	mKeyboard = static_cast<OIS::Keyboard*>(OISInputManager->createInputObject( OIS::OISKeyboard, false ));
	mMouse = static_cast<OIS::Mouse*>(OISInputManager->createInputObject( OIS::OISMouse, false ));
}


InputHandler::~InputHandler(void)
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Shutting down OIS ***");
	OIS::InputManager::destroyInputSystem(OISInputManager);
}

void InputHandler::CaptureState(Ogre::Real dt)
{
	mKeyboard->capture();
	mMouse->capture();

	if (mTimeUntilNextToggle >= 0)
	{
		mTimeUntilNextToggle -= dt;
	}

	this->mDelta = dt;
}

bool InputHandler::ControlQuit()
{
	if (mKeyboard->isKeyDown(OIS::KC_ESCAPE) && mTimeUntilNextToggle < 0)
	{
		mTimeUntilNextToggle = 0.5f;
		Ogre::LogManager::getSingletonPtr()->logMessage("escape button down");
		return false;
	}

	return true;
}

bool InputHandler::ControlRestart()
{
	if (mKeyboard->isKeyDown(OIS::KC_RETURN) && mTimeUntilNextToggle < 0)
	{
		mTimeUntilNextToggle = 0.5f;
		Ogre::LogManager::getSingletonPtr()->logMessage("restart button down");
		return false;
	}

	return true;
}

bool InputHandler::ControlPause()
{
	if (mKeyboard->isKeyDown(OIS::KC_SPACE) && mTimeUntilNextToggle < 0)
	{
		mTimeUntilNextToggle = 0.5f;
		Ogre::LogManager::getSingletonPtr()->logMessage("pause button down");
		return false;
	}

	return true;
}

bool InputHandler::ControlCamera(Camera* camera)
{
	const float mousesensitivity = 1.0f;
	const float cameraMoveSpeed = 10.0f * this->mDelta;
	const float cameraRotateSpeed = 50.0f * this->mDelta;

	auto campos = camera->GetPosition();
	auto camdir = camera->GetDirection();
	auto camright = camera->GetRight();
	auto camup = camera->GetUp();
	auto cammove = Ogre::Vector3(0, 0, 0);

	auto& ms = mMouse->getMouseState();

	camera->Yaw(Ogre::Radian(-cameraRotateSpeed * mousesensitivity * ms.X.rel * 0.01f));
	camera->Pitch(Ogre::Radian(-cameraRotateSpeed * mousesensitivity * ms.Y.rel * 0.01f));

	if (mKeyboard->isKeyDown(OIS::KC_I))
	{
		cammove += camdir;
	}

	if (mKeyboard->isKeyDown(OIS::KC_K))
	{
		cammove -= camdir;
	}

	if (mKeyboard->isKeyDown(OIS::KC_J))
	{
		cammove -= camright;
	}

	if (mKeyboard->isKeyDown(OIS::KC_L))
	{
		cammove += camright;
	}

	if (mKeyboard->isKeyDown(OIS::KC_U))
	{
		cammove += camup;
	}

	if (mKeyboard->isKeyDown(OIS::KC_O))
	{
		cammove -= camup;
	}

	cammove.normalise();
	camera->SetPosition(campos + cammove * cameraMoveSpeed);

	return true;
}

void InputHandler::ControlMeteor(Meteor* meteor)
{
	if (mKeyboard->isKeyDown(OIS::KC_SPACE) && mTimeUntilNextToggle < 0)
	{
		meteor->Create();
		mTimeUntilNextToggle = 0.5f;
	}
}

void InputHandler::ControlTestBalls(Camera* camera, TestBall* testBall1, TestBall* testBall2)
{
	auto upForce = camera->GetUp();
	upForce.y = 0;
	upForce.normalise();
	upForce *= 600;

	auto rightForce = camera->GetRight();
	rightForce.y = 0;
	rightForce.normalise();
	rightForce *= 600;

	// -- ball 1 --

	auto forceVector1 = Ogre::Vector3(0, 0, 0);

	if (mKeyboard->isKeyDown(OIS::KC_LEFT))
	{
		forceVector1 -= upForce;
	}

	if (mKeyboard->isKeyDown(OIS::KC_RIGHT))
	{
		forceVector1 += upForce;
	}

	if (mKeyboard->isKeyDown(OIS::KC_UP))
	{
		forceVector1 -= rightForce;
	}

	if (mKeyboard->isKeyDown(OIS::KC_DOWN))
	{
		forceVector1 += rightForce;
	}

	testBall1->ApplyForce(forceVector1);

	// -- ball 2 --

	auto forceVector2 = Ogre::Vector3(0, 0, 0);

	if (mKeyboard->isKeyDown(OIS::KC_A))
	{
		forceVector2 -= upForce;
	}

	if (mKeyboard->isKeyDown(OIS::KC_D))
	{
		forceVector2 += upForce;
	}

	if (mKeyboard->isKeyDown(OIS::KC_W))
	{
		forceVector2 -= rightForce;
	}

	if (mKeyboard->isKeyDown(OIS::KC_S))
	{
		forceVector2 += rightForce;
	}

	testBall2->ApplyForce(forceVector2);
}

void InputHandler::ControlSkates(Skate* skate1, Skate* skate2)
{
	// -- ball 1 --

	if (mKeyboard->isKeyDown(OIS::KC_UP))
	{
		skate1->ApplyForce(Ogre::Vector3(0, 0, 1000));
	}

	if (mKeyboard->isKeyDown(OIS::KC_DOWN))
	{
		skate1->ApplyForce(Ogre::Vector3(0, 0, -1000));
	}

	if (mKeyboard->isKeyDown(OIS::KC_LEFT))
	{
		skate1->Steer(Ogre::Vector3(150, 0, 0));
	}

	if (mKeyboard->isKeyDown(OIS::KC_RIGHT))
	{
		skate1->Steer(Ogre::Vector3(-150, 0, 0));
	}

	// -- ball 2 --

	if (mKeyboard->isKeyDown(OIS::KC_W))
	{
		skate2->ApplyForce(Ogre::Vector3(0, 0, 1000));
	}

	if (mKeyboard->isKeyDown(OIS::KC_S))
	{
		skate2->ApplyForce(Ogre::Vector3(0, 0, -1000));
	}

	if (mKeyboard->isKeyDown(OIS::KC_A))
	{
		skate2->Steer(Ogre::Vector3(150, 0, 0));
	}

	if (mKeyboard->isKeyDown(OIS::KC_D))
	{
		skate2->Steer(Ogre::Vector3(-150, 0, 0));
	}
	
}