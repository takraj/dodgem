#include "stdafx.h"
#include "HUD.h"

using namespace Dodgem;

HUD::HUD(void)
{
	Ogre::Overlay* mainOverlay = Ogre::OverlayManager::getSingleton().getByName("MainOverlay");
	mainOverlay->show();

	Ogre::OverlayElement* ke = Ogre::OverlayManager::getSingleton().createOverlayElementFromTemplate("KillsTemplate", "TextArea", Ogre::String("akarmi") + "Kills");
	ke->setTop(ke->getTop() + 0.04 + 0.04 * (0+1));
	ke->setCaption("akarmi");
}


HUD::~HUD(void)
{
}

void HUD::refreshScore(int player1, int player2)
{
	Ogre::String newCaption = Ogre::StringConverter::toString(player1) + " : " + Ogre::StringConverter::toString(player2);
	Ogre::OverlayElement* score = Ogre::OverlayManager::getSingleton().getOverlayElement("Score");
	score->setCaption(newCaption);
}