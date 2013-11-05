/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Meecast for Tizen
 *
 * Copyright (C) 2012 - 2013 Vlad Vasilyeu
 * 	for the code
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

#include "MeecastDynamicBoxAppProvider.h"
#include "MeecastDynamicBoxAppProviderFactory.h"

using namespace Tizen::Base;
using namespace Tizen::Shell;

MeecastDynamicBoxAppProviderFactory::MeecastDynamicBoxAppProviderFactory()
{

}

MeecastDynamicBoxAppProviderFactory::~MeecastDynamicBoxAppProviderFactory()
{

}

AppWidgetProvider*
MeecastDynamicBoxAppProviderFactory::CreateInstance(const String& name, float width, float height, const String& userInfo)
{
	// TODO:
	// Create the AppWidgetProvider instance through the constructor.
    _mdbaprovider = new MeecastDynamicBoxAppProvider();
	return _mdbaprovider;
}
void
MeecastDynamicBoxAppProviderFactory::Update(){
    _mdbaprovider->Update();
}
