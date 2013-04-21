#include "meecastPanelFactory.h"
#include <FUi.h>

using namespace Tizen::Ui::Scenes;


meecastPanelFactory::meecastPanelFactory(void)
{
}

meecastPanelFactory::~meecastPanelFactory(void)
{
}

Tizen::Ui::Controls::Panel*
meecastPanelFactory::CreatePanelN(const Tizen::Base::String& panelId, const Tizen::Ui::Scenes::SceneId& sceneId)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	Tizen::Ui::Controls::Panel* pNewPanel = null;

	// TODO:
	// Add your panel creation code here
	return pNewPanel;
}
