# Disabled for QtWebEngine
!QTWEBENGINE_DISABLED {
    TEMPLATE = subdirs
} else {
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
        translations/bg_BG.ts \
        translations/ca_ES.ts \
        translations/cs_CZ.ts \
        translations/da_DK.ts \
        translations/de_DE.ts \
        translations/el_GR.ts \
        translations/es_ES.ts \
        translations/es_MX.ts \
        translations/eu_ES.ts \
        translations/fr_FR.ts \
        translations/hr_HR.ts \
        translations/id_ID.ts \
        translations/is.ts \
        translations/it_IT.ts \
        translations/ja_JP.ts \
        translations/lt.ts \
        translations/lv_LV.ts \
        translations/nl_NL.ts \
        translations/pl_PL.ts \
        translations/pt_PT.ts \
        translations/ro_RO.ts \
        translations/ru_RU.ts \
        translations/sk_SK.ts \
        translations/sr.ts \
        translations/sr@ijekavian.ts \
        translations/sr@ijekavianlatin.ts \
        translations/sr@latin.ts \
        translations/tr_TR.ts \
        translations/uk_UA.ts \
        translations/zh_CN.ts \
        translations/zh_HK.ts \
        translations/zh_TW.ts \

    PLUGIN_DIR = $$PWD
    srcdir = $$(QUPZILLA_SRCDIR)
    equals(srcdir, "") {
        include(../../plugins.pri)
    }
    else {
        include($$srcdir/src/plugins.pri)
    }
}
