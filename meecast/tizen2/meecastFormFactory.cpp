#include "meecastFormFactory.h"
#include "meecastMainForm.h"

using namespace Tizen::Ui::Scenes;


meecastFormFactory::meecastFormFactory(void)
{
}

meecastFormFactory::~meecastFormFactory(void)
{
}

Tizen::Ui::Controls::Form*
meecastFormFactory::CreateFormN(const Tizen::Base::String& formId, const Tizen::Ui::Scenes::SceneId& sceneId)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	Tizen::Ui::Controls::Form* pNewForm = null;

	if (formId == L"IDF_FORM")
	{
		meecastMainForm* pForm = new meecastMainForm();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	}
	// TODO:
	// Add your form creation code here

	return pNewForm;
}
