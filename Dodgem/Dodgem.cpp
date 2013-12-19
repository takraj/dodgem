// Dodgem.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Core\WindowListener\WindowListener.h"
#include "Core\GameplayListener\GameplayListener.h"
#include "Core\InputHandler\InputHandler.h"
#include "Core\World\World.h"

void loadResources()
{
	// set up resources
	// Load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load("resources.cfg");
 
	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
 
	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups(); 
}

int _tmain(int argc, _TCHAR* argv[])
{
	Ogre::Root ogreRoot;

	ogreRoot.restoreConfig();
	ogreRoot.showConfigDialog();

	auto window = ogreRoot.initialise(true, "Dodge'em Game");
	loadResources();
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
