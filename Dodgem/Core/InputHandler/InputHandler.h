#pragma once

#include "..\World\Camera\Camera.h"
#include "..\World\Meteor\Meteor.h"
#include "..\World\TestBall\TestBall.h"
#include "..\World\Skate\Skate.h"

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
		bool ControlQuit();
		bool ControlCamera(Camera* camera);
		void ControlMeteor(Meteor* meteor);
		void ControlTestBalls(Camera* camera, TestBall* testBall1, TestBall* testBall2);
		void ControlSkates(Skate* skate1, Skate* skate2);
		bool ControlPause();
		bool ControlRestart();
	};

}