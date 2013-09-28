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
/*******************************************************************************/

//#include "SampleType.h"
#include "meecastServiceProxy.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Runtime;
using namespace Tizen::Base::Collection;
using namespace Tizen::Io;

static const int CHECK_INTERVAL = 1000; // Checking interval in sec.

static const wchar_t* LOCAL_MESSAGE_PORT_NAME = L"UI_PORT";

MeecastServiceProxy::MeecastServiceProxy(void)
	: __pLocalMessagePort(null)
	, __pRemoteMessagePort(null)
{
}

MeecastServiceProxy::~MeecastServiceProxy(void)
{
}

result
MeecastServiceProxy::Construct(const AppId& appId, const String& remotePortName)
{
	result r = E_FAILURE;

	__appId = appId;

	AppManager* pAppManager = AppManager::GetInstance();
	TryReturn(pAppManager != null, E_FAILURE, "SampleUiApp : Fail to get AppManager instance.");

	AppLog("SampleUiApp : Checking service status for time out 5 sec...");
/*
	for (int i=0; i < 5; ++i)
	{
		if (pAppManager->IsRunning(__appId))
		{
			AppLog("SampleUiApp : Service is ready !!!");
			break;
		}
		else
		{
			AppLog("SampleUiApp : Service is not ready !!! try to launch !!! ");
			r = pAppManager->LaunchApplication(__appId, null);
			TryReturn(!IsFailed(r), r, "SampleUiApp : [%s]", GetErrorMessage(r));
			Thread::Sleep(CHECK_INTERVAL);
		}
	}
*/
	TryReturn(pAppManager->IsRunning(__appId), E_FAILURE, "SampleUiApp : The service is not working..");

	__pLocalMessagePort = MessagePortManager::RequestLocalMessagePort(LOCAL_MESSAGE_PORT_NAME);
	TryReturn(__pLocalMessagePort != null, E_FAILURE, "[E_FAILURE] Failed to get LocalMessagePort instance.");

	__pLocalMessagePort->AddMessagePortListener(*this);

	__pRemoteMessagePort = MessagePortManager::RequestRemoteMessagePort(appId, remotePortName);
	TryReturn(__pRemoteMessagePort != null, E_FAILURE, "[E_FAILURE] Failed to get LocalMessagePort instance.");


	AppLog("LocalMessagePort(\"%ls\") is ready !!!", __pLocalMessagePort->GetName().GetPointer());

	return E_SUCCESS;
}

result
MeecastServiceProxy::SendMessage(const IMap* pMessage)
{
	result r = E_SUCCESS;
	AppLog("SendMessage is called.");

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

void
MeecastServiceProxy::OnMessageReceivedN(RemoteMessagePort* pRemoteMessagePort, IMap* pMessage)
{
	AppLog("MeecastServiceProxy : A response message is Received.");

	String key(L"ServiceApp");
	String* pData = static_cast<String*>(pMessage->GetValue(key));

	App* pApp = App::GetInstance();
    AppLog("Response data %S", pData->GetPointer());
/*
	if (pData != null && pApp != null)
	{
		AppLog("SampleUiApp : Received data : %ls", pData->GetPointer());

		if (pData->CompareTo(L"ready") == 0)
		{
			pApp->SendUserEvent(STATE_CONNECTED, null);
		}
		else if (pData->CompareTo(L"started") == 0)
		{
			pApp->SendUserEvent(STATE_TIMER_STARTED, null);
		}
		else if (pData->CompareTo(L"timer expired") == 0)
		{
			pApp->SendUserEvent(STATE_TIMER_EXPIRED, null);
		}
		else if (pData->CompareTo(L"stopped") == 0)
		{
			pApp->SendUserEvent(STATE_TIMER_STOPPED, null);
		}
		else if (pData->CompareTo(L"exit") == 0)
		{
			pApp->SendUserEvent(STATE_EXIT, null);
		}
	}
*/
//	delete pMessage;
}
