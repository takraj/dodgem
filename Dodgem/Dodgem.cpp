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

Dodgem::World* world;
Ogre::Root ogreRoot;
Ogre::SceneManager* sceneManager;
Ogre::RenderWindow* window;

int _tmain(int argc, _TCHAR* argv[])
{
	ogreRoot.restoreConfig();
	ogreRoot.showConfigDialog();

	window = ogreRoot.initialise(true, "Dodge'em Game");
	loadResources();
	sceneManager = ogreRoot.createSceneManager(Ogre::SceneType::ST_EXTERIOR_CLOSE, "WORLD");

	world = new Dodgem::World(window, sceneManager, new Dodgem::InputHandler(window));
	
	ogreRoot.addFrameListener(world->GetDebugger());
	ogreRoot.addFrameListener(new Dodgem::WindowListener(window));
	ogreRoot.addFrameListener(new Dodgem::GameplayListener(world));
	ogreRoot.startRendering();

	return 0;
}
