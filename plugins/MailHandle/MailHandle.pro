TARGET = $$qtLibraryTarget(MailHandle)
os2: TARGET  = MailHandle

SOURCES = \
    mailhandle_plugin.cpp \
    mailhandle_settings.cpp \
    mailhandle_schemehandler.cpp

HEADERS = \
    mailhandle_plugin.h \
    mailhandle_settings.h \
    mailhandle_schemehandler.h

FORMS += \
    mailhandle_settings.ui

RESOURCES = mailhandle_res.qrc

TRANSLATIONS = \
    translations/ar_SA.ts \
    translations/sr.ts \
    translations/sr@ijekavian.ts \
    translations/sr@ijekavianlatin.ts \
    translations/sr@latin.ts

PLUGIN_DIR = $$PWD
srcdir = $$(QUPZILLA_SRCDIR)
equals(srcdir, "") {
    include(../../plugins.pri)
}
else {
    include($$srcdir/src/plugins.pri)
}
