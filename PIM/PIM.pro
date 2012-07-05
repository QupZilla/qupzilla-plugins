TARGET = PIM
os2: TARGET  = PIM

SOURCES = \
    PIM_plugin.cpp \
    PIM_handler.cpp \
    PIM_settings.cpp

HEADERS = \
    PIM_plugin.h \
    PIM_handler.h \
    PIM_settings.h

FORMS += \
    PIM_settings.ui

RESOURCES = PIM_res.qrc

TRANSLATIONS = \
    translations/sr_BA.ts \
    translations/sr_RS.ts \

include(../../plugins.pri)
