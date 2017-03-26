TARGET = $$qtLibraryTarget(SimpleSessionManager)
# OS/2 allows only 8 chars in TARGET
os2: TARGET = SSM_PLUG

SOURCES += ssm_plugin.cpp
HEADERS += ssm_plugin.h
RESOURCES += ssm_plugin.qrc

PLUGIN_DIR = $$PWD
include(../../plugins.pri)
