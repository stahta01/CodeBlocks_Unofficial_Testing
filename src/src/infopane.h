/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef INFOPANE_H
#define INFOPANE_H


#include <limits>
#include <logmanager.h>

#include "cbauibook.h"

static const int infopane_flags = wxAUI_NB_MIDDLE_CLICK_CLOSE
                                  | wxAUI_NB_WINDOWLIST_BUTTON
                                  | wxAUI_NB_SCROLL_BUTTONS
                                  | wxAUI_NB_TAB_MOVE
                                  | wxAUI_NB_TAB_SPLIT;

class wxWindow;
class wxCommandEvent;
class wxMouseEvent;

class InfoPane : public cbAuiNotebook
{
        DECLARE_EVENT_TABLE()

        struct Page
        {
            Page() : icon(nullptr), window(nullptr), logger(nullptr), indexInNB(std::numeric_limits<int>::min()), eventID(0), islogger(0) {};
            wxString  title;
#if wxCHECK_VERSION(3, 1, 6)
            wxBitmapBundle * icon;
#else
            wxBitmap * icon;
#endif
            wxWindow * window;
            Logger  * logger;
            int       indexInNB; // used to be "visible" flag: invisible is <0, any other value means visible
            int       eventID;
            bool      islogger;
        };

        typedef int (*CompareFunction)(Page **, Page **);
        static int  CompareIndexes(Page ** p1, Page ** p2);
        void        ReorderTabs(CompareFunction cmp_f);

#if wxCHECK_VERSION(3, 1, 6)
        wxBitmapBundle m_DefaultBitmap;
#else
        wxBitmap m_DefaultBitmap;
#endif

        WX_DEFINE_ARRAY(Page *, wxArrayOfPage);
        wxArrayOfPage m_Pages;

        void Toggle(size_t index);
        void Hide(size_t i);

        void OnMenu(wxCommandEvent & event);
        void OnCopy(wxCommandEvent & event);
        void OnWrapMode(wxCommandEvent & event);
        void OnClear(wxCommandEvent & event);
        void ContextMenu(wxContextMenuEvent & event);
        void OnTabContextMenu(wxAuiNotebookEvent & event);
        void OnCloseClicked(wxAuiNotebookEvent & event);
        void OnTabPosition(wxCommandEvent & event);
        void DoShowContextMenu();
#if wxCHECK_VERSION(3, 1, 6)
        int  AddPagePrivate(wxWindow * p, const wxString & title, wxBitmapBundle * icon = nullptr);
        bool InsertPagePrivate(wxWindow * p, const wxString & title, wxBitmapBundle * icon = nullptr, int index = -1);
#else
        int  AddPagePrivate(wxWindow * p, const wxString & title, wxBitmap * icon = nullptr);
        bool InsertPagePrivate(wxWindow * p, const wxString & title, wxBitmap * icon = nullptr, int index = -1);
#endif
        using wxWindow::Show;
    public:

        InfoPane(wxWindow * parent);
        virtual ~InfoPane();

        /*
        *  Show a tab, or bring it to the foreground if already shown.
        *  Only use this method if crucial information is shown which the user must not miss, or if
        *  information is shown that requires input from the user (for example search results).
        *  In all other cases, it is the user's decision which tab he wants to look at, not yours.
        *  Note that you cannot hide a tab programatically.
        */
        void Show(size_t index);
        void Show(Logger * logger);
        void Hide(Logger * logger);
        void ShowNonLogger(wxWindow * p);
        void HideNonLogger(wxWindow * p);

        int GetPageIndexByWindow(wxWindow * win);
        int GetCurrentPage(bool & is_logger);

        Logger  * GetLogger(int index);
        wxWindow * GetWindow(int index);

        void UpdateEffectiveTabOrder(); // refreshes the tab effective order, needed, because tabs might have moved with drag and drop
        void LoadTabOrder(wxString layout);
        wxString SaveTabOrder();

        /*
        *  You should not need to call these functions under normal conditions. The application initialises
        *  everything at startup, after the main window has been created and the plugins have been loaded.
        *  AddLogger:
        *  Add the GUI control obtained by a logger's CreateControl() function.
        *  DeleteLogger:
        *  Remove (delete) a tab and its logger. All log output directed to the corresponding log index
        *  will be redirected to the null log thereafter.
        *  To prove that you are serious, you must know the logger belonging to the tab to delete.
        */
#if wxCHECK_VERSION(3, 1, 6)
        bool AddLogger(Logger * logger, wxWindow * p, const wxString & title, wxBitmapBundle * icon = nullptr);
#else
        bool AddLogger(Logger * logger, wxWindow * p, const wxString & title, wxBitmap * icon = nullptr);
#endif
        bool DeleteLogger(Logger * l);

        /*
        *  If something that is not a logger should be placed in the info pane (think twice about this before doing it),
        *  use AddNonLogger()/DeleteNonLogger() for that purpose.
        *  An example of something that is not a logger but might still show up in the info pane is the list of search results.
        */
#if wxCHECK_VERSION(3, 1, 6)
        bool AddNonLogger(wxWindow * p, const wxString & title, wxBitmapBundle * icon = nullptr);
#else
        bool AddNonLogger(wxWindow * p, const wxString & title, wxBitmap * icon = nullptr);
#endif
        bool RemoveNonLogger(wxWindow * p);
        bool DeleteNonLogger(wxWindow * p);
};


#endif
