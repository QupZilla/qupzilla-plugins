/* ============================================================
* MailHandle plugin for QupZilla
* Copyright (C) 2012-2014  David Rosca <nowrep@gmail.com>
* Copyright (C) 2012-2014  Mladen Pejaković <pejakm@autistici.org>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */
#include "mailhandle_plugin.h"
#include "mailhandle_settings.h"
#include "mailhandle_schemehandler.h"
#include "mainapplication.h"
#include "networkmanager.h"
#include "pluginproxy.h"
#include "browserwindow.h"

#include <QTranslator>

MailHandle_Plugin::MailHandle_Plugin()
    : QObject()
    , m_schemehandler(0)
{
}

PluginSpec MailHandle_Plugin::pluginSpec()
{
    PluginSpec spec;
    spec.name = "MailHandle";
    spec.info = "Mailto links handler";
    spec.description = "Makes QupZilla handle mailto links in online services";
    spec.version = "0.1.0";
    spec.author = QString::fromUtf8("Mladen Pejaković <pejakm@autistici.org>");
    spec.icon = QPixmap(":/mailhandle/data/mailhandle.png");
    spec.hasSettings = true;

    return spec;
}

void MailHandle_Plugin::init(InitState state, const QString &settingsPath)
{
    Q_UNUSED(state);
    m_schemehandler = new MailHandle_SchemeHandler(settingsPath);

    QZ_REGISTER_SCHEME_HANDLER("mailto", m_schemehandler);
}

void MailHandle_Plugin::unload()
{
    QZ_UNREGISTER_SCHEME_HANDLER("mailto", m_schemehandler);

    delete m_settings.data();
    delete m_schemehandler;
}

bool MailHandle_Plugin::testPlugin()
{
    // Require the version that the plugin was built with
    return (Qz::VERSION == QLatin1String(QUPZILLA_VERSION));
}

QTranslator* MailHandle_Plugin::getTranslator(const QString &locale)
{
    QTranslator* translator = new QTranslator(this);
    translator->load(locale, ":/mailhandle/locale/");
    return translator;
}

void MailHandle_Plugin::showSettings(QWidget* parent)
{
    if (!m_settings) {
        m_settings = new MailHandle_Settings(m_schemehandler, parent);
    }

    m_settings.data()->show();
    m_settings.data()->raise();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(MailHandle, MailHandle_Plugin)
#endif
