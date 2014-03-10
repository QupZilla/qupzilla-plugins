TARGET = $$qtLibraryTarget(CopyTitle)
# OS/2 allows only 8 chars in TARGET
os2: TARGET = CopTitle

SOURCES += copytitle.cpp
HEADERS += copytitle.h
RESOURCES += copytitle.qrc

TRANSLATIONS = \
    translations/ar_SA.ts \
    translations/sr.ts \
    translations/sr@ijekavian.ts \
    translations/sr@ijekavianlatin.ts \
    translations/sr@latin.ts \
    translations/zh_CN.ts \
    translations/zh_TW.ts

PLUGIN_DIR = $$PWD
srcdir = $$(QUPZILLA_SRCDIR)
equals(srcdir, "") {
    include(../../plugins.pri)
}
else {
    include($$srcdir/src/plugins.pri)
}
