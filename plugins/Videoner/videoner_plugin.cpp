/* ============================================================
* Videoner plugin for QupZilla
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
#include "videoner_plugin.h"
#include "videoner_handler.h"
#include "videoner_settings.h"
#include "mainapplication.h"
#include "pluginproxy.h"
#include "browserwindow.h"

#include <QTranslator>

Videoner_Plugin::Videoner_Plugin()
    : QObject()
    , m_handler(0)
{
}

PluginSpec Videoner_Plugin::pluginSpec()
{
    PluginSpec spec;
    spec.name = "Videoner";
    spec.info = "Video links handler";
    spec.description = "Open video links from video sharing services in external program";
    spec.version = "0.2.0";
    spec.author = QString::fromUtf8("Mladen Pejaković <pejakm@autistici.org>");
    spec.icon = QPixmap(":/videoner/data/videoner.png");
    spec.hasSettings = true;

    return spec;
}

void Videoner_Plugin::init(InitState state, const QString &settingsPath)
{
    Q_UNUSED(state)

    m_handler = new Videoner_Handler(settingsPath, this);
}

void Videoner_Plugin::unload()
{
    delete m_settings.data();
    m_handler->deleteLater();
}

bool Videoner_Plugin::testPlugin()
{
    // Require the version that the plugin was built with
    return (Qz::VERSION == QLatin1String(QUPZILLA_VERSION));
}

QTranslator* Videoner_Plugin::getTranslator(const QString &locale)
{
    QTranslator* translator = new QTranslator(this);
    translator->load(locale, ":/videoner/locale/");
    return translator;
}

void Videoner_Plugin::showSettings(QWidget* parent)
{
    if (!m_settings) {
        m_settings = new Videoner_Settings(m_handler, parent);
    }

    m_settings.data()->show();
    m_settings.data()->raise();
}

void Videoner_Plugin::populateWebViewMenu(QMenu* menu, WebView* view, const WebHitTestResult &r)
{
    m_handler->populateWebViewMenu(menu, view, r);
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Videoner, Videoner_Plugin)
#endif
