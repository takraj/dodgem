#pragma once

#include <OIS\OIS.h>
#include <OGRE\OgreLogManager.h>
#include <OGRE\OgreMath.h>
#include <OGRE\OgreRenderWindow.h>

#include "..\World\Camera\Camera.h"
#include "..\World\Meteor\Meteor.h"

namespace Dodgem
{

	class InputHandler
	{
	private:
		OIS::Keyboard* mKeyboard;
		OIS::Mouse* mMouse;
		OIS::InputManager* OISInputManager;

		float mTimeUntilNextToggle;
	public:
		InputHandler(Ogre::RenderWindow* renderWindow);
		~InputHandler(void);

		void CaptureState();
		bool ControlCamera(Camera* camera, Ogre::Real dt);
		void ControlMeteor(Meteor* meteor, Ogre::Real dt);
	};

}