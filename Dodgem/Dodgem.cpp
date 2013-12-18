// Dodgem.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Core\WindowListener\WindowListener.h"
#include "Core\GameplayListener\GameplayListener.h"
#include "Core\InputHandler\InputHandler.h"
#include "Core\World\World.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Ogre::Root ogreRoot;

	ogreRoot.restoreConfig();
	ogreRoot.showConfigDialog();

	auto window = ogreRoot.initialise(true, "Dodge'em Game");
	auto sceneManager = ogreRoot.createSceneManager(Ogre::SceneType::ST_EXTERIOR_CLOSE, "WORLD");

	auto inputHandler = new Dodgem::InputHandler(window);
	auto world = new Dodgem::World(window, sceneManager, inputHandler);
	
	ogreRoot.addFrameListener(world->GetDebugger());
	ogreRoot.addFrameListener(new Dodgem::WindowListener(window));
	ogreRoot.addFrameListener(new Dodgem::GameplayListener(world));
	ogreRoot.startRendering();

	delete world;
	delete inputHandler;

	return 0;
}
