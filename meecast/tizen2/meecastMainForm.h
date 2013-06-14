#ifndef _MEECAT_MAIN_FORM_H_
#define _MEECAST_MAIN_FORM_H_

#include <FBase.h>
#include <FMedia.h>
#include <FUi.h>
#include "configtizen.h"

class meecastMainForm
	: public Tizen::Ui::Controls::Form
	, public Tizen::Ui::IActionEventListener
	, public Tizen::Ui::Controls::IFormBackEventListener
 	, public Tizen::Ui::Scenes::ISceneEventListener
{
public:
	meecastMainForm(void);
	virtual ~meecastMainForm(void);
	bool Initialize(void);
    void CreateContextMenuList(void);
public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);
	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);
	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
									const Tizen::Ui::Scenes::SceneId& nextSceneId);

protected:
    static const int ID_BUTTON_OK = 101;
    static const int ID_BUTTON_MENU = 905;
    static const int ID_MENU_SETTINGS = 906;
    static const int ID_MENU_ABOUT = 907;
    static const int ID_BUTTON_UPDATE = 908;
private:
    Tizen::Ui::Controls::ContextMenu* __pContextMenuText;
    ConfigTizen *_config;
};

#endif	//_MEECAST_MAIN_FORM_H_
