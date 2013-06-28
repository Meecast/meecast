#ifndef _MEECAST_STATIONS_FORM_H_
#define _MEECAST_STATIONS_FORM_H_

#include <FBase.h>
#include <FUi.h>
#include "../core/core.h"


class meecastStationsForm
	: public Tizen::Ui::Controls::Form
	, public Tizen::Ui::IActionEventListener
	, public Tizen::Ui::Controls::IFormBackEventListener
	, public Tizen::Ui::Controls::IListViewItemEventListener
	, public Tizen::Ui::Controls::IListViewItemProvider
 	, public Tizen::Ui::Scenes::ISceneEventListener
    , public Tizen::Ui::Controls::IFastScrollListener
{
public:
	meecastStationsForm(void);
	virtual ~meecastStationsForm(void);
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

	virtual void OnListViewItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListItemStatus status);
	virtual void OnListViewItemSwept(Tizen::Ui::Controls::ListView& listView, int index, Tizen::Ui::Controls::SweepDirection direction);
	virtual void OnListViewContextItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListContextItemStatus state);
	virtual void OnItemReordered(Tizen::Ui::Controls::ListView& view, int oldIndex, int newIndex);
	virtual Tizen::Ui::Controls::ListItemBase* CreateItem(int index, int itemWidth);
	virtual bool DeleteItem(int index, Tizen::Ui::Controls::ListItemBase* pItem, int itemWidth);
	// FastScroll
	virtual void OnFastScrollIndexSelected(Tizen::Ui::Control& source, Tizen::Base::String& index);
virtual int GetItemCount(void);
    bool LoadList(void);
protected:
private:
    Tizen::Ui::Controls::ListView* __pListView;
    Tizen::Base::String __SourceId;
    Tizen::Base::String __CountryName;
    Tizen::Base::String __RegionName;
    Tizen::Base::String __StationName;
    Tizen::Base::String __StationCode;
    Tizen::Base::Collection::HashMap *__map;
    Tizen::Base::String __dbPath;
    Core::DatabaseSqlite *__db;
    Tizen::Base::String __indexString;
};

#endif	//_MEECAST_STATIONS_FORM_H_
