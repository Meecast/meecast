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
#include "MeecastServiceMessagePort.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Io;


MeecastServiceMessagePort::MeecastServiceMessagePort(void)
	: __pLocalMessagePort(null)
	, __pRemoteMessagePort(null)
{
}

MeecastServiceMessagePort::~MeecastServiceMessagePort(void)
{
}

result
MeecastServiceMessagePort::Construct(const String& localPortName)
{
	result r = E_SUCCESS;

	__pLocalMessagePort = MessagePortManager::RequestLocalMessagePort(localPortName);
	TryReturn(__pLocalMessagePort != null, E_FAILURE, "MeeCastService : [E_FAILURE] Failed to get LocalMessagePort instance.");

	__pLocalMessagePort->AddMessagePortListener(*this);

	AppLog("MeeCastService : LocalMessagePort is ready.");

	return r;
}

void
MeecastServiceMessagePort::OnMessageReceivedN(RemoteMessagePort* pRemoteMessagePort, IMap* pMessage)
{
	String *pData = static_cast<String *>(pMessage->GetValue(String(L"UiApp")));

	AppLog("MeeCastService : Received data : %ls", pData->GetPointer());

	HashMap *pMap =	new HashMap(SingleObjectDeleter);
	pMap->Construct();

	if (pData->CompareTo(L"Reload_Config") == 0){
		App* pApp = App::GetInstance();
        AppLog("Send reload to App");
		pApp->SendUserEvent(RELOAD_CONFIG, null);
		pMap->Add(new String(L"ServiceApp"), new String(L"Service reload config"));
	}
	else

/*
	if (pData->CompareTo(L"connect") == 0)
	{
		__pRemoteMessagePort = pRemoteMessagePort;
		pMap->Add(new String(L"ServiceApp"), new String(L"ready"));
	}
	else if (pData->CompareTo(L"start") == 0)
	{
		App* pApp = App::GetInstance();
		pApp->SendUserEvent(TIMER_START, null);
		pMap->Add(new String(L"ServiceApp"), new String(L"started"));
	}
	else if (pData->CompareTo(L"stop") == 0)
	{
		App* pApp = App::GetInstance();
		pApp->SendUserEvent(TIMER_STOP, null);
		pMap->Add(new String(L"ServiceApp"), new String(L"stopped"));
	}
	else if (pData->CompareTo(L"exit") == 0)
	{
		App* pApp = App::GetInstance();
		pApp->SendUserEvent(TIMER_EXIT, null);
		pMap->Add(new String(L"ServiceApp"), new String(L"exit"));
	}
	else
	{
		pMap->Add(new String(L"ServiceApp"), new String(L"unsupported"));
	}
*/
    {
	    pMap->Add(new String(L"ServiceApp"), new String(L"unsupported"));
	    pRemoteMessagePort->SendMessage(__pLocalMessagePort, pMap);
    }
	delete pMap;

	delete pMessage;
}

result
MeecastServiceMessagePort::SendMessage(const IMap* pMessage)
{
	result r = E_SUCCESS;
	AppLog("MeeCastService : SendMessage is called.");

	if (__pRemoteMessagePort != null)
	{
		r = __pRemoteMessagePort->SendMessage(__pLocalMessagePort, pMessage);
	}
	else
	{
		r = E_FAILURE;
	}

	return r;
}
