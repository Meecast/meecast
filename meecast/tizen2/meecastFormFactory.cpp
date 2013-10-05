#include "meecastFormFactory.h"
#include "meecastMainForm.h"
#include "meecastSettingsForm.h"
#include "meecastManageLocationsForm.h"
#include "meecastSourcesForm.h"
#include "meecastCountriesForm.h"
#include "meecastRegionsForm.h"
#include "meecastStationsForm.h"
#include "meecastFullWeatherForm.h"
#include "meecastMeasurementUnitsForm.h"
#include "meecastAboutForm.h"
#include "meecastUpdateForm.h"

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

	if (formId == L"IDF_FORM"){
		meecastMainForm* pForm = new meecastMainForm();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	}else if (formId == L"SETTINGS_FORM"){
		meecastSettingsForm* pForm = new (std::nothrow) meecastSettingsForm();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	}else if (formId == L"MEASUREMENT_UNITS_FORM"){
		meecastMeasurementUnits* pForm = new (std::nothrow) meecastMeasurementUnits();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	}else if (formId == L"UPDATE_FORM"){
		meecastUpdate* pForm = new (std::nothrow) meecastUpdate();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	}else if (formId == L"MANAGE_LOCATIONS_FORM"){
		meecastManageLocationsForm* pForm = new (std::nothrow) meecastManageLocationsForm();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	}else if (formId == L"SOURCES_FORM"){
		meecastSourcesForm* pForm = new (std::nothrow) meecastSourcesForm();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	}else if (formId == L"COUNTRIES_FORM"){
		meecastCountriesForm* pForm = new (std::nothrow) meecastCountriesForm();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	}else if (formId == L"REGIONS_FORM"){
		meecastRegionsForm* pForm = new (std::nothrow) meecastRegionsForm();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	}else if (formId == L"STATIONS_FORM"){
		meecastStationsForm* pForm = new (std::nothrow) meecastStationsForm();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	}else if (formId == L"FULL_WEATHER_FORM"){
		meecastFullWeatherForm* pForm = new (std::nothrow) meecastFullWeatherForm();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	}else if (formId == L"ABOUT_FORM"){
		meecastAboutForm* pForm = new (std::nothrow) meecastAboutForm();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	}




	// TODO:
	// Add your form creation code here

	return pNewForm;
}
