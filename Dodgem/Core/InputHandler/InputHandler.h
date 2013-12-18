#pragma once

#include "..\World\Camera\Camera.h"
#include "..\World\Meteor\Meteor.h"
#include "..\World\TestBall\TestBall.h"

namespace Dodgem
{

	class InputHandler
	{
	private:
		OIS::Keyboard* mKeyboard;
		OIS::Mouse* mMouse;
		OIS::InputManager* OISInputManager;

		Ogre::Real mTimeUntilNextToggle;
		Ogre::Real mDelta;
	public:
		InputHandler(Ogre::RenderWindow* renderWindow);
		~InputHandler(void);

		void CaptureState(Ogre::Real dt);
		bool ControlCamera(Camera* camera);
		void ControlMeteor(Meteor* meteor);
		void ControlTestBall(Camera* camera, TestBall* testBall);
	};

}