/*0*/
// Don't include this header, only configmanager-revision.cpp should do this.
#ifndef AUTOREVISION_H
#define AUTOREVISION_H


#include <wx/string.h>

namespace autorevision
{
const unsigned int svn_revision = 12776;
const wxString svnRevision(_T("12776 EXPERIMENTAL PLUS"));
const wxString svnDate(_T("2022-04-03 00:00:00"));
}

#endif
