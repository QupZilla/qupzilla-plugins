TARGET = $$qtLibraryTarget(MailHandle)
os2: TARGET  = MailHandle

SOURCES = \
    mailhandle_plugin.cpp \
    mailhandle_handler.cpp \
    mailhandle_settings.cpp \
    mailhandle_schemehandler.cpp

HEADERS = \
    mailhandle_plugin.h \
    mailhandle_handler.h \
    mailhandle_settings.h \
    mailhandle_schemehandler.h

FORMS += \
    mailhandle_settings.ui

RESOURCES = mailhandle_res.qrc

TRANSLATIONS = \
    translations/sr_BA.ts \
    translations/sr_BA@latin.ts \
    translations/sr_RS.ts \
    translations/sr_RS@latin.ts

srcdir = $$(QUPZILLA_SRCDIR)
equals(srcdir, "") {
    include(../../plugins.pri)
}
else {
    include($$srcdir/src/plugins.pri)
}
