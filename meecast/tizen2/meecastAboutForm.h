#ifndef _MEECAST_ABOUT_FORM_H_
#define _MEECAST_ABOUT_FORM_H_

#include <FBase.h>
#include <FMedia.h>
#include <FUi.h>

#include <libintl.h>
#include <locale.h>

#define SAFE_DELETE(x)  if (x) { delete x; x = null; }
#define _(String) gettext(String)



class meecastAboutForm
	: public Tizen::Ui::Controls::Form
	, public Tizen::Ui::IActionEventListener
	, public Tizen::Ui::Controls::IFormBackEventListener
 	, public Tizen::Ui::Scenes::ISceneEventListener
{
public:
	meecastAboutForm(void);
	virtual ~meecastAboutForm(void);
	bool Initialize(void);
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
private:
};

#endif	//_MEECAST_ABOUT_FORM_H_
