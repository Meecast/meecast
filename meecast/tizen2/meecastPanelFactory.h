#ifndef _MEECAST_PANEL_FACTORY_H_
#define _MEECAST_PANEL_FACTORY_H_

#include <FUi.h>


class meecastPanelFactory
	: public Tizen::Ui::Scenes::IPanelFactory
{
public:
	meecastPanelFactory(void);
	virtual ~meecastPanelFactory(void);

	virtual Tizen::Ui::Controls::Panel* CreatePanelN(const Tizen::Base::String& panelId, const Tizen::Ui::Scenes::SceneId& sceneId);
};

#endif // _MEECAST_PANEL_FACTORY_H_
