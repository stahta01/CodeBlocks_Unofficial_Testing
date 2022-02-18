// ----------------------------------------------------------------------------
/*
	This file is part of CodeCompletion, a plugin for Code::Blocks
	Copyright (C) 2007 Pecan Heber

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
// ----------------------------------------------------------------------------

#ifndef VERSION_H
#define VERSION_H
// ---------------------------------------------------------------------------
// Logging / debugging
// ---------------------------------------------------------------------------
//debugging control
#include <wx/log.h>
#include <wx/string.h>

#define LOGIT wxLogDebug
#if defined(LOGGING)
#define LOGGING 1
#undef LOGIT
#define LOGIT wxLogMessage
#define TRAP asm("int3")
#endif

//-----Release-Feature-Fix------------------
#define VERSION wxT("0.2.9913 2022/02/02")
//------------------------------------------
// Release - Current development identifier
// Feature - User interface level
// Fix     - bug fix or non UI breaking addition
// ----------------------------------------------------------------------------
class AppVersion
// ----------------------------------------------------------------------------
{
public:
    AppVersion()
    {
        m_version = VERSION;
    }
    ~AppVersion() {};

    wxString GetVersion()
    {
        return m_version;
    }

    wxString m_version;
    wxString m_AppName;
protected:
private:
};

#endif // VERSION_H
// ----------------------------------------------------------------------------
// Modifications
// ----------------------------------------------------------------------------
//0.2.99     2022/01/26 ac
//          Major refactor of code arround the clang/clangd/clang.exe/clangd.exe usage
//          Fix bugs with auto detection and refactor some areas to simplify the code
//          Allow spaces in paths in clangx executables
//          Refactor clangx version checking to move it into ClangLocator.h
//          Update clangd_client_wx31_64.cbp to have two targers, one for the dll & zip and the other for the zip & dll & cbplugin. The
//              default is the target for the dll & zip only.
//          Modified the clsettings.xrc to split the clang.exe and clangd.exe to have their own entry.
//          Modified the clsettings.xrc to add a auto detect button to detect both clang.exe and clangd.exe.
//0.2.13    2022/02/2 ph/ac
//          Add CB fix trunk rev 12689 ticket 1152
//0.2.12
//          2022/01/31_14 ph
//          Removed event.Skip() from OnReparseSelectedProject() seems to have stopped
//              unexpected exit after changing clangd.exe path, then moving mouse to logs.
//          Quote clangd command strings if needed.ProcessLanguageClient ctor.
//          Fix disappearing functions in GotoPrev/NextFunction() caused by allowing
//              callback data to be used in default event code.
//          Add messageBox that project must be reloaded/reparsed after (re)setting the clangd
//              location in settings.
//          Fix loading incorrect bitmap for Settings/configuration. Should have been "codecompletion"
//              not "clangd_client" for ConfigManager. It's a type, not a plugin.
//          Retested auto detection with no LLVM path set.
//          2022/01/26 ph
//          Show clangd_client MessageBoxes in front of Plugin manager dlg.
//0.2.11    2022/01/26 ph
//          Add code to check for libcodecompletion.so existence in IsOldCC_Enabled()
//          clangd_client_wx30-unix.cbp for image zipping to:
//              <Add after="cd src/resources && zip -rq9 ../../clangd_client.zip images && cd ../.." />
//              Thanks Andrew.
//          Remove usage/dependency on ccmanager's Settings/Editor/Code Completion checkbox.
//          Beef up verification that old CodeCompletion plugin is loaded/missing/enabled/disabled
//              code in ctor and OnAttach() function. This solves the "enable clangd_client" crash
//              when old CodeCompletion is already enabled or enabled after clangd_client is enabled.
//              Particles cannot possess the same space at the same time. And, it seems, that's also true
//              of information.
//0.2.10
//          2022/01/24 ph
//          Dont output wxUniChar in wxString::Format causing assert in DoValidateUTF8data()
//              when internationalization is enabled.
//          2022/01/22 ph
//          Assure image folder is in first level of clangd_client.zip file
//          Remove some wxSafeShowMessage() that were used for debugging
//          Add more info to 'invalid utf8' msg (thanks ollydbg)
//          Implemented some ollydbg suggestions https://forums.codeblocks.org/index.php/topic,24357.0.html
//          Clean out preamble-*.{tmp|pch} left in temp directory
//          Remove LSP:Hover results debugging msgs
//          ProcessLanguageClient dtor: allow time for pipe/ReadJson threads to terminate
//          Linux lsof: code to avoid dumb "No such file in directory" msg
//          Locks: Record current owner of lock for use in error msgs
//0.2.09
//          2022/01/22 ac
//          Update project files to comment building clangd_client.cbplugin.
//          Update clangd_client_wx31_64.cbp from clangd_client_wx31_64.cbp changes.
//          2022/01/15 ph
//          Removed accidental use of older CodeCompletion image files.
//          Moved image files in clangd_client.zip to the first level.
//          Reworked the repo upload files to accomodate the above two changes
//          Remove .cbPlugin file. It borked my system three time. Your out!
//0.2.08
//          2022/01/14 ph
//          Code to removed invalid utf8 chars from clangd responses responses
//          cf:client.cpp DoValidateUTF8data()
//0.2.07
//          2022/01/13 ph
//          ReadJson() clear illegal utf8 and show in debug log
//          Change project title of clangd_client-uw to clangd_client-wx31_64
//          Add MakeRepoUpload.bat to preserve clangd_client.zip and to avoid stripping the dll
//0.2.06
//          2022/01/13 AC
//          Doc typo fixed
//          Plugin zip file is deleted on before extra command in the project file to stop recursive zip file creation..
//0.2.05
//          2022/01/12
//          Updated the way the plugin was built
//          Major doc update
//0.2.04
//          2022/01/10
//          Removed old codecompletion xrc files.
//          Fix compilation when -DCB_PRECOMP is specified (needd later when part of C::B truck)
//          Sync version between this file and the plugin manifext.xml file.
//          Remove old codecompletion XRC files as per ticket 13. Also renamed the project files.
//          Sync version in manifest.xml and version.h (ticket 18)
//          Change plugin name back to show as Clangd_Client (camel case) in the plugin manager (ticket 16)
//          Add support for building "third" party plug to be installed via plugin manager (ticket 20)
//          Change clsettings.xrc to change the Clangd executable wxStaticBoxSizer text from "clangd's installation location" to "Specify clangd executable to use" (ticket 11)
//0.2.03
//          2022/01/7
//          Always return the length of a LSP record in the incoming buffer.
//              else it gets stuck there forever.
//          Fix bad length in headers because of LSP invalid utf8 chars
//          Fix json assets because of LSP invalid utf8 chars
//          2022/01/3
//          Fix bad symbol rename in Parser::OnLSP_RenameResponse( )
//          Fix bad string_ref/wxString translation in LSP_RequestRename()
//          Switch from using wxString on incoming clangd data to std::string
//          2021/12/22
//          Modifications to run on Linux
//          2021/12/14
//          Contains untested unix support
//0.0.20
//          2021/11/19
//          Moved clangd-cache lock into the .cache directory
//          2021/11/18
//          Removed hard coded filename for clangd resources folder in CodeCompletion.cpp
//          Show msg when Clangd_Client was unable to auto detect clangd installation.
//          2021/11/15
//          Fixed ReparseCurrentProject using wrong pProject
//          Moved parsing toggle (on/off) from main menu to context menu
//          2021/11/13
//          Implemented PauseParsingForReason() to freeze and un-freeze parsing.
//          Moved UI parsing toogle to ParseParsingForReason()
//          2021/11/12
//          SetInternalParsingPaused() set at ShutdownClient and UpdateClassBrowser view.
//          Fixed to set only once in LSP_ParseDocumentSymbols()
//          2021/11/11
//          Fixed choosing wrong parser/project combination in OnLSP_Event();
//          2021/11/9
//          To ccoptionsdlg.cpp/settings.xrc, added checkBox items for logging client and server logs.
//          Set unused check boxes in ccoptionsdlg.cpp to hiddent. What to do with them?
//              Does Clangd have an equivalent to these (local,global,preprocessor,macros) choices?
//          2021/11/7
//          To ccoptionsdlg.cpp/settings.xrc, added LLVM txtMasterPath, btnMasterPath, btnAutoDetect config fields.
//          2021/11/6
//          For OnLSP_ReferencesResponse() remember the log filebase and use for
//              subsequent "textDocument/definition" and "textDocument/definition"
//              responses.
//          2021/11/3
//          Remove dead code, remove double STX's in LSP headers
//          2021/11/2
//          change LSPEventSinkHandler to use a sequence integer rather than just the
//              LSP textDocument/xxx header request hdr. Send integer to clangd and use it
//              on return to execute the appropriate callback.
//              This solves some miss executions of queued requests and stolen responses
//              because multiple textDocucmet/xxx hdrs of the same type are sent at the same time.
//          2021/11/1
//          Moved all functions responding to requested clangd data to Parser class.
//          Removed some debugging msgs from ParseFunctionsAndFillToolbar()
//          2021/10/31
//          Moved OnLSP_CompletionResponse() to Parser.
//          Call LSP_ParseDocumentSymbols() directly rather than using a command event
//          2021/10/30
//          Fixed LSP_ParseDocumentSymbols() DoAddToken() miss-queing tokens esp. after RemoveFile()
//              Was not updating m_LastParent properly.
//          2021/10/15
//          Cone and delete IdleCallBack queue entry before executing the call
//          Fixed erroneous "editor is being parsed" caused by LSP_AddToServerFilesParsing()
//              in LSP_DidChange(). There's no response to clear the entry.
//          2021/10/14
//          Deprecated IsFileParsed() to stop huge amount of mutex locking.
//              Replaced it with Parser query of std::set of filenames having been parsed and
//              having received their clangd diagnostics response.
//              Except for ~ParserBase() dtor there are no mutex blocks in the UI thread.
//              If the lockTimeout fails, the function is rescheduled on the idle queue
//               or simply returns (if code allows).
//          2021/10/11-2
//          If main ccmanager's Code completion box is unchecked, go silent.
//          2021/10/11
//          Reinstated single line updates to clangd v13
//          Fixed GetTokenInFile() to search headers and implementations
//          2021/10/10
//          GenerateHTML() Don't block UI locking TokenTree
//          2021/10/9
//          RemoveLastFunction. Testing deprecation. Just return.
//          FindTokensInFile(); Require call to own TokenTree lock.
//          Enable/Test GenerateHTML.
//          Enable/Test Insert/Refactor All class methods w/o implementation.
//          Enable/test Add doxygen when implementing fuction method
//          Enable/test Documentation popup / GetDocumentation() / GetTokenInFile()
//              called in OnLSP_CompletionResponse() to get tknIndex.
//          2021/10/4
//          Fix CB crash/hang when CB shutdown with shown/floating windows (main::OnApplicationClose)
//          Hang issueing 'reparse project'. Fix: use find/remove event hander, not popEventHandler
//          Re-instated code for dialog ccdebug info (key: Alt-Shift double click tree item)
//          2021/09/28
//          Added parser.h/cpp m_nternalParsingPaused to allow code to pause file parsing //(ph 2021/07/31)
//              Especially where the code invokes a dialog that holds the TokenTree mutex lock. This
//              can cause the clangd jason responses to backup up and eat memory. Cf: OnLSP_ParseDocumentSymbols()
//          2021/09/27
//          Implement IdleCallbackHandler to avoid mutex blocking main UI thread
//              using wxMutex.lockTimeOut(). If fails, queue an idle time callback
//              to the function instead of blocking.
//          Fix crash because IdleCallbacks weren't cleared during project close;
//          2021/09/21
//          Eliminated the mutex s_ParserMutex. Was only being used by ParserThreadedTask to guard
//              m_BatchParseFiles. ParserThreadedTask is now deprecated and m_BatchParseFiles is only
//              used by Parser, each of which has its own m_BatchParseFiles.
//          2021/09/18
//          Don't update ClassBrowserView (UI symbols tree) if mouse is in the Symbls tab window.
//              User could be using it and updating it causes users to lose their selections.
//          Update ClassBrowserView only every 4 "DocumentSymbols" files processed.
//          2021/09/15
//          if Batch files queued not empty, set Batch timer to sleep to 300ms instead of 1000sec in order
//              to check finished parsering and possibly start another clangd parser.
//          2021/09/11
//          Updating symbols tree done in OnIdle() time only via Parser::OnLSP_ParseDocumentSymbols()
//          CodeCompletion::OnToolbarTimer(wxCommandEvent& event) allows CC Toolbar (class and function) to be initiated from OnLSP_ParseDocumentSymbols()
//          For GotoPrevNextFunction() revert to original CC code. Much faster than a LSP request/response cycle
//          2021/09/10
//          Fixed crash in Parser::Done(). wasn't checking ptrs before usage.
//          client.cpp: Reverted to allowing clangd to using half of processes, else
//              all hardware threads used for parsing and none left for on-the-fly requests such as completions, goto's etc.
//              Use the pid in CBCCLogger-<pid> to avoid debugged process conflicts over name.
//          2021/09/9
//          Fixed loop when class missing ending '}' in DoHandleClass()
//          Added Entry guard to ClassBrowserBuilderThread::Init() to stop dead lock with ExpandItems()
//          2021/09/15
//          if Batch files queued not empty, set Batch timer to sleep to 300ms instead of 1000sec in order
//              to check finished parsering and possibly start another clangd parser.
//          2021/09/11
//          Updating symbols tree done in OnIdle() time only via Parser::OnLSP_ParseDocumentSymbols()
//          CodeCompletion::OnToolbarTimer(wxCommandEvent& event) allows CC Toolbar (class and function) to be initiated from OnLSP_ParseDocumentSymbols()
//          For GotoPrevNextFunction() revert to original CC code. Much faster than a LSP request/response cycle
//          2021/09/10
//          Fixed crash in Parser::Done(). wasn't checking ptrs before usage.
//          client.cpp: Reverted to allowing clangd to using half of processes, else
//              all hardware threads used for parsing and none left for on-the-fly requests such as completions, goto's etc.
//              Use the pid in CBCCLogger-<pid> to avoid debugged process conflicts over name.
//          2021/09/9
//          Fixed loop when class missing ending '}' in DoHandleClass()
//          Added Entry guard to ClassBrowserBuilderThread::Init() to stop dead lock with ExpandItems()
//          2021/08/30
//          Changed cclogger to write log to \temp\CCLogger-<#>.log
//          changed cclogger to trace locks/unlocks using trylock(250). Trying to catch hang.
//          changed cclogger to assert locks/unlocks using trylock(250). Trying to catch hang.
//          Changed cclogger to send DebugLogError events
//          Fixed ParseManager::DeleteParser()  bad parser address by re-instating DeleteParser() in OnProjectClosed() 2021/09/2
//          Added m_CCDelay (config slider) support code to use as auto-code-completion delay. 2021/09/2
//          2021/08/25
//          Fixed Done() to check when active editor does not belong to active project.
//          2021/08/14
//          Added LSP_SignatureHelp() for menu item "Edit/Show calltip"
//          2021/08/11
//          Fixed LSP_hover, modified CC::GetTokenAt() and CC::GetCallTips() 2021/08/11
//          2021/08/10
//          LSPDiagnosticsResultsLog::SyncEditor() verify file actually exists 2021/08/10
//          OnLSP_DiagnosticsResponse() append filename to "LSP diagnostics:" header line. 2021/08/10
//          Deprecated parserthreadedtask.h/cpp (no longer used) 2021/08/10
//          2021/08/7
//          Move gathering files to parse from CodeCompletion::OnLSP_Event() initialization to ParseManager::DoFullParsing()  2021/08/7
//          2021/07/17
//          Get parallel parse threads from configuration. Remove unused config items.  2021/07/17
//          client.cpp Set hasChangedLineCount = true to avoid crashes when modifying single line 2021/07/26
//              It causes client to alway send the whole text of a file. Needs to be fixed
//          Removed showerr parameter from GetLSPclient(pProject|peditor). Just too confusing.   2021/07/27
//          Updated Goto[Decl|Impl] to verify file belongs to active project    2021/07/27
//          Deprecated/Removed parserthread.h/cpp 2021/07/27
//          Added parser.h/cpp m_UserParsingPaused to allow user to manually pause file parsing //(ph 2021/07/31)
//0.0.20(h) 2021/07/16
//          Support CanDetach() missing call from sdk\pluginsconfigurationdlg.cpp
//          Use CanDetach() to ask user to close workspace before uninstalling clangd_client
//          Reduce number of clangd parsing cores to 2. 3 causes 50%-90% cpu load on 4core 8thread cpu.
//          Fixed Annoying dialog to record users reponse correctly
//0.0.20(h) 2021/06/29
//          Added "LSP clangd", "LSP Clangd is not active for this project." to codecompletion::GetLSPclient(cbProject*)
// 0.0.19(h) 2021/06/14    moved to wx3.1.5
//          Fixed Re-parse now Symbols context event by invoking idParseCurrentProject event        //(ph 2021/06/14)
//          Added "Editor not yet parsed" and "...parsing delay" msg to find refs, goto decl/impls funcs. //(ph 2021/06/14)
//          changed wxCheck_msg2 to cbAssertNonFatal. wxCheck did not work in release mode. //(ph 2021/06/15)
//          For FindReferences and GotoDeclImpl, Output InfoWindow::Display() for files not contained in a project. //(ph 2021/06/15)
//          Turned on #define CC_ENABLE_LOCKER_ASSERT   2021/06/16 // NOTE (ph#): Turn off CC_ENABLE_LOCKER_ASSERT for production ?
//          Added IsServerFilesParsing() to client.h 2021/06/16
// 0.0.18
//          Dont request symbols when only a document completion request 2021/04/6
//          Fixed crash re: double gotoFunction dialogs
//          Implemented background parse of unopened project files
//          Fixed: Stopped EncodingDector conversion msgs when file is loaded into hidden editors 2021/04/13
//          Modified CC GotoFunction dialog to use TokenTree when using LSP client 2021/04/13
//          Focus LSP messages log on didSave   2021/04/14
//          Fixed: Dont call for symbols on dumb diagnostics sent on a didclose //(ph 2021/04/15
//          Dont parse system headers, clangd already does that.    2021/04/16
//          On reparse project, call CC parse first in so it's ptr is available for LSP reparseing 2021/04/15
//          Fixed: invalid utf8 text handed to clangd in didOpen for background 2021/04/16
//          Sort background files to be parsed in last modified order   2021/04/16
//          Switch to LSP messages log only when user uses Menu or shortcut keys to save file 2021/04/16
//          For background parsing sort by Modification time to process recently used files first.  2021/04/19
//          Fixed: crash after switching lsp-to-CC-to-LSP then reparsing project. Event handers are removed.
//              Invalidated event.Skip() as both LSP and CC had been restarted before return and Skip(); 2021/04/17
//          Added pProject as token.m_UserData so that cmd titles are highlighted when parsing is finished. 2021/04/22
//          Track background parcing in m_LSP_CurrBackgroundFilesParsing array 2021/04/23
//          Changed m_LSP_CurrBackgroundFilesParsing to std::map containing filename and parse starting time. 2021/04/24
//          Write parse duration time for background parsed files to  DebugLog  2021/04/24
//          added #ifdef for SHOW_SERVER_CONSOLE in ProcessLanguageClient::ProcessLanguageClient()  2021/04/24
//              Showing the server console steals focus from CodeBlocks.
//          Dont ask for .h symbols in diagnostics response. It clobbers token tree .cpp tokens with wrong items. 2021/04/24
//          Update ClassBrowser every three completed background parsing finishes (OnLSP_ParseDocumentSymbols) 2021/04/27
//          Paused background parsing while debugger is running 2021/04/28
//          Parallel parse max of half of cpus or 3 2021/04/28
//          Always put out a message to the LSP messages log even if zero diagnostics   2021/04/28
//          Replace all log calls containing ...log(F(... with ...log(wxString::Format("",) because 2021/05/4
//                the F() call is not thread safe and causes crashes.
//          Move ClassBrowser updates to OnLSP_ParseDocumentSymbols() to avoid OnParserEnd() lock hangs. 2021/05/4
//          Write "ended parsing" msg for each file to regular log 2021/05/7
//          Verify pParser for pProject exists before creating client
//          Switch from DoLockClangd_CacheAccess() to OnDebuggerStarting() to shutdown clangd when debugging clangd. 2021/05/11
//          Stopped didOpen() of .h files during LSP initialization. But it causes goto defs/decls from .h files to fail. 2021/05/11
//          ^^^ Reverted: symbols are not requested in OnLSP_DiagnosticsResponse() for .h files. But .h files needs didOpen() for clangd. //(ph 2021/05/12)
//          Implemented Reparse single file for LSP clangd //(ph 2021/05/13)
//          Fixed GoToFunction() for both headers and implementations. 2021/05/22
//          ParseManager::DoFullParsing() Parse headers only if 'ShowInheritance" is set //(ph 2021/05/24)
//              Note: cfg setting and symbols context setting is out of sync.
//          Removed all WriteOptions() except for OnApply(). They were causing previous parsers options
//              to be applied to the next created parser after a parser delete. //(ph 2021/05/25)
//              and Modifying the config while a project was loaded was impossible. Config items wouldn't stick.
//          Removed Parser->RemoveFile() before parsing json data. Was causing crashes. 2021/06/4
//              Still don't understand how stale data is removed from Token tree.
//          Parsing headers in both foreground and background when "Show inheritance members" 2021/06/11
//          Fixed: parsing children in LSP_symbolsparser.cpp //(ph 2021/06/11)
//          Fixed some crashes because not handling typedef and class forward declaration correctly. 2021/06/12
// 0.0.17   2021/04/2
//          Support for TokenTree (symbols browser & toolbar)
//  0.0.16  2021/03/13
//          Output msg when .json or .cache is out of sync with move/deleted files. 2021/03/6
//          Delete the clangd_cache.lock file when project closed.
//          Experimental textDocument/symbol code in Parser();
//          Fixed missing GoToNextFunction() LSP routine 2021/03/17
//          Added LSP calls for textDocument/semanticTokens/full
//          Added wxMutex guards for the pipe input buffer  2021/03/18
//  0.0.15  2021/03/6
//          Add callback to OnProjectFileAdded() to delay LSP_DidOpen() until after the
//              ProjectFile* is added to the editor and the project. 2021/03/5
//          Fix OnLSP_ReferencesResponse() bad calculation of relative file names. 2021/03/6
//              Not correct fix. The .json CDB was out of sync bec. of moved files/
//  0.0.15  2021/03/4
//          LSP_didChange() - if last line of text, send full editor text. We dont know
//              how that last line used to look. So we cant calculate line and col accurately.
//          Optimize OnLSP_DiagnosticsResponse() to show 'ignored' diagnostics count.
//          Persistent user ignored diagnostics to config
//          Detect clang and clangd installation.   2021/03/3
//          Set query-driver agrument from master toolchain 2021/03/4
//          Verify clang installation and clangd version >= 12.0.0 2021/03/4
//  0.0.14  2021/02/27
//          Implemented "Reparse active project" for clangd. Also used to kill/recreate
//              the client/server to force re-reading of compile_commands.json.
//          CleanUpLSPLogs() to remove closed logs locked by windows.
//          Use clangd12, remove envPath mangling, add log "Ignore Messages" context item
//          For clangd-12 add " --resource-dir=<clang-12>" and " --query-driver=f:\\usr\\MinGW810_64seh\\**\\g*"
//          Disable clangd plugin if CodeCompletion is loaded and enabled.
//          Allow user to restart clangd client/server on unusual termination
//          Modded didChange() call to send full editor text only when line count has changed.
//              otherwise just send the single line changed.
//  0.0.13  2021/02/9
//          check for stc->active AutoComp/CallTip windows before switching log focus
//          Added wxWakeUpIdle when setting m_PendindingCompletionRequest
//          Fixed missed completion buffer causing permanent CompletionIsBusy condition
//          Implement lockFile for .Clangd-cache directory
//          Rework Get/SetCompletionTimeBusy() to count down from a set busy time
//              so it can't block forever.
//          Changed to Clangd, it's faster and less buggier
//          Start calling didClose/didOpen instead of didSave because of bug #320 which
//              causes completions to stop after a didSave.
//              https://github.com/clangd/clangd/issues/320
//  0.0.12  2021/01/31
//          Fix OnEditorActivated file-reopen asking for file instead of project initialized
//          Switch to LSP log only when last LSP request was textDocument/didSave
//          Dwell time for keyboard activity before invoking LSP_complete()
//  0.0.11  2021/01/28
//          Add wxCHECK2_MSG() for editor param in calls to LSP_ functions
//          Add wxCHECK2_MSG() to catch non response to LSP_FindReferences
//          Fix LSP log focus to not screw up code completion popup window
//          Make sure ALL json data is in the input buffer before using
//          Add "Editors file is not yet parsed." message boxes
//  0.0.10  2021/01/27
//          Removed modifying .Clangd files. None of them worked.
//          Added () and (arguments) after completion function
//  0.0.9   2021/01/23
//          Remove Manager::Yield()'s for project close
//          try/catch the hell out of OnLSP_DiagnosticsResponse()
//  0.0.8   2021/01/22
//          Optimize LSP_Shutdown() to avoid long project close time
//          Optimize reading Clangd for input reads and missed buffers
//          Create a separate client/server for each file/project to circumvent
//              Clangd's inablility to handle files outside the root_dir.
//  0.0.7   2021/01/18
//          Implemented Clangd extension request $Clangd/info
//          Decided not to used $Clangd/info as status for IsParsed(editor)
//          Add tuple status mechanism to track status of LSP IsParsed(editor)
//              last LSP request and editor position of the request.
//  0.0.6   2021/01/16
//          OnLSP_references: specific requests for declaration and definition
//          On LSP requests, require editor and editor caret position
//          Clear LSP messages log on LSP_didSave()
//          Preserve last editor*, LSPtype, and caret position for last LSP request
//          Focus LSP log to the last diagnostics header entry
//  0.0.5   2021/01/16
//          Fixed compiler_commands filename and dir relationship
//          One-Clangd-per-project implementation
//          LSP messages log implementation
//  0.0.04  2021/01/9
//          Optimized UpdateCompileCommands.json file to only update when file is changed.
//          Fixed LSP diagnostics log to not interfere with Search results log
