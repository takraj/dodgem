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

void InputHandler::CaptureState()
{
	mKeyboard->capture();
	mMouse->capture();
}

bool InputHandler::ControlCamera(Camera* camera, Ogre::Real dt)
{
	const float mousesensitivity = 1.0f;
	const float cameraMoveSpeed = 1000.0f * dt;
	const float cameraRotateSpeed = 50.0f * dt;

	auto campos = camera->GetPosition();
	auto camdir = camera->GetDirection();
	auto camright = camera->GetRight();
	auto camup = camera->GetUp();
	auto cammove = Ogre::Vector3(0, 0, 0);

	auto& ms = mMouse->getMouseState();

	if (ms.buttonDown(OIS::MB_Right))
	{
		cammove += camright * ((float)ms.X.rel) * 0.01f;
		cammove += camup * ((float)ms.Y.rel) * 0.01f;
	}
	else
	{
		camera->Yaw(Ogre::Radian(-cameraRotateSpeed * mousesensitivity * ms.X.rel * 0.01f));
		camera->Pitch(Ogre::Radian(-cameraRotateSpeed * mousesensitivity * ms.Y.rel * 0.01f));
	}

	if (ms.buttonDown(OIS::MB_Left) && mTimeUntilNextToggle < 0)
	{
		mTimeUntilNextToggle = 0.5f;
		Ogre::LogManager::getSingletonPtr()->logMessage("left button down");
	}

	if (mTimeUntilNextToggle >= 0)
	{
		mTimeUntilNextToggle -= dt;
	}

	if (mKeyboard->isKeyDown(OIS::KC_ESCAPE) && mTimeUntilNextToggle < 0)
	{
		mTimeUntilNextToggle = 0.5f;
		Ogre::LogManager::getSingletonPtr()->logMessage("escape button down");
		return false;
	}

	if (mKeyboard->isKeyDown(OIS::KC_UP) || mKeyboard->isKeyDown(OIS::KC_W))
	{
		cammove += camdir;
	}

	if (mKeyboard->isKeyDown(OIS::KC_DOWN) || mKeyboard->isKeyDown(OIS::KC_S))
	{
		cammove -= camdir;
	}

	if (mKeyboard->isKeyDown(OIS::KC_A))
	{
		cammove -= camright;
	}

	if (mKeyboard->isKeyDown(OIS::KC_D))
	{
		cammove += camright;
	}

	if (mKeyboard->isKeyDown(OIS::KC_Q))
	{
		cammove += camup;
	}

	if (mKeyboard->isKeyDown(OIS::KC_E))
	{
		cammove -= camup;
	}

	cammove.normalise();
	camera->SetPosition(campos + cammove * cameraMoveSpeed);

	if (mKeyboard->isKeyDown(OIS::KC_LEFT))
	{
		camera->Rotate(Ogre::Vector3::UNIT_Y,Ogre::Radian(cameraRotateSpeed * 0.02f));
	}

	if (mKeyboard->isKeyDown(OIS::KC_RIGHT))
	{
		camera->Rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(-cameraRotateSpeed * 0.02f));
	}

	camright = camera->GetRight();

	if (mKeyboard->isKeyDown(OIS::KC_PGUP))
	{
		camera->Rotate(camright, Ogre::Radian(cameraRotateSpeed * 0.02f));
	}

	if (mKeyboard->isKeyDown(OIS::KC_PGDOWN))
	{
		camera->Rotate(camright, Ogre::Radian(-cameraRotateSpeed * 0.02f));
	}

	return true;
}

void InputHandler::ControlMeteor(Meteor* meteor, Ogre::Real dt)
{
	if (mKeyboard->isKeyDown(OIS::KC_SPACE) && mTimeUntilNextToggle < 0)
	{
		meteor->Create();
		mTimeUntilNextToggle = 0.5f;
	}
}