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
    translations/ar_SA.ts \
    translations/ca_ES.ts \
    translations/cs_CZ.ts \
    translations/de_DE.ts \
    translations/el_GR.ts \
    translations/es_ES.ts \
    translations/es_MX.ts \
    translations/es_VE.ts \
    translations/eu_ES.ts \
    translations/fa_IR.ts \
    translations/fi_FI.ts \
    translations/fr_FR.ts \
    translations/gl_ES.ts \
    translations/he_IL.ts \
    translations/hr_HR.ts \
    translations/id_ID.ts \
    translations/it_IT.ts \
    translations/ja_JP.ts \
    translations/lv_LV.ts \
    translations/nl_NL.ts \
    translations/nqo.ts \
    translations/pl_PL.ts \
    translations/pt_PT.ts \
    translations/ru_RU.ts \
    translations/sk_SK.ts \
    translations/sr.ts \
    translations/sr@ijekavian.ts \
    translations/sr@ijekavianlatin.ts \
    translations/sr@latin.ts \
    translations/tr_TR.ts \
    translations/uk_UA.ts \
    translations/uz@Cyrl.ts \
    translations/uz@Latn.ts \
    translations/zh_TW.ts \

PLUGIN_DIR = $$PWD
srcdir = $$(QUPZILLA_SRCDIR)
equals(srcdir, "") {
    include(../../plugins.pri)
}
else {
    include($$srcdir/src/plugins.pri)
}
