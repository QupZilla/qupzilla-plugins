TARGET = $$qtLibraryTarget(Videoner)
os2: TARGET  = Videoner

SOURCES = \
    videoner_plugin.cpp \
    videoner_handler.cpp \
    videoner_settings.cpp

HEADERS = \
    videoner_plugin.h \
    videoner_handler.h \
    videoner_settings.h

FORMS += \
    videoner_settings.ui

RESOURCES = videoner_res.qrc

TRANSLATIONS = \
    translations/ar.ts \
    translations/cs_CZ.ts \
    translations/de_DE.ts \
    translations/it_IT.ts \
    translations/ja_JP.ts \
    translations/pt_PT.ts \
    translations/sr_BA.ts \
    translations/sr_BA@latin.ts \
    translations/sr_RS.ts \
    translations/sr_RS@latin.ts \
    translations/zh_TW.ts

srcdir = $$(QUPZILLA_SRCDIR)
equals(srcdir, "") {
    include(../../plugins.pri)
}
else {
    include($$srcdir/src/plugins.pri)
}
