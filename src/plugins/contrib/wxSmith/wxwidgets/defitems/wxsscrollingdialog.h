/*
* This file is part of wxSmith plugin for Code::Blocks Studio
* Copyright (C) 2006-2007  Bartlomiej Swiecki
*
* wxSmith is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* wxSmith is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith. If not, see <http://www.gnu.org/licenses/>.
*
* $Revision: 12297 $
* $Id: wxsscrollingdialog.h 12297 2021-03-06 15:47:05Z fuscated $
* $HeadURL: https://svn.code.sf.net/p/codeblocks/code/trunk/src/plugins/contrib/wxSmith/wxwidgets/defitems/wxsscrollingdialog.h $
*/

#ifndef WXSSCROLLINGDIALOG_H
#define WXSSCROLLINGDIALOG_H

#include "../wxscontainer.h"

class wxsScrollingDialog : public wxsContainer
{
    public:

        wxsScrollingDialog(wxsItemResData * Data);

    private:

        virtual long OnGetPropertiesFlags()
        {
            return wxsContainer::OnGetPropertiesFlags() | flTopLevel;
        }
        virtual wxObject * OnBuildPreview(wxWindow * Parent, long Flags);
        virtual void OnBuildCreatingCode();
        virtual void OnEnumContainerProperties(long Flags);

        wxString Title;
        bool Centered;
};

#endif
