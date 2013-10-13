#ifndef _MEECAST_APPEREANCE_FORM_H_
#define _MEECAST_APPEREANCE_FORM_H_

#include <FBase.h>
#include <FUi.h>
#include "configtizen.h"
#include <FMedia.h>

#include <libintl.h>
#include <locale.h>

#define SAFE_DELETE(x)  if (x) { delete x; x = null; }
#define _(String) gettext(String)



class meecastAppereanceForm
	: public Tizen::Ui::Controls::Form
	, public Tizen::Ui::IActionEventListener
	, public Tizen::Ui::Controls::IFormBackEventListener
    , public Tizen::Ui::Controls::IGroupedListViewItemEventListener
    , public Tizen::Ui::Controls::IGroupedListViewItemProvider
 	, public Tizen::Ui::Scenes::ISceneEventListener
    , public Tizen::Ui::IAdjustmentEventListener
{
public:
	meecastAppereanceForm(void);
	virtual ~meecastAppereanceForm(void);
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
// IGroupedListViewItemEventListener
    virtual void OnGroupedListViewItemSwept(Tizen::Ui::Controls::GroupedListView& listView, int groupIndex, int itemIndex, Tizen::Ui::Controls::SweepDirection direction) {};
	virtual void OnGroupedListViewItemStateChanged(Tizen::Ui::Controls::GroupedListView& listView, int groupIndex, int itemIndex, int elementId, Tizen::Ui::Controls::ListItemStatus state);
    virtual void OnGroupedListViewContextItemStateChanged(Tizen::Ui::Controls::GroupedListView& listView, int groupIndex, int itemIndex, int elementId, Tizen::Ui::Controls::ListContextItemStatus state) {};

    // IGroupedListViewItemProvider
	virtual int GetGroupCount(void);
	virtual int GetItemCount(int groupIndex);
	virtual Tizen::Ui::Controls::GroupItem* CreateGroupItem(int groupIndex, int itemWidth);
	virtual bool DeleteGroupItem(int groupIndex, Tizen::Ui::Controls::GroupItem* pItem, int itemWidth);
	virtual Tizen::Ui::Controls::ListItemBase* CreateItem(int groupIndex, int itemIndex, int itemWidth);
	virtual bool DeleteItem(int groupIndex, int itemIndex, Tizen::Ui::Controls::ListItemBase* pItem, int itemWidth);

    void CreateGroupedListView(void);
    virtual void OnAdjustmentValueChanged(const Tizen::Ui::Control& source, int adjustment);

protected:
private:
	Tizen::Ui::Controls::GroupedListView* __pList;
    Tizen::Ui::Controls::Slider* __pSliderX;
    Tizen::Ui::Controls::Slider* __pSliderY;
	//Tizen::Ui::Controls::ListContextItem* __pItemContext;
};

#endif	//_MEECAST_APPEREANCE_FORM_H_
