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


//
// This file contains the Tizen application entry point.
//
#include "meecast.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//
// The entry function of Tizen application called by the operating system.
//
_EXPORT_ int
OspMain(int argc, char *pArgv[])
{
           AppLog("Application started.");
           ArrayList args(SingleObjectDeleter);
           args.Construct();
           for (int i = 0; i < argc; i++)
           {
                     args.Add(new (std::nothrow) String(pArgv[i]));
           }

           result r = Tizen::App::UiApp::Execute(meecastApp::CreateInstance, &args);
           TryLog(r == E_SUCCESS, "[%s] Application execution failed", GetErrorMessage(r));
           AppLog("Application finished.");

           return static_cast<int>(r);
}
#ifdef __cplusplus
}
#endif // __cplusplus
