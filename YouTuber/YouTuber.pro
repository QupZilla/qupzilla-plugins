TARGET = YouTuber
os2: TARGET  = YouTuber

SOURCES = \
    youtuber_plugin.cpp \
    youtuber_handler.cpp \
    youtuber_settings.cpp

HEADERS = \
    youtuber_plugin.h \
    youtuber_handler.h \
    youtuber_settings.h

FORMS += \
    youtuber_settings.ui

RESOURCES = youtuber_res.qrc

TRANSLATIONS = \
    translations/cs_CZ.ts \
    translations/de_DE.ts \
    translations/it_IT.ts \
    translations/ja_JP.ts \
    translations/sr_BA.ts \
    translations/sr_RS.ts \

include(../../plugins.pri)
