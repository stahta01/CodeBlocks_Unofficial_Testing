.rc.lo:
	$(LIBTOOL) --tag=RC --mode=compile $(WX_RESCOMP) $< -o $@

AM_CPPFLAGS = \
			$(WX_CXXFLAGS) \
			-I$(top_srcdir)/src/include \
			-I$(top_srcdir)/src/sdk/wxscintilla/include

if CODEBLOCKS_NT_BUILD_INSTALLER
pluginlibdir = $(pkgdatadir)/plugins
else
pluginlibdir = $(pkglibdir)/plugins
endif

pkgdata_DATA = \
			RegExTestbed.zip

pluginlib_LTLIBRARIES = \
			libRegExTestbed.la

libRegExTestbed_la_LDFLAGS = \
			@MODULE_SHARED_LDFLAGS@ \
			-no-undefined \
			-avoid-version

libRegExTestbed_la_LIBADD = \
			$(top_srcdir)/src/sdk/libcodeblocks.la $(WX_LIBS)

libRegExTestbed_la_SOURCES = \
			regexdlg.cpp RegExTestbed.cpp

if CODEBLOCKS_NT
libRegExTestbed_la_SOURCES += \
			manifest.rc
endif

noinst_HEADERS = \
			regexdlg.h RegExTestbed.h

EXTRA_DIST = \
			*.cbp \
			regexdlg.xrc \
			manifest.xml \
			wxsmith/RegExDlg.wxs

CLEANFILES = \
			$(pkgdata_DATA)

RegExTestbed.zip: manifest.xml regexdlg.xrc
	PWD=`pwd` cd $(srcdir) && zip $(PWD)/RegExTestbed.zip manifest.xml regexdlg.xrc > /dev/null
