// System include files
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/textfile.h>
#include <wx/filefn.h>

// CB include files
#include <sdk.h> // Code::Blocks SDK
#include "cbworkspace.h"
//#include "compilerfactory.h"
#include "editormanager.h"
#include "filemanager.h"
#include "logmanager.h"
#include "manager.h"
#include "macrosmanager.h"
#include "uservarmanager.h"
//#include "tinyxml.h"

// ProjectExporter include files
#include "CMakeListsExporter.h"

#define CMAKE_MIN_VERSION_REQUIRED "3.24.0"

CMakeListsExporter::CMakeListsExporter()
{
    // Does not support C::B ${variable}. Supports $(variable)
    m_RE_Unix.Compile("([^$]|^)(\\$[(]?(#?[A-Za-z_0-9.]+)[\\) /\\\\]?)", wxRE_EXTENDED | wxRE_NEWLINE);
    // MSDOS %VARIABLE%
    m_RE_DOS.Compile("([^%]|^)(%(#?[A-Za-z_0-9.]+)%)", wxRE_EXTENDED | wxRE_NEWLINE);
}

CMakeListsExporter::~CMakeListsExporter()
{
}

void CMakeListsExporter::ExpandMacros(wxString & buffer)
{
    if (buffer.IsEmpty())
    {
        return;
    }

    if (buffer.IsSameAs("\"\"") || buffer.IsSameAs("''"))
    {
        buffer.Clear();
        return;
    }

    static const wxString delim("$%[");

    if (buffer.find_first_of(delim) != wxString::npos)
    {
        // See macromanager.cpp MacrosManager::ReplaceMacros function for where this code was taken from
        static const wxString const_COIN("COIN");
        static const wxString const_RANDOM("RANDOM");
        static const wxString toNativePath("$TO_NATIVE_PATH{");
        static const wxString toUnixPath("$TO_UNIX_PATH{");
        static const wxString toWindowsPath("$TO_WINDOWS_PATH{");
        wxString search;
        wxString replace;
        UserVariableManager * pUsrVarMan = Manager::Get()->GetUserVariableManager();
        MacrosManager * macroMan = Manager::Get()->GetMacrosManager();
        const MacrosMap & Macros = macroMan->GetMacros();

        while (m_RE_Unix.Matches(buffer))
        {
            replace.Empty();
            search = m_RE_Unix.GetMatch(buffer, 2);
            wxString var = m_RE_Unix.GetMatch(buffer, 3).Upper();

            if (var.GetChar(0) == '#')
            {
                replace = UnixFilename(pUsrVarMan->Replace(var), wxPATH_UNIX);
            }
            else
            {
                if (var.compare(const_COIN) == 0)
                {
                    replace.assign(1u, (rand() & 1) ? '1' : '0');
                }
                else
                    if (var.compare(const_RANDOM) == 0)
                    {
                        replace = wxString::Format("%d", rand() & 0xffff);
                    }
                    else
                    {
                        if (!Macros.empty())
                        {
                            MacrosMap::const_iterator it;

                            if ((it = Macros.find(var)) != Macros.end())
                            {
                                replace = it->second;
                            }
                        }
                    }
            }

            const wxChar l = search.Last(); // make non-braced variables work

            if (l == '/' || l == '\\' || l == '$' || l == ' ')
            {
                replace.append(l);
            }

            if (replace.IsEmpty())
            {
                wxGetEnv(var, &replace);
            }

            if (replace.IsSameAs("\"\"") || replace.IsSameAs("''") || replace.empty())
            {
                buffer.Replace(search, "", false);
            }
            else
            {
                buffer.Replace(search, replace, false);
            }
        }

        while (m_RE_DOS.Matches(buffer))
        {
            replace.Empty();
            search = m_RE_DOS.GetMatch(buffer, 2);
            wxString var = m_RE_DOS.GetMatch(buffer, 3).Upper();

            if (var.GetChar(0) == '#')
            {
                replace = UnixFilename(pUsrVarMan->Replace(var), wxPATH_UNIX);
            }
            else
            {
                if (var.compare(const_COIN) == 0)
                {
                    replace.assign(1u, (rand() & 1) ? '1' : '0');
                }
                else
                    if (var.compare(const_RANDOM) == 0)
                    {
                        replace = wxString::Format("%d", rand() & 0xffff);
                    }
                    else
                    {
                        if (!Macros.empty())
                        {
                            MacrosMap::const_iterator it;

                            if ((it = Macros.find(var)) != Macros.end())
                            {
                                replace = it->second;
                            }
                        }
                    }
            }

            if (replace.IsEmpty())
            {
                wxGetEnv(var, &replace);
            }

            if (replace.IsSameAs("\"\"") || replace.IsSameAs("''") || replace.empty())
            {
                buffer.Replace(search, "", false);
            }
            else
            {
                buffer.Replace(search, replace, false);
            }
        }
    }

    buffer.Replace("%%", "%");
    buffer.Replace("$$", "$");
}

void CMakeListsExporter::ConvertMacros(wxString & buffer)
{
    if (buffer.IsEmpty())
    {
        return;
    }

    if (buffer.IsSameAs("\"\"") || buffer.IsSameAs("''"))
    {
        buffer.Clear();
        return;
    }

    static const wxString delim("$%[");

    if (buffer.find_first_of(delim) != wxString::npos)
    {
        // See macromanager.cpp MacrosManager::ReplaceMacros function for where this code was taken from
        static const wxString const_COIN("COIN");
        static const wxString const_RANDOM("RANDOM");
        static const wxString toNativePath("$TO_NATIVE_PATH{");
        static const wxString toUnixPath("$TO_UNIX_PATH{");
        static const wxString toWindowsPath("$TO_WINDOWS_PATH{");
        wxString search;
        wxString replace;
        // UserVariableManager* pUsrVarMan = Manager::Get()->GetUserVariableManager();
        MacrosManager * macroMan = Manager::Get()->GetMacrosManager();
        const MacrosMap & Macros = macroMan->GetMacros();

        while (m_RE_Unix.Matches(buffer))
        {
            replace.Empty();
            search = m_RE_Unix.GetMatch(buffer, 2);
            wxString var = m_RE_Unix.GetMatch(buffer, 3).Upper();

            if (var.GetChar(0) == '#')
            {
                replace = var.Upper();

                if (replace.Find('.') == wxNOT_FOUND)
                {
                    replace.Append(".BASE");
                }

                replace.Prepend("${GCV_");
                replace.Replace("#", "");
                replace.Replace(".", "_");
                replace.Append("}");
            }
            else
            {
                if (var.compare(const_COIN) == 0)
                {
                    replace.assign(1u, (rand() & 1) ? '1' : '0');
                }
                else
                    if (var.compare(const_RANDOM) == 0)
                    {
                        replace = wxString::Format("%d", rand() & 0xffff);
                    }
                    else
                    {
                        if (!Macros.empty())
                        {
                            MacrosMap::const_iterator it;

                            if ((it = Macros.find(var)) != Macros.end())
                            {
                                replace = it->second;
                            }
                        }
                    }
            }

            const wxChar l = search.Last(); // make non-braced variables work

            if (l == '/' || l == '\\' || l == '$' || l == ' ')
            {
                replace.append(l);
            }

            if (replace.IsEmpty() && search.StartsWith("$("))
            {
                replace = var.Upper();

                if (replace.Find('.') == wxNOT_FOUND)
                {
                    replace.Append(".BASE");
                }

                replace.Prepend("${GCV_");
                replace.Replace("#", "");
                replace.Replace("(", "");
                replace.Replace(")", "");
                replace.Replace(".", "_");
                replace.Append("}");
            }

            if (!replace.IsEmpty())
            {
                if (replace.IsSameAs("\"\"") || replace.IsSameAs("''") || replace.empty())
                {
                    buffer.Replace(search, "", false);
                }
                else
                {
                    buffer.Replace(search, replace, false);
                }
            }
        }

        while (m_RE_DOS.Matches(buffer))
        {
            replace.Empty();
            search = m_RE_DOS.GetMatch(buffer, 2);
            wxString var = m_RE_DOS.GetMatch(buffer, 3).Upper();

            if (var.GetChar(0) == '#')
            {
                replace = var.Upper();

                if (replace.Find('.') == wxNOT_FOUND)
                {
                    replace.Append(".BASE");
                }

                replace.Prepend("${GCV_");
                replace.Replace("#", "");
                replace.Replace(".", "_");
                replace.Append("}");
            }
            else
            {
                if (var.compare(const_COIN) == 0)
                {
                    replace.assign(1u, (rand() & 1) ? '1' : '0');
                }
                else
                    if (var.compare(const_RANDOM) == 0)
                    {
                        replace = wxString::Format("%d", rand() & 0xffff);
                    }
                    else
                    {
                        if (!Macros.empty())
                        {
                            MacrosMap::const_iterator it;

                            if ((it = Macros.find(var)) != Macros.end())
                            {
                                replace = it->second;
                            }
                        }
                    }
            }

            if (!replace.IsEmpty())
            {
                if (replace.IsSameAs("\"\"") || replace.IsSameAs("''") || replace.empty())
                {
                    buffer.Replace(search, "", false);
                }
                else
                {
                    buffer.Replace(search, replace, false);
                }
            }
        }
    }

    buffer.Replace("%%", "%");
    buffer.Replace("$$", "$");
    buffer.Replace("\\", "/");
}

void CMakeListsExporter::ExportGlobalVariableSets(ExportMode eMode)
{
    ConfigManager * pCfgMan = Manager::Get()->GetConfigManager("gcv");

    if (pCfgMan)
    {
        const wxString defSet(pCfgMan->Read("/active"));

        if (eMode == ExportMode::GVS_EXPORT_DEFAULT_ONLY)
        {
            m_ContentCMakeListGlobalVariables.append(wxString::Format("# Global Variables for \"%s\" set:%s", defSet, EOL));
        }
        else
        {
            m_ContentCMakeListGlobalVariables.append(wxString::Format("# Global Variables. The default set is: \"%s\"%s", defSet, EOL));
        }

        m_ContentCMakeListGlobalVariables.append(EOL);
        wxString value;
        const wxString cSets("/sets/");
        wxArrayString sets = pCfgMan->EnumerateSubPaths(cSets);
        sets.Sort();

        for (const wxString & sCurrentSet : sets)
        {
            wxString  contentCurrentSetBASE = wxEmptyString;
            wxString  contentCurrentSetOSBITS = wxEmptyString;
            wxString  contentCurrentSet = wxEmptyString;
            wxArrayString vars = pCfgMan->EnumerateSubPaths(cSets + sCurrentSet + "/");
            vars.Sort();

            for (const wxString & sCurrentVar : vars)
            {
                wxString path(cSets + sCurrentSet + '/' + sCurrentVar + '/');
                wxArrayString knownMembers = pCfgMan->EnumerateKeys(path);

                if (defSet.IsSameAs(sCurrentSet, false))
                {
                    if (eMode != ExportMode::GVS_EXPORT_NON_DEFAULT)
                    {
                        for (unsigned int i = 0; i < knownMembers.GetCount(); ++i)
                        {
                            value = pCfgMan->Read(path + knownMembers[i]);
                            ConvertMacros(value);

                            if (knownMembers[i].Upper().IsSameAs("BASE"))
                            {
                                contentCurrentSetBASE.append(wxString::Format("set(GCV_%s_%s \"%s\")%s", sCurrentVar.Upper(), knownMembers[i].Upper(), value, EOL));
                            }
                            else
                            {
                                if (knownMembers[i].Upper().IsSameAs("OSBITS"))
                                {
                                    contentCurrentSetOSBITS.append(wxString::Format("set(GCV_%s_%s \"%s\")%s", sCurrentVar.Upper(), knownMembers[i].Upper(), value, EOL));
                                }
                                else
                                {
                                    contentCurrentSet.append(wxString::Format("set(GCV_%s_%s \"%s\")%s", sCurrentVar.Upper(), knownMembers[i].Upper(), value, EOL));
                                }
                            }
                        }

                        if (eMode == ExportMode::GVS_EXPORT_ALL)
                        {
                            contentCurrentSet.append(EOL);
                        }
                    }
                }
                else
                {
                    if (eMode != ExportMode::GVS_EXPORT_DEFAULT_ONLY)
                    {
                        for (unsigned int i = 0; i < knownMembers.GetCount(); ++i)
                        {
                            value = pCfgMan->Read(path + knownMembers[i]);
                            ConvertMacros(value);

                            if (knownMembers[i].Upper().IsSameAs("BASE"))
                            {
                                contentCurrentSetBASE.append(wxString::Format("# %s.%s.%s: %s%s", sCurrentSet, sCurrentVar, knownMembers[i], value, EOL));
                            }
                            else
                            {
                                if (knownMembers[i].Upper().IsSameAs("OSBITS"))
                                {
                                    contentCurrentSetOSBITS.append(wxString::Format("# %s.%s.%s: %s%s", sCurrentSet, sCurrentVar, knownMembers[i], value, EOL));
                                }
                                else
                                {
                                    contentCurrentSet.append(wxString::Format("# %s.%s.%s: %s%s", sCurrentSet, sCurrentVar, knownMembers[i], value, EOL));
                                }
                            }
                        }

                        contentCurrentSet.append(EOL);
                    }
                }
            }

            if (!contentCurrentSetOSBITS.IsEmpty())
            {
                m_ContentCMakeListGlobalVariables.append(contentCurrentSetOSBITS);
            }

            if (!contentCurrentSetBASE.IsEmpty())
            {
                m_ContentCMakeListGlobalVariables.append(contentCurrentSetBASE);
            }

            m_ContentCMakeListGlobalVariables.append(contentCurrentSet);
            m_ContentCMakeListGlobalVariables.append(EOL);
        }
    }
    else
    {
        m_ContentCMakeListGlobalVariables.append(wxString::Format("# No Global Variables found!%s", EOL));
    }

    m_ContentCMakeListGlobalVariables.append(EOL);
}

wxString CMakeListsExporter::ExportMacros(ProjectBuildTarget * buildTarget)
{
    wxString sMacroContentTarget = wxEmptyString;
    MacrosManager * macroMan = Manager::Get()->GetMacrosManager();

    if (macroMan)
    {
        const cbProject * project = buildTarget
                                    ? buildTarget->GetParentProject()
                                    : Manager::Get()->GetProjectManager()->GetActiveProject();
        Manager::Get()->GetMacrosManager()->RecalcVars(project, nullptr, buildTarget);
        const MacrosMap & Macros = macroMan->GetMacros();

        if (Macros.empty())
        {
            sMacroContentTarget.append(wxString::Format("# No Macros found!%s", EOL));
        }
        else
        {
            sMacroContentTarget.append(wxString::Format("# Macros:%s", EOL));
            // MacrosMap uses a hash as key, to get sorted macros we need to copy them to a non-hashed map
            std::map <wxString, wxString> NewMap;

            for (MacrosMap::const_iterator it = Macros.begin(); it != Macros.end(); ++it)
            {
                NewMap[it->first] = it->second;
            }

            for (decltype(NewMap)::value_type & Item : NewMap)
            {
                sMacroContentTarget.append(wxString::Format("#        %s: %s%s", Item.first,  Item.second, EOL));
            }
        }
    }

    sMacroContentTarget.append(EOL);
    return (sMacroContentTarget);
}

void CMakeListsExporter::ExportGlobalVariables()
{
    m_ContentCMakeListGlobalVariables  = wxEmptyString;
    m_sGlobalVariableFileName = wxEmptyString;
    cbWorkspace * pWorkspace = Manager::Get()->GetProjectManager()->GetWorkspace();

    if (pWorkspace)
    {
        FileManager * fileMgr = Manager::Get()->GetFileManager();
        LogManager * logMgr = Manager::Get()->GetLogManager();
        // ====================================================================================
        // Global variables - GVS_EXPORT_DEFAULT_ONLY
        ExportGlobalVariableSets(ExportMode::GVS_EXPORT_DEFAULT_ONLY);
        // ====================================================================================
        m_ContentCMakeListGlobalVariables.append(wxString::Format("# -----------------------------------------------------------------------------%s", EOL));
        m_ContentCMakeListGlobalVariables.append(EOL);
        // ====================================================================================
        // Global variables - GVS_EXPORT_NON_DEFAULT
        ExportGlobalVariableSets(ExportMode::GVS_EXPORT_NON_DEFAULT);
        // ====================================================================================
        // Save the global variables to a file
        wxString wxsFileName = wxEmptyString;
        wxString wxsWorkspaceTitle = pWorkspace->GetTitle();
        const wxString sGlobalVariableFileName(ValidateFilename(wxString::Format("CMakeLists_GlobalVariables_%s.txt", wxsWorkspaceTitle)));

        if (pWorkspace->IsOK())
        {
            wxFileName wxfWorkspaceFileName(pWorkspace->GetFilename());
            wxsFileName = ValidateFilename(wxString::Format("%s%s", wxfWorkspaceFileName.GetPathWithSep(), sGlobalVariableFileName));
        }
        else
        {
            cbProject * project = Manager::Get()->GetProjectManager()->GetProjects()->Item(0);

            if (project)
            {
                wxFileName wxfProjectFileName(project->GetFilename());
                wxsFileName = ValidateFilename(wxString::Format("%s%s", wxfProjectFileName.GetPathWithSep(), sGlobalVariableFileName));
            }
        }

        if (!wxsFileName.IsEmpty())
        {
            fileMgr->Save(wxsFileName, m_ContentCMakeListGlobalVariables, wxFONTENCODING_SYSTEM, true, true);
            logMgr->DebugLog(wxString::Format("Exported file: %s", wxsFileName));
            m_sGlobalVariableFileName = wxsFileName.Clone();
        }
        else
        {
            logMgr->DebugLogError(wxString::Format("Could not export the global variables!!!"));
        }
    }

    m_ContentCMakeListGlobalVariables = wxEmptyString;
}

wxString CMakeListsExporter::ValidateFilename(const wxString & iFileName)
{
    wxFileName fnFileName(iFileName);
    wxString sFN = fnFileName.GetName();

    for (size_t i = 0; i < sFN.Length(); i++)
    {
        if (!wxIsalnum(sFN[i]))
        {
            sFN[i] = '_';
        }
    }

    fnFileName.SetName(sFN);
    return fnFileName.GetFullPath();
}

wxString CMakeListsExporter::GetHumanReadableOptionRelation(ProjectBuildTarget * buildTarget, OptionsRelationType type)
{
    wxString result = wxEmptyString;
    OptionsRelation relation = buildTarget->GetOptionRelation(type);

    switch (relation)
    {
        case orUseParentOptionsOnly:
            result = wxString::Format("Use parent options only. Type %d", (int)relation);
            break;

        case orUseTargetOptionsOnly:
            result = wxString::Format("Use target options only. Type %d", (int)relation);
            break;

        case orPrependToParentOptions:
            result = wxString::Format("Use target options first then parent options. Type %d", (int)relation);
            break;

        case orAppendToParentOptions:
            result = wxString::Format("Uses parent options first then target options. Type %d", (int)relation);
            break;

        default:
            result = wxString::Format("Unknown option for type: %d", (int)relation);
            break;
    }

    return result;
}

wxString CMakeListsExporter::GetTargetRootDirectory(ProjectBuildTarget * buildTarget)
{
    wxFileName wxfTargetFileName;
    FilesList & filesList = buildTarget->GetFilesList();

    for (FilesList::iterator it = filesList.begin(); it != filesList.end(); it++)
    {
        ProjectFile * pf = *it;
        wxString filename = pf->file.GetFullPath();
        FileType fileType = FileTypeOf(filename);

        if (pf->compile && (fileType == ftSource))
        {
            wxFileName wxFN(filename);

            if (!wxfTargetFileName.Exists() || (wxFN.GetDirCount() < wxfTargetFileName.GetDirCount()))
            {
                wxfTargetFileName = wxFN;
            }
        }
    }

    return wxfTargetFileName.GetFullPath();
}

void CMakeListsExporter::RunExport()
{
    ExportGlobalVariables();

    if (m_sGlobalVariableFileName.IsEmpty())
    {
        return;
    }

    m_ContentCMakeListTopLevel.Clear();
    m_ContentCMakeListTarget.Clear();
    FileManager * fileMgr = Manager::Get()->GetFileManager();
    LogManager * logMgr = Manager::Get()->GetLogManager();
    int iProjectCountSaved = 0;
    wxString tmpStringA, tmpStringB, tmpStringC;
    wxArrayString tmpArrayA, tmpArrayB, tmpArrayC;
    bool bModuleLibrary;
    ProjectManager * prjManager = Manager::Get()->GetProjectManager();
    ProjectsArray * prjArr = prjManager->GetProjects();
    m_mapTargetOuputs.clear();

    for (unsigned int i = 0; i < prjArr->GetCount(); ++i)
    {
        cbProject * project = prjArr->Item(i);

        if (!project)
        {
            continue;
        }

        wxString tgtStr(project->GetActiveBuildTarget());

        if (tgtStr.IsEmpty())
        {
            tgtStr = project->GetFirstValidBuildTargetName();    // last-chance default
        }

        wxArrayString tmpArrayVBT = project->GetExpandedVirtualBuildTargetGroup(tgtStr);
        unsigned int icountVBT = tmpArrayVBT.GetCount();
        unsigned int icountPRJ = tmpArrayVBT.GetCount();

        if (icountPRJ == 0)
        {
            icountPRJ = project->GetBuildTargetsCount();
        }

        ProjectBuildTarget * buildTarget;

        for (unsigned int i = 0; i < icountPRJ; i++)
        {
            if (icountVBT == 0)
            {
                buildTarget = project->GetBuildTarget(i);
            }
            else
            {
                buildTarget = project->GetBuildTarget(tmpArrayVBT[i]);
            }

            if (!buildTarget)
            {
                continue;
            }

            wxString targetOutPutName = RemLib(wxFileName(buildTarget->GetOutputFilename()).GetName());
            m_mapTargetOuputs.insert(std::pair<wxString, wxString>(targetOutPutName, targetOutPutName));
        }
    }

    for (unsigned int i = 0; i < prjArr->GetCount(); ++i)
    {
        cbProject * project = prjArr->Item(i);

        if (!project)
        {
            continue;
        }

        wxString projectTitle = project->GetTitle();
        wxString projectTopLevelPath = UnixFilename(project->GetCommonTopLevelPath(), wxPATH_UNIX);
        wxString tgtStr(project->GetActiveBuildTarget());

        if (tgtStr.IsEmpty())
        {
            tgtStr = project->GetFirstValidBuildTargetName();    // last-chance default
        }

        wxArrayString tmpArrayVBT = project->GetExpandedVirtualBuildTargetGroup(tgtStr);
        unsigned int icountVBT = tmpArrayVBT.GetCount();
        unsigned int icountPRJ = tmpArrayVBT.GetCount();

        if (icountPRJ == 0)
        {
            icountPRJ = project->GetBuildTargetsCount();
        }

        ProjectBuildTarget * buildTarget;

        for (unsigned int i = 0; i < icountPRJ; i++)
        {
            bModuleLibrary = false;

            if (icountVBT == 0)
            {
                buildTarget = project->GetBuildTarget(i);
            }
            else
            {
                buildTarget = project->GetBuildTarget(tmpArrayVBT[i]);
            }

            if (!buildTarget)
            {
                continue;
            }


            wxString targetTitle = ValidateFilename(buildTarget->GetTitle());
            wxString targetOutPutName = RemLib(wxFileName(buildTarget->GetOutputFilename()).GetName());
            wxString prjBasepath = project->GetBasePath();

            m_ContentCMakeListTarget.Clear();
            m_ContentCMakeListTarget.append(wxString::Format("cmake_minimum_required(VERSION %s) %s", CMAKE_MIN_VERSION_REQUIRED, EOL));
            m_ContentCMakeListTarget.append(EOL);
            m_ContentCMakeListTarget.append(wxString::Format("project(\"%s\")%s", targetTitle, EOL));
            m_ContentCMakeListTarget.append(wxString::Format("set(PROJECT_OUTPUTNAME \"%s\")%s", targetOutPutName, EOL));
            m_ContentCMakeListTarget.append(wxString::Format("# Target Title: %s%s", targetTitle, EOL));
            m_ContentCMakeListTarget.append(wxString::Format("# Target Output name : %s%s",  targetOutPutName, EOL));
            m_ContentCMakeListTarget.append(wxString::Format("# Target Output full path: %s%s",  buildTarget->GetOutputFilename(), EOL));
            m_ContentCMakeListTarget.append(wxString::Format("# Project Compiler: %s%s", project->GetCompilerID(), EOL));
            m_ContentCMakeListTarget.append(wxString::Format("# Target Compiler:  %s%s", buildTarget->GetCompilerID(), EOL));
            m_ContentCMakeListTarget.append(wxString::Format("# Project CommonTopLevelPath:  %s%s", project->GetCommonTopLevelPath(), EOL));

            // ====================================================================================
            // Target Output Type
            switch (buildTarget->GetTargetType())
            {
                case ttExecutable:
                    m_ContentCMakeListTarget.append(wxString::Format("# Target type: ttExecutable%s", EOL));
                    break;

                case ttConsoleOnly:
                    m_ContentCMakeListTarget.append(wxString::Format("# Target type: ttConsoleOnly%s", EOL));
                    break;

                case ttStaticLib:
                    m_ContentCMakeListTarget.append(wxString::Format("# Target type: ttStaticLib%s", EOL));
                    break;

                case ttDynamicLib:
                    //if (buildTarget->GetCreateStaticLib() || buildTarget->GetCreateDefFile())
                    //{
                    m_ContentCMakeListTarget.append(wxString::Format("# Target type: ttDynamicLib - DLL%s", EOL));
                    //}
                    //else
                    //{
                    //    m_ContentCMakeListTarget.append(wxString::Format("# Target type: ttDynamicLib - module%s",  EOL));
                    //}
                    break;

                default:
                    m_ContentCMakeListTarget.append(wxString::Format("# Target type: unrecognized target type%s", EOL));
                    logMgr->LogError("Warning: \"" + targetTitle + "\" is of an unrecognized target type; skipping...");
                    break;
            }

            wxString sTargetRootDir = GetTargetRootDirectory(buildTarget);
            m_ContentCMakeListTarget.append(wxString::Format("# Target detected root directory:  %s%s", sTargetRootDir, EOL));
            m_ContentCMakeListTarget.append(wxString::Format("# Target Options:%s", EOL));
            m_ContentCMakeListTarget.append(wxString::Format("#                 projectCompilerOptionsRelation: %s%s",     GetHumanReadableOptionRelation(buildTarget, ortCompilerOptions), EOL));
            m_ContentCMakeListTarget.append(wxString::Format("#                 projectLinkerOptionsRelation: %s%s",       GetHumanReadableOptionRelation(buildTarget, ortLinkerOptions), EOL));
            m_ContentCMakeListTarget.append(wxString::Format("#                 projectIncludeDirsRelation: %s%s",         GetHumanReadableOptionRelation(buildTarget, ortIncludeDirs), EOL));
            m_ContentCMakeListTarget.append(wxString::Format("#                 projectLibDirsRelation: %s%s",             GetHumanReadableOptionRelation(buildTarget, ortLibDirs), EOL));
            m_ContentCMakeListTarget.append(wxString::Format("#                 projectResourceIncludeDirsRelation: %s%s", GetHumanReadableOptionRelation(buildTarget, ortResDirs), EOL));
            m_ContentCMakeListTarget.append(EOL);
            // ====================================================================================
            m_ContentCMakeListTarget.append(wxString::Format("# -----------------------------------------------------------------------------%s", EOL));
            m_ContentCMakeListTarget.append(EOL);
            // ====================================================================================
            m_ContentCMakeListTarget.append(wxString::Format("# Include global variable definition file:%s", EOL));
            m_ContentCMakeListTarget.append(wxString::Format("include(%s)%s", UnixFilename(m_sGlobalVariableFileName, wxPATH_UNIX), EOL));
            m_ContentCMakeListTarget.append(EOL);
            // ====================================================================================
            m_ContentCMakeListTarget.append(wxString::Format("# -----------------------------------------------------------------------------%s", EOL));
            m_ContentCMakeListTarget.append(EOL);
            // ====================================================================================
            // output target macro file
            wxFileName wxfTargetMacroFileName(sTargetRootDir);

            if (wxfTargetMacroFileName.DirExists())
            {
                wxfTargetMacroFileName.SetFullName(wxString::Format("CMakeLists_Macros_%s.txt", targetTitle));
                wxString wsFullMacroFileName(wxfTargetMacroFileName.GetFullPath());

                if (wxfTargetMacroFileName.FileExists())
                {
                    logMgr->DebugLogError(wxString::Format("Deleting file: %s!!!", wsFullMacroFileName));
                    ::wxRemoveFile(wxfTargetMacroFileName.GetFullPath());

                    if (wxfTargetMacroFileName.FileExists())
                    {
                        logMgr->DebugLogError(wxString::Format("File still exists: %s!!!", wsFullMacroFileName));
                    }
                }

                if (!wxfTargetMacroFileName.FileExists())
                {
                    wxString sMacroData = wxEmptyString;
                    sMacroData.append(wxString::Format("cmake_minimum_required(VERSION %s) %s", CMAKE_MIN_VERSION_REQUIRED, EOL));
                    sMacroData.append(EOL);
                    sMacroData.append(ExportMacros(buildTarget));
                    fileMgr->Save(wsFullMacroFileName, sMacroData, wxFONTENCODING_SYSTEM, true, true);
                    logMgr->DebugLog(wxString::Format("Exported file: %s", wsFullMacroFileName));
                    m_ContentCMakeListTarget.append(wxString::Format("# -----------------------------------------------------------------------------%s", EOL));
                    m_ContentCMakeListTarget.append(wxString::Format("# Include target macro definition file:%s", EOL));
                    m_ContentCMakeListTarget.append(wxString::Format("include(%s)%s", UnixFilename(wsFullMacroFileName, wxPATH_UNIX), EOL));
                }
            }
            else
            {
                logMgr->DebugLogError(wxString::Format("Could not save %s as the directory does not exist!!!", wxfTargetMacroFileName.GetFullPath()));
            }

            // ====================================================================================
            m_ContentCMakeListTarget.append(wxString::Format("# -----------------------------------------------------------------------------%s", EOL));
            m_ContentCMakeListTarget.append(EOL);
            // ====================================================================================
            m_ContentCMakeListTarget.append(wxString::Format("# Include CMakePrintHelpers module:%s", EOL));
            m_ContentCMakeListTarget.append(wxString::Format("include(CMakePrintHelpers)%s", EOL));
            m_ContentCMakeListTarget.append(EOL);
            // ====================================================================================
            m_ContentCMakeListTarget.append(wxString::Format("# -----------------------------------------------------------------------------%s", EOL));
            m_ContentCMakeListTarget.append(EOL);

            // ====================================================================================
            // Compiler search directories
            wxArrayString prjSearchDirs = project->GetIncludeDirs();
            for (unsigned int i = 0; i < prjSearchDirs.GetCount(); i++)
            {
                if (!::wxIsAbsolutePath(prjSearchDirs[i]))
                {
                    tmpStringA = prjSearchDirs[i];
                    Manager::Get()->GetMacrosManager()->ReplaceMacros(tmpStringA);
                    if (tmpStringA.IsSameAs(prjSearchDirs[i]))
                    {
                        prjSearchDirs[i]  = wxString::Format("%s%s", prjBasepath, prjSearchDirs[i]);
                    }
                    else
                    {
                        if (wxDir::Exists(tmpStringA) && !::wxIsAbsolutePath(tmpStringA))
                        {
                            prjSearchDirs[i]  = wxString::Format("%s%s", prjBasepath, prjSearchDirs[i]);
                        }
                    }
                }
            }

            wxArrayString tgtSearchDirs = buildTarget->GetIncludeDirs();
            for (unsigned int i = 0; i < tgtSearchDirs.GetCount(); i++)
            {
                if (!::wxIsAbsolutePath(tgtSearchDirs[i]))
                {
                    tmpStringA = tgtSearchDirs[i];
                    Manager::Get()->GetMacrosManager()->ReplaceMacros(tmpStringA);
                    if (tmpStringA.IsSameAs(tgtSearchDirs[i]))
                    {
                        tgtSearchDirs[i]  = wxString::Format("%s%s", prjBasepath, tgtSearchDirs[i]);
                    }
                    else
                    {
                        if (wxDir::Exists(tmpStringA) && !::wxIsAbsolutePath(tmpStringA))
                        {
                            tgtSearchDirs[i]  = wxString::Format("%s%s", prjBasepath, tgtSearchDirs[i]);
                        }
                    }
                }
            }

            tmpArrayA = AppendOptionsArray(prjSearchDirs, tgtSearchDirs, buildTarget->GetOptionRelation(ortIncludeDirs));
            prjSearchDirs.Clear();
            tmpStringA.Clear();

            for (unsigned int j = 0; j < tmpArrayA.GetCount(); j++)
            {
                tmpStringA += wxString::Format("\"%s\"%s                    ", tmpArrayA[j], EOL);
            }

            if (!tmpStringA.IsEmpty())
            {
                m_ContentCMakeListTarget.append(wxString::Format("# Compiler Include paths:%s", EOL));
                ConvertMacros(tmpStringA);
                m_ContentCMakeListTarget.append(wxString::Format("include_directories(%s)%s", tmpStringA, EOL));
                m_ContentCMakeListTarget.append(EOL);
            }

            // ====================================================================================
            // Compiler options
            tmpArrayA = AppendOptionsArray(project->GetCompilerOptions(), buildTarget->GetCompilerOptions(), buildTarget->GetOptionRelation(ortCompilerOptions));
            tmpStringA.Clear();
            tmpStringB.Clear();

            for (unsigned int j = 0; j < tmpArrayA.GetCount(); j++)
            {
                if ((tmpArrayA[j].Left(2) == "-D") || (tmpArrayA[j].Left(2) == "/D"))
                {
                    wxString tmpStringCVT = tmpArrayA[j].Clone();
                    ConvertMacros(tmpStringCVT);
                    tmpStringB += wxString::Format("add_definitions(%s)%s", tmpStringCVT, EOL);
                }
                else
                {
                    wxString tmpStringCVT = tmpArrayA[j].Clone();
                    ConvertMacros(tmpStringCVT);
                    tmpStringA += wxString::Format("set(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} %s\")%s", tmpStringCVT, EOL);
                }
            }

            if (!tmpStringA.IsEmpty())
            {
                m_ContentCMakeListTarget.append(wxString::Format("# Compiler flags:%s", EOL));
                m_ContentCMakeListTarget.append(wxString::Format("%s%s", tmpStringA, EOL));
            }

            if (!tmpStringB.IsEmpty())
            {
                m_ContentCMakeListTarget.append(wxString::Format("# Compiler Definitions:%s", EOL));
                m_ContentCMakeListTarget.append(wxString::Format("%s%s", tmpStringB, EOL));
            }

            // ====================================================================================
            m_ContentCMakeListTarget.append(wxString::Format("# -----------------------------------------------------------------------------%s", EOL));
            m_ContentCMakeListTarget.append(EOL);
            // ====================================================================================
            // Source code file list
            FilesList & filesList = buildTarget->GetFilesList();
            wxArrayString tmpArraySrc, tmpArrayhdr, tmpArrayRes;

            for (FilesList::iterator j = filesList.begin(); j != filesList.end(); j++)
            {
                ProjectFile * pf = *j;
                wxString cfn(pf->relativeFilename);

                // is header
                if (cfn.Right(2) == ".h" || cfn.Right(4) == ".hpp" || cfn.Right(4) == ".hxx" || cfn.Right(3) == ".hh")
                {
                    tmpArrayhdr.Add(ConvertSlash(cfn));
                }
                else
                    if (cfn.Right(4) == ".cpp" || cfn.Right(2) == ".c" || cfn.Right(3) == ".cc" || cfn.Right(4) == ".cxx")
                    {
                        if (pf->compile)
                        {
                            tmpArraySrc.Add(ConvertSlash(cfn));
                        }
                    }
                    else
                        if (cfn.Right(3) == ".rc")
                        {
                            if (pf->compile)
                            {
                                tmpArrayRes.Add(ConvertSlash(cfn));
                            }
                        }
            }

            if (!tmpArraySrc.IsEmpty())
            {
                tmpArraySrc.Sort();
                m_ContentCMakeListTarget.append(wxString::Format("# Source files to compile:%s", EOL));
                m_ContentCMakeListTarget.append(wxString::Format("FILE(GLOB SOURCE_FILES%s", EOL));

                for (unsigned int j = 0; j < tmpArraySrc.GetCount(); j++)
                {
                    tmpStringA = tmpArraySrc[j];
                    ConvertMacros(tmpStringA);
                    m_ContentCMakeListTarget.append(wxString::Format("            \"%s%s\"%s", projectTopLevelPath, tmpStringA, EOL));
                }

                if (!tmpArrayhdr.IsEmpty())
                {
                    m_ContentCMakeListTarget.append(EOL);
                    tmpArrayhdr.Sort();

                    for (unsigned int j = 0; j < tmpArrayhdr.GetCount(); j++)
                    {
                        tmpStringA = tmpArrayhdr[j];
                        ConvertMacros(tmpStringA);

                        if ((buildTarget->GetTargetType() == ttStaticLib) || (buildTarget->GetTargetType() == ttDynamicLib))
                        {
                            m_ContentCMakeListTarget.append(wxString::Format("            \"%s%s\"%s", projectTopLevelPath, tmpStringA, EOL));
                        }
                        else
                        {
                            m_ContentCMakeListTarget.append(wxString::Format("#             %s%s%s", projectTopLevelPath, tmpStringA, EOL));
                        }
                    }
                }

                m_ContentCMakeListTarget.append(wxString::Format("    )%s", EOL));
                m_ContentCMakeListTarget.append(EOL);
            }

            // ====================================================================================
            // Resource file(s)
#if 0
            BUILDING RESOURCES IS NOT FULLY WORKING, SO EXCLUDE FOR THE TIME BEING
            NEED TO FIX LATER

            if (!tmpArrayRes.IsEmpty())
            {
                tmpArrayRes.Sort();
                tmpStringC.Replace("/", "\\");
                m_ContentCMakeListTarget.append(wxString::Format("if (MINGW)%s", EOL));
                m_ContentCMakeListTarget.append(wxString::Format("    # Windows Resource file:%s", EOL));

                for (unsigned int j = 0; j < tmpArrayRes.GetCount(); j++)
                {
                    tmpStringA = tmpArrayRes[j];
                    ConvertMacros(tmpStringA);
                    m_ContentCMakeListTarget.append(wxString::Format("    set(SOURCE_FILES \"${SOURCE_FILES} %s%s\")%s", projectTopLevelPath, tmpStringA, EOL));
                }

                m_ContentCMakeListTarget.append(wxString::Format("    # Setup initial resource flags:%s", EOL));
                m_ContentCMakeListTarget.append(wxString::Format("    set(CMAKE_RC_FLAGS \"${CMAKE_RC_FLAGS} ${WX_RC_FLAGS}\")%s", EOL));
                wxArrayString tmpArrayRI = AppendOptionsArray(project->GetResourceIncludeDirs(), buildTarget->GetResourceIncludeDirs(), buildTarget->GetOptionRelation(ortResDirs));

                if (tmpArrayRI.GetCount() > 0)
                {
                    wxString tmpStringRI;

                    for (unsigned int j = 0; j < tmpArrayRI.GetCount(); j++)
                    {
                        tmpStringRI += tmpArrayRI[j];
                    }

                    m_ContentCMakeListTarget.append(wxString::Format("    # Update resource flags to include directories:%s", EOL));
                    tmpStringRI.Replace("/", "\\");
                    ConvertMacros(tmpStringRI);
                    m_ContentCMakeListTarget.append(wxString::Format("    set(CMAKE_RC_FLAGS \"${CMAKE_RC_FLAGS} -I %s\")%s", tmpStringRI, EOL));
                }

                m_ContentCMakeListTarget.append(wxString::Format("endif() %s", EOL));
                m_ContentCMakeListTarget.append(EOL);
            }

#endif

            // ====================================================================================
            // Target Output Type with source files
            switch (buildTarget->GetTargetType())
            {
                case ttExecutable:
                    m_ContentCMakeListTarget.append(wxString::Format("# Target type: ttExecutable%s", EOL));
                    m_ContentCMakeListTarget.append(wxString::Format("add_executable(${PROJECT_OUTPUTNAME} ${SOURCE_FILES})%s", EOL));
                    break;

                case ttConsoleOnly:
                    m_ContentCMakeListTarget.append(wxString::Format("# Target type: ttConsoleOnly%s", EOL));
                    m_ContentCMakeListTarget.append(wxString::Format("add_executable(${PROJECT_OUTPUTNAME} ${SOURCE_FILES})%s", EOL));
                    break;

                case ttStaticLib:
                    m_ContentCMakeListTarget.append(wxString::Format("# Target type: ttStaticLib%s", EOL));
                    m_ContentCMakeListTarget.append(wxString::Format("add_library(${PROJECT_OUTPUTNAME} STATIC ${SOURCE_FILES})%s", EOL));
                    break;

                case ttDynamicLib:
                    //if (buildTarget->GetCreateStaticLib() || buildTarget->GetCreateDefFile())
                    //{
                    m_ContentCMakeListTarget.append(wxString::Format("# Target type: ttDynamicLib - DLL%s", EOL));
                    m_ContentCMakeListTarget.append(wxString::Format("add_library(${PROJECT_OUTPUTNAME} SHARED ${SOURCE_FILES})%s", EOL));
                    //}
                    //else
                    //{
                    //    m_ContentCMakeListTarget.append(wxString::Format("# Target type: ttDynamicLib - module%s",  EOL));
                    //    m_ContentCMakeListTarget.append(wxString::Format("add_library(${PROJECT_OUTPUTNAME} MODULE ${SOURCE_FILES})%s", EOL));
                    //    bModuleLibrary = true;
                    //}
                    break;

                default:
                    m_ContentCMakeListTarget.append(wxString::Format("# Target type: unrecognized target type%s", EOL));
                    logMgr->LogError("Warning: The target \"" + targetTitle + "\" has an un-recognized target type; skipping...");
                    break;
            }

            m_ContentCMakeListTarget.append(EOL);
            m_ContentCMakeListTarget.append(wxString::Format("# Set the target output directory:%s", EOL));
            wxString wxsOutputDir = wxFileName(buildTarget->GetOutputFilename()).GetPath();
            ConvertMacros(wxsOutputDir);
            m_ContentCMakeListTarget.append(wxString::Format("set_target_properties(${PROJECT_OUTPUTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY  \"%s%s\")%s", projectTopLevelPath, wxsOutputDir, EOL));
            m_ContentCMakeListTarget.append(wxString::Format("set_target_properties(${PROJECT_OUTPUTNAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY  \"%s%s\")%s", projectTopLevelPath, wxsOutputDir, EOL));
            m_ContentCMakeListTarget.append(wxString::Format("set_target_properties(${PROJECT_OUTPUTNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY  \"%s%s\")%s", projectTopLevelPath, wxsOutputDir, EOL));
            m_ContentCMakeListTarget.append(EOL);
            m_ContentCMakeListTarget.append(wxString::Format("unset(SOURCE_FILES)%s", EOL));
            m_ContentCMakeListTarget.append(EOL);
            // ====================================================================================
            m_ContentCMakeListTarget.append(wxString::Format("# -----------------------------------------------------------------------------%s", EOL));
            m_ContentCMakeListTarget.append(EOL);
            // ====================================================================================
            // Linker options
            tmpArrayA = AppendOptionsArray(project->GetLinkerOptions(), buildTarget->GetLinkerOptions(), buildTarget->GetOptionRelation(ortLinkerOptions));

            if (!tmpArrayA.IsEmpty())
            {
                m_ContentCMakeListTarget.append(wxString::Format("# Linker options:%s", EOL));
                m_ContentCMakeListTarget.append(wxString::Format("set(LINKER_OPTIONS_LIST)%s", EOL));

                for (unsigned int j = 0; j < tmpArrayA.GetCount(); j++)
                {
                    tmpStringA = tmpArrayA[j].Clone();
                    ConvertMacros(tmpStringA);
                    m_ContentCMakeListTarget.append(wxString::Format("list(APPEND LINKER_OPTIONS_LIST %s)%s", tmpStringA, EOL));
                }

                m_ContentCMakeListTarget.append(wxString::Format("target_link_options(${PROJECT_OUTPUTNAME} PRIVATE ${LINKER_OPTIONS_LIST})%s", EOL));
                m_ContentCMakeListTarget.append(wxString::Format("unset(LINKER_OPTIONS_LIST)%s", EOL));
                m_ContentCMakeListTarget.append(EOL);
            }

            // ====================================================================================
            // Linker libraries
            if (bModuleLibrary == false)
            {
                tmpArrayA = AppendOptionsArray(project->GetLinkLibs(), buildTarget->GetLinkLibs(), buildTarget->GetOptionRelation(ortLinkerOptions));

                if (!tmpArrayA.IsEmpty())
                {
                    m_ContentCMakeListTarget.append(wxString::Format("# Linker libraries to include:%s", EOL));
                    m_ContentCMakeListTarget.append(wxString::Format("set(LINKER_LIBRARIES_LIST)%s", EOL));

                    for (unsigned int j = 0; j < tmpArrayA.GetCount(); j++)
                    {
                        tmpStringA = tmpArrayA[j].Clone();
                        ConvertMacros(tmpStringA);
                        m_ContentCMakeListTarget.append(wxString::Format("list(APPEND LINKER_LIBRARIES_LIST %s)%s", tmpStringA, EOL));
                    }

                    ConvertMacros(tmpStringA);
                    m_ContentCMakeListTarget.append(wxString::Format("target_link_libraries(${PROJECT_OUTPUTNAME} PRIVATE ${LINKER_LIBRARIES_LIST})%s", EOL));
                    m_ContentCMakeListTarget.append(wxString::Format("unset(LINKER_LIBRARIES_LIST)%s", EOL));
                    m_ContentCMakeListTarget.append(EOL);
                }
            }

            // ====================================================================================
            // Linker search directories
            wxArrayString prjLibDirs = project->GetLibDirs();
            for (unsigned int i = 0; i < prjLibDirs.GetCount(); i++)
            {
                if (!::wxIsAbsolutePath(prjLibDirs[i]))
                {
                    tmpStringA = prjLibDirs[i];
                    Manager::Get()->GetMacrosManager()->ReplaceMacros(tmpStringA);
                    if (tmpStringA.IsSameAs(prjLibDirs[i]))
                    {
                        prjLibDirs[i]  = wxString::Format("%s%s", prjBasepath, prjLibDirs[i]);
                    }
                    else
                    {
                        if (wxDir::Exists(tmpStringA) && !::wxIsAbsolutePath(tmpStringA))
                        {
                            prjLibDirs[i]  = wxString::Format("%s%s", prjBasepath, prjLibDirs[i]);
                        }
                    }
                }
            }


            wxArrayString tgtLibDirs  = buildTarget->GetLibDirs();
            for (unsigned int i = 0; i < tgtLibDirs.GetCount(); i++)
            {
                if (!::wxIsAbsolutePath(tgtLibDirs[i]))
                {
                    tmpStringA = tgtLibDirs[i];
                    Manager::Get()->GetMacrosManager()->ReplaceMacros(tmpStringA);
                    if (tmpStringA.IsSameAs(tgtLibDirs[i]))
                    {
                        tgtLibDirs[i]  = wxString::Format("%s%s", prjBasepath, tgtLibDirs[i]);
                    }
                    else
                    {
                        if (wxDir::Exists(tmpStringA) && !::wxIsAbsolutePath(tmpStringA))
                        {
                            tgtLibDirs[i]  = wxString::Format("%s%s", prjBasepath, tgtLibDirs[i]);
                        }
                    }
                }
            }


            tmpArrayA = AppendOptionsArray(prjLibDirs, tgtLibDirs, buildTarget->GetOptionRelation(ortLibDirs));

            if (!tmpArrayA.IsEmpty())
            {
                m_ContentCMakeListTarget.append(wxString::Format("# Linker search paths:%s", EOL));
                m_ContentCMakeListTarget.append(wxString::Format("set(LINKER_DIR_LIST)%s", EOL));

                for (unsigned int j = 0; j < tmpArrayA.GetCount(); j++)
                {
                    tmpStringA = tmpArrayA[j].Clone();
                    ConvertMacros(tmpStringA);
                    m_ContentCMakeListTarget.append(wxString::Format("list(APPEND LINKER_DIR_LIST %s)%s", tmpStringA, EOL));
                }

                m_ContentCMakeListTarget.append(wxString::Format("target_link_directories(${PROJECT_OUTPUTNAME} PUBLIC ${LINKER_DIR_LIST})%s", EOL));
                m_ContentCMakeListTarget.append(wxString::Format("unset(LINKER_DIR_LIST)%s", EOL));
                m_ContentCMakeListTarget.append(EOL);
            }

            // ====================================================================================
            m_ContentCMakeListTarget.append(wxString::Format("# -----------------------------------------------------------------------------%s", EOL));
            m_ContentCMakeListTarget.append(EOL);
            // ====================================================================================
            // Before build commands
            tmpArrayA = buildTarget->GetCommandsBeforeBuild();

            if (!tmpArrayA.IsEmpty())
            {
                m_ContentCMakeListTarget.append(wxString::Format("# Target before commands:%s", EOL));

                for (unsigned int j = 0; j < tmpArrayA.GetCount(); j++)
                {
                    tmpStringA = tmpArrayA[j];
                    //ConvertMacros(tmpStringA);
                    m_ContentCMakeListTarget.append(wxString::Format("# %s%s", tmpStringA, EOL));
                }
            }

            tmpArrayA = project->GetCommandsBeforeBuild();

            if (!tmpArrayA.IsEmpty())
            {
                m_ContentCMakeListTarget.append(wxString::Format("# Project before commands:%s", EOL));

                for (unsigned int j = 0; j < tmpArrayA.GetCount(); j++)
                {
                    tmpStringA = tmpArrayA[j];
                    //ConvertMacros(tmpStringA);
                    m_ContentCMakeListTarget.append(wxString::Format("# %s%s", tmpStringA, EOL));
                }

                m_ContentCMakeListTarget.append(EOL);
            }

            // ====================================================================================
            // After build commands
            tmpArrayA = buildTarget->GetCommandsAfterBuild();

            if (!tmpArrayA.IsEmpty())
            {
                m_ContentCMakeListTarget.append(wxString::Format("# Target after commands:%s", EOL));

                for (unsigned int j = 0; j < tmpArrayA.GetCount(); j++)
                {
                    tmpStringA = tmpArrayA[j];
                    //ConvertMacros(tmpStringA);
                    m_ContentCMakeListTarget.append(wxString::Format("# %s%s", tmpStringA, EOL));
                }

                m_ContentCMakeListTarget.append(EOL);
            }

            tmpArrayA = project->GetCommandsAfterBuild();

            if (!tmpArrayA.IsEmpty())
            {
                m_ContentCMakeListTarget.append(wxString::Format("# Project after commands:%s", EOL));

                for (unsigned int j = 0; j < tmpArrayA.GetCount(); j++)
                {
                    tmpStringA = tmpArrayA[j];
                    //ConvertMacros(tmpStringA);
                    m_ContentCMakeListTarget.append(wxString::Format("# %s%s", tmpStringA, EOL));
                }

                m_ContentCMakeListTarget.append(EOL);
            }

            m_ContentCMakeListTarget.append(wxString::Format("unset(PROJECT_OUTPUTNAME)%s", EOL));
            // ====================================================================================
            m_ContentCMakeListTarget.append(wxString::Format("# -----------------------------------------------------------------------------%s", EOL));
            m_ContentCMakeListTarget.append(EOL);
            // ====================================================================================
            m_ContentCMakeListTarget.append(wxString::Format("# -----------------------------------------------------------------------------%s", EOL));
            m_ContentCMakeListTarget.append(EOL);
            // ====================================================================================
            // TESTING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            // TESTING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#if 0
            // calculate project/workspace dependencies
            wxArrayString tlist;
            wxArrayInt deps;
            CalculateProjectDependencies(project, deps);

            // loop all projects in the dependencies list
            for (size_t i = 0; i < deps.GetCount(); ++i)
            {
                cbProject * prj = Manager::Get()->GetProjectManager()->GetProjects()->Item(deps[i]);

                if (!prj->SupportsCurrentPlatform())
                {
                    wxString msg = wxString::Format("\"%s\" does not support the current platform. Skipping...", prj->GetTitle());
                    logMgr->LogWarning(msg);
                    continue;
                }

                ExpandTargets(prj, targetName, tlist);

                if (tlist.GetCount() == 0)
                {
                    wxString msg = wxString::Format(msg.Printf("Warning: No target named '%s' in project '%s'. Project will not be built...", targetName, prj->GetTitle());
                                                    logMgr->LogWarning(msg);
                }

                               // add all matching targets in the job list
                               for (size_t x = 0; x < tlist.GetCount(); ++x)
                {
                    ProjectBuildTarget * tgt = prj->GetBuildTarget(tlist[x]);

                    if (!tgt->SupportsCurrentPlatform())
                    {
                        wxString msg = wxString::Format("\"%s - %s\" does not support the current platform. Skipping...", prj->GetTitle(), tlist[x]);
                        logMgr->LogWarning(msg);
                        continue;
                    }

                    Compiler * compiler = CompilerFactory::GetCompiler(tgt->GetCompilerID());

                    if (compiler && !compiler->IsValid())
                    {
                        wxString msg = wxString::Format("The target has an invalid compiler \"%s\". Skipping...", compiler->GetName());
                        logMgr->LogWarning(msg);
                        continue;
                    }

                    wxString msg = wxString::Format("Job: %s - %s %s", prj->GetTitle(), tlist[x], prj->GetBuildTarget(tlist[x])->GetTitle());
                    logMgr->Log(msg);
                }
            }

#endif
            // TESTING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            // TESTING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            // ========================================================================================================================================================================
            // ========================================================================================================================================================================
            // ========================================================================================================================================================================
            //output target file
            wxFileName wxfTargetFileName(sTargetRootDir);

            if (wxfTargetFileName.DirExists())
            {
                //FAILS: wxfTargetFileName.SetFullName(wxString::Format("CMakeLists_%s.txt", targetTitle));
                wxfTargetFileName.SetFullName("CMakeLists.txt");
                wxString wsFullFileName(wxfTargetFileName.GetFullPath());

                if (wxfTargetFileName.FileExists())
                {
                    logMgr->DebugLogError(wxString::Format("Deleting file: %s!!!", wsFullFileName));
                    ::wxRemoveFile(wxfTargetFileName.GetFullPath());

                    if (wxfTargetFileName.FileExists())
                    {
                        logMgr->DebugLogError(wxString::Format("File still exists: %s!!!", wsFullFileName));
                    }
                }

                if (!wxfTargetFileName.FileExists())
                {
                    fileMgr->Save(wsFullFileName, m_ContentCMakeListTarget, wxFONTENCODING_SYSTEM, true, true);
                    logMgr->DebugLog(wxString::Format("Exported file: %s", wsFullFileName));
                    iProjectCountSaved++;
                    m_ContentCMakeListTopLevel.append(wxString::Format("# -----------------------------------------------------------------------------%s", EOL));
                    m_ContentCMakeListTopLevel.append(wxString::Format("# CBP: %s , Target: %s  %s", project->GetFilename(), targetTitle, EOL));
                    wxString tmpString(wxfTargetFileName.GetPath());
                    ConvertMacros(tmpString);
                    m_ContentCMakeListTopLevel.append(wxString::Format("add_subdirectory(\"%s\")%s", tmpString, EOL));
                }
            }
            else
            {
                logMgr->DebugLogError(wxString::Format("Could not save %s as the directory does not exist!!!", wxfTargetFileName.GetFullPath()));
            }

            // ========================================================================================================================================================================
            // ========================================================================================================================================================================
            // ========================================================================================================================================================================
        }

        m_ContentCMakeListTopLevel.append(wxString::Format("# -----------------------------------------------------------------------------%s", EOL));
    }

    if (iProjectCountSaved > 1)
    {
        cbWorkspace * pWorkspace = Manager::Get()->GetProjectManager()->GetWorkspace();

        if (pWorkspace)
        {
            wxString sCMakeListTopLevel;
            sCMakeListTopLevel.append(wxString::Format("cmake_minimum_required(VERSION %s) %s", CMAKE_MIN_VERSION_REQUIRED, EOL));
            wxString Title = pWorkspace->GetTitle();
            // If only have one project then hopefully it was not loaded via a workspace
            ProjectsArray * prjArr = Manager::Get()->GetProjectManager()->GetProjects();

            if (prjArr->GetCount() == 1)
            {
                cbProject * project = prjArr->Item(0);

                if (project)
                {
                    Title = project->GetTitle();
                }
            }

#if 0
            // DEBUGGING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            sCMakeListTopLevel.append(wxString::Format("Project prjArr count: %lld%s", prjArr->GetCount(), EOL));

            for (unsigned int i = 0; i < prjArr->GetCount(); ++i)
            {
                cbProject * project = prjArr->Item(i);

                if (project)
                {
                    unsigned int icountVBT = project->GetExpandedVirtualBuildTargetGroup(project->GetActiveBuildTarget()).GetCount();
                    sCMakeListTopLevel.append(wxString::Format("%d %s - alias: %s , targets: %d , virtual targets: %d %s",
                                                               i,
                                                               project->GetTitle(),
                                                               project->GetActiveBuildTarget(),
                                                               project->GetBuildTargetsCount(),
                                                               icountVBT,
                                                               EOL));
                }
                else
                {
                    sCMakeListTopLevel.append(wxString::Format("%d no project!!%s", i, EOL));
                }
            }

            // DEBUGGING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#endif
            wxString wxsWorkspaceTitle = ValidateFilename(Title);
            sCMakeListTopLevel.append("#################################################################################################################################################################");
            sCMakeListTopLevel.append(EOL);
            sCMakeListTopLevel.append("##                                                                                                                                                              #");
            sCMakeListTopLevel.append(EOL);
            sCMakeListTopLevel.append("## CMake top level file                                                                                                                                         #");
            sCMakeListTopLevel.append(EOL);
            sCMakeListTopLevel.append("## Typical usage will be (build in release mode):                                                                                                               #");
            sCMakeListTopLevel.append(EOL);
            sCMakeListTopLevel.append("##                                                                                                                                                              #");
            sCMakeListTopLevel.append(EOL);
            sCMakeListTopLevel.append("## > mkdir build                                                                                                                                                #");
            sCMakeListTopLevel.append(EOL);
            sCMakeListTopLevel.append("## > cd build                                                                                                                                                   #");
            sCMakeListTopLevel.append(EOL);
            sCMakeListTopLevel.append("## > cmake -G \"Unix Makefile\" ..                                                                                                                              #");
            sCMakeListTopLevel.append(EOL);
            sCMakeListTopLevel.append("## > make -jN                                                                                                                                                   #");
            sCMakeListTopLevel.append(EOL);
            sCMakeListTopLevel.append("##                                                                                                                                                              #");
            sCMakeListTopLevel.append(EOL);
            sCMakeListTopLevel.append("#################################################################################################################################################################");
            sCMakeListTopLevel.append(EOL);
            sCMakeListTopLevel.append(EOL);
            sCMakeListTopLevel.append("if (NOT CMAKE_VERSION VERSION_LESS 3.1) # THIS MUST STAY AT THE TOP OF THE FILE");
            sCMakeListTopLevel.append(EOL);
            sCMakeListTopLevel.append("    cmake_policy(VERSION 3.1) # Doing this prevents multiple, very verbose warnings about policy CMP0053 not being set");
            sCMakeListTopLevel.append(EOL);
            sCMakeListTopLevel.append("endif()");
            sCMakeListTopLevel.append(EOL);
            sCMakeListTopLevel.append(EOL);
            sCMakeListTopLevel.append("#############################################");
            sCMakeListTopLevel.append(EOL);
            sCMakeListTopLevel.append("## Name top level project");
            sCMakeListTopLevel.append(EOL);
            sCMakeListTopLevel.append("#############################################");
            sCMakeListTopLevel.append(EOL);
            sCMakeListTopLevel.append(wxString::Format("project(\"%s\")", wxsWorkspaceTitle));
            sCMakeListTopLevel.append(EOL);
            sCMakeListTopLevel.append(EOL);
            sCMakeListTopLevel = wxString::Format("%s%s%s", sCMakeListTopLevel, EOL, m_ContentCMakeListTopLevel);
            // ====================================================================================
            // Save the top level CMakeLists file
            wxString wxsFileName = wxEmptyString;
            const wxString sGlobalVariableFileName("CMakeLists.txt");
            cbWorkspace * pWorkspace = Manager::Get()->GetProjectManager()->GetWorkspace();

            if (pWorkspace && pWorkspace->IsOK())
            {
                wxFileName wxfWorkspaceFileName(pWorkspace->GetFilename());
                wxsFileName = ValidateFilename(wxString::Format("%s%s", wxfWorkspaceFileName.GetPathWithSep(), sGlobalVariableFileName));
            }
            else
            {
                cbProject * project = Manager::Get()->GetProjectManager()->GetProjects()->Item(0);

                if (project)
                {
                    wxFileName wxfProjectFileName(project->GetFilename());
                    wxsFileName = ValidateFilename(wxString::Format("%s%s", wxfProjectFileName.GetPathWithSep(), sGlobalVariableFileName));
                }
            }

            if (!wxsFileName.IsEmpty())
            {
                fileMgr->Save(wxsFileName, sCMakeListTopLevel, wxFONTENCODING_SYSTEM, true, true);
                logMgr->DebugLog(wxString::Format("Exported file: %s", wxsFileName));
                m_sGlobalVariableFileName = wxsFileName.Clone();
            }
            else
            {
                logMgr->DebugLogError(wxString::Format("Could not export the global variables!!!"));
            }
        }
    }

    m_ContentCMakeListTarget.Clear();
    m_ContentCMakeListTopLevel.Clear();
}
