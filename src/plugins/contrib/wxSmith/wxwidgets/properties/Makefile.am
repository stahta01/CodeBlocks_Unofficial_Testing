AM_CPPFLAGS = \
			$(WX_CXXFLAGS) \
			-I$(top_srcdir)/src/include \
			-I$(top_srcdir)/src/sdk/wxscintilla/include

if CODEBLOCKS_NT
AM_CPPFLAGS += \
			-DWXUSINGDLL \
			-DwxUSE_UNICODE \
			-D_WIN64 \
			-DWXUSINGDLL \
			-D__WXMSW__ \
			-DBUILDING_PLUGIN \
			-DHAVE_W32API_H \
			-pipe \
			-fmessage-length=0 \
			-fexceptions
endif

noinst_LTLIBRARIES = \
			libwxsmith_wxwidgets_properties.la

libwxsmith_wxwidgets_properties_la_LDFLAGS = \
			@MODULE_SHARED_LDFLAGS@ \
			-no-undefined \
			-avoid-version

if !CODEBLOCKS_NT
libwxsmith_wxwidgets_properties_la_LDFLAGS += \
			-version-info 0:1:0
endif

libwxsmith_wxwidgets_properties_la_LIBADD = \
			$(top_srcdir)/src/sdk/libcodeblocks.la

libwxsmith_wxwidgets_properties_la_SOURCES = \
			./wxspositionsizeproperty.cpp \
			./wxsfonteditordlg.cpp \
			./wxsarraystringcheckeditordlg.cpp \
			./wxscolourproperty.cpp \
			./wxsstyleproperty.cpp \
			./wxsarraystringcheckproperty.cpp \
			./wxsfontproperty.cpp \
			./wxssizerflagsproperty.cpp \
			./wxsfontfaceeditordlg.cpp \
			./wxsbitmapiconeditordlg.cpp \
			./wxsdimensionproperty.cpp \
			./wxsbitmapiconproperty.cpp \
			./wxssimplefonteditordlg.cpp \
			./wxsimagelisteditordlg.cpp \
			./wxsimagelistproperty.cpp \
			./wxsimageproperty.cpp \
			./wxsimagecomboeditordlg.cpp \
			./wxsimagecomboproperty.cpp \
			./wxsimagetreeeditordlg.cpp \
			./wxsimagetreeproperty.cpp

sdkdir = $(includedir)/wxsmith/wxwidgets/properties/

if !CODEBLOCKS_NT_BUILD_INSTALLER
sdk_HEADERS = \
			./wxsdimensionproperty.h \
			./wxsbitmapiconproperty.h \
			./wxsarraystringcheckeditordlg.h \
			./wxsfonteditordlg.h \
			./wxspositionsizeproperty.h \
			./wxsstyleproperty.h \
			./wxscolourproperty.h \
			./wxsarraystringcheckproperty.h \
			./wxsfontproperty.h \
			./wxssizerflagsproperty.h \
			./wxsfontfaceeditordlg.h \
			./wxsbitmapiconeditordlg.h \
			./wxssimplefonteditordlg.h \
			./wxsimagelisteditordlg.h \
			./wxsimagelistproperty.h \
			./wxsimageproperty.h \
			./wxsimagecomboeditordlg.h \
			./wxsimagecomboproperty.h \
			./wxsimagetreeeditordlg.h \
			./wxsimagetreeproperty.h
endif

