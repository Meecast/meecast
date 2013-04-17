#ifndef _meecastFRAME_H_
#define _meecastFRAME_H_

#include <FBase.h>
#include <FUi.h>

class meecastFrame
	: public Tizen::Ui::Controls::Frame
{
public:
	meecastFrame(void);
	virtual ~meecastFrame(void);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
};

#endif	//_meecastFRAME_H_
