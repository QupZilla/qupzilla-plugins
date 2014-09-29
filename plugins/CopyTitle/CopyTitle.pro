TARGET = $$qtLibraryTarget(CopyTitle)
# OS/2 allows only 8 chars in TARGET
os2: TARGET = CopTitle

SOURCES += copytitle.cpp
HEADERS += copytitle.h
RESOURCES += copytitle.qrc

TRANSLATIONS = \
    translations/ar_SA.ts \
    translations/bg_BG.ts \
    translations/ca_ES.ts \
    translations/de_DE.ts \
    translations/es_ES.ts \
    translations/es_MX.ts \
    translations/eu_ES.ts \
    translations/fi_FI.ts \
    translations/fr_FR.ts \
    translations/he_IL.ts \
    translations/hr_HR.ts \
    translations/it_IT.ts \
    translations/ja_JP.ts \
    translations/lv_LV.ts \
    translations/nl_NL.ts \
    translations/pl_PL.ts \
    translations/pt_PT.ts \
    translations/ru_RU.ts \
    translations/sk_SK.ts \
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
