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
/*******************************************************************************/

#include <FGraphics.h>
#include "maincontextmenu.h"

using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;

bool
ContextMenuMain::Initialize(void)
{
    Construct(FORM_STYLE_NORMAL);
    return true;
}

result
ContextMenuMain::OnInitializing(void)
{
    result r = E_SUCCESS;

    // Creates an instance of ContextMenu
//    __pContextMenu = new ContextMenu();
//    __pContextMenu->Construct(Point(400, 650), CONTEXT_MENU_STYLE_LIST);

//    __pContextMenu->AddItem(L"Item1", ID_CONTEXTMENU_ITEM1);
//    __pContextMenu->AddItem(L"Item2", ID_CONTEXTMENU_ITEM2);
//    __pContextMenu->AddActionEventListener(*this);

    // Creates an instance of Button to show the context menu
    Button* pButton = new Button();
    pButton->Construct(Rectangle(50, 50, 400, 100), L"Show ContextMenu");
    pButton->SetActionId(ID_BTN_SHOW_CONTEXTMENU);
    pButton->AddActionEventListener(*this);

    // Adds the button to the form
    AddControl(*pButton);

    return r;
}

// Sets the anchor position of the context menu
void
ContextMenuMain::ShowContextMenu(bool show)
{
    __pContextMenu->SetPosition(Point(300, 200));

    // Change to desired show state
    __pContextMenu->SetShowState(show);

    //Calls Show() of the control
    if (show)
    {
        __pContextMenu->Show();
    }
    else
    {
        Invalidate(true);
    }
}

result
ContextMenuMain::OnTerminating(void)
{
    result r = E_SUCCESS;

    // Deallocates the __pContextMenu
    delete __pContextMenu;

    return r;
}

// IActionEventListener implementation
void
ContextMenuMain::OnActionPerformed(const Control& source, int actionId)
{
    switch (actionId)
    {
    case ID_CONTEXTMENU_ITEM1:
        {
            // ....
        }
        break;
    case ID_CONTEXTMENU_ITEM2:
        {
            // ....
        }
        break;
    case ID_BTN_SHOW_CONTEXTMENU:
        {
            ShowContextMenu(true);
        }
        break;
    default:
        break;
    }
}
