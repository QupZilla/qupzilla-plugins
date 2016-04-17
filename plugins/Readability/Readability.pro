
TARGET = $$qtLibraryTarget(ReadabilityPlugin)

SOURCES += readability.cpp 
HEADERS += readability.h
RESOURCES += readability.qrc


PLUGIN_DIR = $$PWD
srcdir = $$(QUPZILLA_SRCDIR)
equals(srcdir, "") {
    include(../../plugins.pri)
}
else {
    include($$srcdir/src/plugins.pri)
}
