/**
 * Name        : meecastFrame
 * Version     :
 * Vendor      :
 * Description :
 */


#include "meecastFrame.h"
#include "meecastFormFactory.h"
#include "meecastPanelFactory.h"


using namespace Tizen::Base;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;

meecastFrame::meecastFrame(void)
{
}

meecastFrame::~meecastFrame(void)
{
}

result
meecastFrame::OnInitializing(void)
{
	// Prepare Scene management.
	SceneManager* pSceneManager = SceneManager::GetInstance();
	static meecastFormFactory formFactory;
	static meecastPanelFactory panelFactory;
	pSceneManager->RegisterFormFactory(formFactory);
	pSceneManager->RegisterPanelFactory(panelFactory);
	pSceneManager->RegisterScene(L"workflow");

	// Goto the scene.
	result r = pSceneManager->GoForward(SceneTransitionId(L"ID_SCNT_MAINSCENE"));

	// TODO:
	// Add your initialization code here
	return r;
}

result
meecastFrame::OnTerminating(void)
{
	result r = E_SUCCESS;

	// TODO:
	// Add your termination code here
	return r;
}


