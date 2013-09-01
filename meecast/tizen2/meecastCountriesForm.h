#ifndef _MEECAST_COUNTRIES_FORM_H_
#define _MEECAST_COUNTRIES_FORM_H_

#include <FBase.h>
#include <FUi.h>
#include "../core/core.h"

#include <libintl.h>
#include <locale.h>

#define SAFE_DELETE(x)  if (x) { delete x; x = null; }
#define _(String) gettext(String)
static const int INDICATE_HEIGHT = 70;


class meecastCountriesForm
	: public Tizen::Ui::Controls::Form
	, public Tizen::Ui::Controls::ISearchBarEventListener
	, public Tizen::Ui::IKeypadEventListener
	, public Tizen::Ui::ITextEventListener
	, public Tizen::Ui::IActionEventListener
	, public Tizen::Ui::Controls::IFormBackEventListener
	, public Tizen::Ui::Controls::IListViewItemEventListener
	, public Tizen::Ui::Controls::IListViewItemProvider
 	, public Tizen::Ui::Scenes::ISceneEventListener
    , public Tizen::Ui::Controls::IFastScrollListener
	, public Tizen::Ui::ITouchEventListener
{
public:
	meecastCountriesForm(void);
	virtual ~meecastCountriesForm(void);
	bool Initialize(void);
    void UpdateSearchResult(void);
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
	virtual int GetItemCount(void);
    bool LoadList(void);
	// FastScroll
	virtual void OnFastScrollIndexSelected(Tizen::Ui::Control& source, Tizen::Base::String& index);
	// ISearchBarEventListener
	virtual void OnSearchBarModeChanged(Tizen::Ui::Controls::SearchBar& source, Tizen::Ui::Controls::SearchBarMode mode);
	virtual void OnTextValueChanged(const Tizen::Ui::Control& source);
	virtual void OnTextValueChangeCanceled(const Tizen::Ui::Control& source){}

	// IKeypadEventListener
	virtual void OnKeypadActionPerformed(Tizen::Ui::Control &source, Tizen::Ui::KeypadAction keypadAction);
	virtual void OnKeypadClosed(Tizen::Ui::Control &source);
	virtual void OnKeypadOpened(Tizen::Ui::Control &source);
	virtual void OnKeypadWillOpen(Tizen::Ui::Control &source);
	virtual void OnKeypadBoundsChanged(Tizen::Ui::Control& source);
	//ITouchEventListener
	virtual void  OnTouchDoublePressed (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo){}
	virtual void  OnTouchFocusIn (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo){}
	virtual void  OnTouchFocusOut (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo){}
	virtual void  OnTouchLongPressed (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo){}
	virtual void  OnTouchMoved (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo){}
	virtual void  OnTouchPressed (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo);
	virtual void  OnTouchReleased (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo){}


protected:
private:
    Tizen::Ui::Controls::ListView* __pListView;
    Tizen::Base::String __SourceId;
    Tizen::Base::Collection::HashMap *__map;
    Tizen::Base::String __dbPath;
    Core::DatabaseSqlite *__db;
    Tizen::Base::String __indexString;
	Tizen::Ui::Controls::SearchBar* __pSearchBar;
};

#endif	//_MEECAST_COUNTRIES_FORM_H_
