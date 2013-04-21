#ifndef _MEECAST_FORM_FACTORY_H_
#define _MEECAST_FORM_FACTORY_H_

#include <FUi.h>


class meecastFormFactory
	: public Tizen::Ui::Scenes::IFormFactory
{
public:
	meecastFormFactory(void);
	virtual ~meecastFormFactory(void);

	virtual Tizen::Ui::Controls::Form* CreateFormN(const Tizen::Base::String& formId, const Tizen::Ui::Scenes::SceneId& sceneId);
};

#endif // _MEECAST_FORM_FACTORY_H_
