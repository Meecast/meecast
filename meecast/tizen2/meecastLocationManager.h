/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Meecast for Tizen2
 *
 * Copyright (C) 2013 Vlad Vasilyeu
 *     for the code
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU  General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 *
 * You should have received a copy of the GNU  General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/

#ifndef MEECAST_LOCATION_MANAGER_H_
#define MEECAST_LOCATION_MANAGER_H_

class meecastLocationManager
	:public Tizen::Base::Runtime::Thread
{
public:
	meecastLocationManager(void);
	virtual ~meecastLocationManager(void);
	result Construct(const Tizen::Ui::Control& uiControl);
	Tizen::Base::Object* Run(void);

private:
	const Tizen::Ui::Control* __pUiControl;
};

#endif /* MEECAST_LOCATION_MANAGER_H_ */

