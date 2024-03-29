/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef LOGMGR_H
#define LOGMGR_H

#include "manager.h"
#include "logger.h"
#include <map>

//namespace cb
//{
/**
 * @brief sprintf-like function
 * @warning NOT thread safe within a single compilation unit due to use of
 *          static @c temp_string.
 */
inline wxString F(const wxChar * msg, ...)
{
    va_list arg_list;
    va_start(arg_list, msg);
#if wxUSE_UNICODE
    // in wx >=  3.0 unicode-build (default) we need the %ls here, or the strings get
    // cut after the first character
    ::temp_string = msg;
    ::temp_string.Replace(_T("%s"), _T("%ls"));
    msg = ::temp_string.wx_str();
#endif
    ::temp_string = wxString::FormatV(msg, arg_list);
    va_end(arg_list);
    return ::temp_string;
}

inline wxString F(const wxString & msg, ...)
{
    va_list arg_list;
    va_start(arg_list, msg);
#if wxUSE_UNICODE
    // in wx >=  2.9 unicode-build (default) we need the %ls here, or the strings get
    // cut after the first character
    ::temp_string = msg;
    ::temp_string.Replace(_T("%s"), _T("%ls"));
    ::temp_string = wxString::FormatV(::temp_string, arg_list);
#else
    ::temp_string = wxString::FormatV(msg, arg_list);
#endif
    va_end(arg_list);
    return ::temp_string;
}
//} // namespace cb


struct DLLIMPORT LogSlot
{
    friend class LogManager;

    Logger  * log;
    size_t    index;
#if wxCHECK_VERSION(3, 1, 6)
    wxBitmapBundle * icon;
#else
    wxBitmap * icon;
#endif
    wxString  title;

    LogSlot();
    ~LogSlot();

    size_t Index() const;

    void    SetLogger(Logger * in);
    Logger * GetLogger() const;
};


class DLLIMPORT LogManager : public Mgr<LogManager>
{
    public:
        struct InstantiatorBase
        {
            virtual Logger * New()
            {
                return nullptr;
            };
            virtual bool RequiresFilename() const
            {
                return false;
            };
            virtual ~InstantiatorBase()           { ; };
        };
        template<typename type, bool requires_filename = false> struct Instantiator : public InstantiatorBase
        {
            Logger * New() override
            {
                return new type;
            };
            bool RequiresFilename() const override
            {
                return requires_filename;
            };
        };

        enum { max_logs = 32 };
    private:
        typedef std::map<wxString, InstantiatorBase *> inst_map_t;
        inst_map_t instMap;

        LogSlot slot[max_logs + 1];

        LogManager();
        ~LogManager() override;


        friend class Mgr<LogManager>;
        friend class Manager;

        void ClearLogInternal(int i);
        void LogInternal(const wxString & msg, int i, Logger::level lv);

    public:
        enum { no_index = -1, invalid_log, stdout_log, app_log, debug_log};

        /* ------------------------------------------------------------------------------------------------------
         * Management functions
         * ------------------------------------------------------------------------------------------------------
         * Unless you are writing your own loggers, you will probably not need any of these.
         * SetLog() transfers ownership of the Logger object to the LogManager. Loggers must be heap allocated.
         * On error, SetLog() returns invalid_log
         */
        size_t SetLog(Logger * l, int index = no_index);
        void DeleteLog(int i);
        LogSlot & Slot(int i);
        size_t FindIndex(Logger * l);

        /* ------------------------------------------------------------------------------------------------------
         * Logging functions
         * ------------------------------------------------------------------------------------------------------
         * This section is what most people will be interested in.
         *   - Log(), LogWarning(), and LogError() output "info", "warning", or "error" messages to a log.
         *     Log() is almost certainly the function that you want to use, if you don't know what to pick.
         *     By default, logging is directed to the application log.
         *   - DebugLog() and DebugLogError() direct their output into the debug log.
         *   - LogToStdOut() outputs a message on stdout. Normally you will not want to use this function, it
         *     exists solely for some special cases.
         *   - Panic() signals a condition that does not allow proper continuation of the application or significant
         *     parts of it, but it is slightly less harsh than simply bailing out with an unhandled exception.
         *     Currently, Panic() is simply a wrapper around wxSafeShowMessage(), but it might do something else, too.
         *     When signalling panic, you will usually want to shut down the application as soon as appropriate.
         *     Plugins should call Panic() with the plugin's name as the component argument.
         */

        void Log(const wxString & msg, int i = app_log, Logger::level lv = Logger::info)
        {
            LogInternal(msg, i, lv);
        };
        void LogWarning(const wxString & msg, int i = app_log)
        {
            Log(msg, i, Logger::warning);
        };
        void LogError(const wxString & msg, int i = app_log)
        {
            Log(msg, i, Logger::error);
        };

        void Panic(const wxString & msg, const wxString & component = wxEmptyString);

        void DebugLog(const wxString & msg, Logger::level lv = Logger::info)
        {
            Log(msg, debug_log, lv);
        };
        void DebugLogError(const wxString & msg)
        {
            DebugLog(msg, Logger::error);
        };

        void LogToStdOut(const wxString & msg, Logger::level lv = Logger::info)
        {
            Log(msg, stdout_log, lv);
        };

        void ClearLog(int i)
        {
            ClearLogInternal(i);
        };

        /* ------------------------------------------------------------------------------------------------------
         * Logger registry and RTTI
         * ------------------------------------------------------------------------------------------------------
         * These functions allow to obtain a list of names for all generic Loggers that are presently available
         * and to create a new Logger by name without knowing the type at compile time.
         *
         *   logptr = LogManager::Get()->New(_T("stdout"));  // does exactly the same as
         *   logptr = new StdoutLogger();
         *
         * You normally do not need to worry about creating Loggers. Only ever consider using these functions if you
         * really have to (want to) change the global behaviour of Code::Blocks for some reason.
         */
        wxArrayString ListAvailable();
        Logger * New(const wxString & name);
        bool FilenameRequired(const wxString & name);
        /*
         * Add a logger to the registry of "available Logger types". Unless you are adding a general Logger which should
         * be accessible by name, you will not need this function. Having said that, you probably NEVER need this function.
         */
        void Register(const wxString & name, InstantiatorBase * ins);

        /* ------------------------------------------------------------------------------------------------------
         *  Unless your name is "main.cpp" by any chance, you don't ever need to call this.
         *  If you use it, and your name is not "main.cpp", then you better come up with a good excuse, if someone asks.
         * ------------------------------------------------------------------------------------------------------
         */
        void NotifyUpdate();
};

#endif

