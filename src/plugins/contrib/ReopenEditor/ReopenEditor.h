/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * Copyright: 2010 Jens Lody
 *
 */

#ifndef REOPEN_EDITOR_H_INCLUDED
#define REOPEN_EDITOR_H_INCLUDED

#include <cbplugin.h>
#include "ReopenEditorListView.h"


WX_DEFINE_ARRAY_PTR(cbProject *, ProjectPtrArray);

class ReopenEditor : public cbPlugin
{

    public:
        ReopenEditor();
        ~ReopenEditor();
        int GetConfigurationPriority() const
        {
            return 50;
        }
        int GetConfigurationGroup() const
        {
            return cgEditor;
        }
        cbConfigurationPanel * GetConfigurationPanel(wxWindow * parent);
        cbConfigurationPanel * GetProjectConfigurationPanel(wxWindow *  /*parent*/, cbProject * /*project*/)
        {
            return 0;
        }
        void BuildMenu(wxMenuBar * menuBar);
        void BuildModuleMenu(const ModuleType /*type*/, wxMenu * /*menu*/, const FileTreeData * /*data*/ = 0) {}
        bool BuildToolBar(wxToolBar * /*toolBar*/)
        {
            return false;
        }
        virtual void SetManaged(bool managed = true);
        virtual void ShowList();

    private:
        void OnAttach();
        void OnRelease(bool appShutDown);
        void OnReopenEditor(wxCommandEvent & event);
        void OnProjectClosed(CodeBlocksEvent & event);
        void OnProjectOpened(CodeBlocksEvent & event);
        void OnEditorClosed(CodeBlocksEvent & event);
        void OnEditorOpened(CodeBlocksEvent & event);
        void OnViewList(wxCommandEvent & event);
        void OnUpdateUI(wxUpdateUIEvent & event);
        void OnDoubleClick(wxListEvent & event);
        void DoReopenEditor(long item);
        ProjectPtrArray m_ClosedProjects;
        ReopenEditorListView * m_pListLog;

        bool m_IsManaged;
        /// We need this to live longer than the logger, because the InfoPane stores a pointer to
        /// this icon.
#if wxCHECK_VERSION(3, 1, 6)
        wxBitmapBundle m_LogIcon;
#else
        wxBitmap m_LogIcon;
#endif

        DECLARE_EVENT_TABLE()
};

#endif // REOPEN_EDITOR_H_INCLUDED
