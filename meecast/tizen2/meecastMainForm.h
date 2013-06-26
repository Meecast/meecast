#ifndef _MEECAT_MAIN_FORM_H_
#define _MEECAST_MAIN_FORM_H_

#include <FBase.h>
#include <FMedia.h>
#include <FUi.h>
#include "configtizen.h"


#define SAFE_DELETE(x)  if (x) { delete x; x = null; }



class meecastMainForm
	: public Tizen::Ui::Controls::Form
	, public Tizen::Ui::IActionEventListener
	, public Tizen::Ui::Controls::IFormBackEventListener
 	, public Tizen::Ui::Scenes::ISceneEventListener
	, public Tizen::Ui::Controls::IListViewItemEventListener
	, public Tizen::Ui::Controls::IListViewItemProvider
    , public Tizen::Ui::ITouchEventListener
    , public Tizen::Base::Runtime::ITimerEventListener
     {
public:
	meecastMainForm(void);
	virtual ~meecastMainForm(void);
	bool Initialize(void);
    void CreateContextMenuList(Tizen::Graphics::Point Corner_Point);
public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);
	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);
	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
									const Tizen::Ui::Scenes::SceneId& nextSceneId);
// from ITouchEventListener
	virtual void OnTouchPressed(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo);
	virtual void OnTouchDoublePressed(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo){}
	virtual void OnTouchFocusIn(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo){}
	virtual void OnTouchFocusOut(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo){}
	virtual void OnTouchLongPressed(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo){}
	virtual void OnTouchMoved(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo){}
	virtual void OnTouchReleased(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo){}
    void ReInitElements(void);


	virtual void OnListViewItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListItemStatus status);
	virtual void OnListViewItemSwept(Tizen::Ui::Controls::ListView& listView, int index, Tizen::Ui::Controls::SweepDirection direction);
	virtual void OnListViewContextItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListContextItemStatus state);
	virtual void OnItemReordered(Tizen::Ui::Controls::ListView& view, int oldIndex, int newIndex);
	virtual Tizen::Ui::Controls::ListItemBase* CreateItem(int index, int itemWidth);
	virtual bool DeleteItem(int index, Tizen::Ui::Controls::ListItemBase* pItem, int itemWidth);
	virtual int GetItemCount(void);
    Tizen::Graphics::Color* GetTemperatureColor(int t);


    virtual void OnTimerExpired(Tizen::Base::Runtime::Timer& timer);
protected:
    static const int ID_BUTTON_OK = 101;
    static const int ID_BUTTON_MENU = 905;
    static const int ID_MENU_SETTINGS = 906;
    static const int ID_MENU_ABOUT = 907;
    static const int ID_BUTTON_UPDATE = 908;
    static const int DURATION = 500;
    static const int PROGRESS_COUNT = 30;
private:
    Tizen::Ui::Controls::ContextMenu* __pContextMenuText;
    ConfigTizen *_config;
    int _dayCount;
    Tizen::Ui::Controls::Animation* __pAnimation;
	Tizen::Base::Collection::ArrayList* __pAnimationFrameList;
    Tizen::Ui::Controls::FooterItem* __updateButton;
    Tizen::Base::Runtime::Timer* __updateTimer;
};

#endif	//_MEECAST_MAIN_FORM_H_
