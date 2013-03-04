/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2013 Vlad Vasilyeu
 *     for the code
 *
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU weather-config.h General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#include "active.h"


ActiveFrame::ActiveFrame() : SceneCover(this) {
	//std::cerr<<"Start ActiveFrame::ActiveFrame"<<std::endl;
	//_current = new DataModelQt(new DataItem, qApp);
	_qml = QmlDocument::create("asset:///qml/active_frame.qml").parent(this);
    //controller->active_frame_qview(qml);
 //   qml->setContextProperty("Current", _current);
            // You can do it now or trigger the slot whne application is moved to baackground.
        // QObject::connect(Application::instance(), SIGNAL(thumbnail()), this, SLOT(update()));
        // and/or disable updates when it's in foreground

}

void
ActiveFrame::StartFrame(){
	mMainContainer = _qml->createRootObject<Container>();
    // Set the content of ActiveFrame
    setContent(mMainContainer);
    QObject::connect(Application::instance(), SIGNAL(thumbnail()),
            this, SLOT(update()));
    update();
}

QmlDocument *
ActiveFrame::qml(){
	return _qml;
}
ActiveFrame::~ActiveFrame() {
        // TODO Auto-generated destructor stub
}


void
ActiveFrame::update() {


}

