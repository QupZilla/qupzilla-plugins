/* ============================================================
* YouTuber plugin for QupZilla
* Copyright (C) 2012  David Rosca <nowrep@gmail.com>
* Copyright (C) 2012  Mladen Pejaković <pejakm@gmail.com>
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
#include "youtuber_plugin.h"
#include "youtuber_handler.h"
#include "youtuber_settings.h"
#include "mainapplication.h"
#include "pluginproxy.h"
#include "qupzilla.h"

#include <QTranslator>

YouTuber_Plugin::YouTuber_Plugin()
    : QObject()
    , m_handler(0)
{
}

PluginSpec YouTuber_Plugin::pluginSpec()
{
    PluginSpec spec;
    spec.name = "YouTuber";
    spec.info = "YouTube video link handler";
    spec.description = "Open YouTube videos in external program";
    spec.version = "0.3.2";
    spec.author = QString::fromUtf8("Mladen Pejaković <pejakm@gmail.com>");
    spec.icon = QPixmap(":/youtuber/data/youtube.png");
    spec.hasSettings = true;

    return spec;
}

void YouTuber_Plugin::init(const QString &sPath)
{
    m_handler = new YouTuber_Handler(sPath, this);
}

void YouTuber_Plugin::unload()
{
    delete m_settings.data();
    m_handler->deleteLater();
}

bool YouTuber_Plugin::testPlugin()
{
    // Let's be sure, require latest version of QupZilla

    return (QupZilla::VERSION == QLatin1String("1.3.5"));
}

QTranslator* YouTuber_Plugin::getTranslator(const QString &locale)
{
    QTranslator* translator = new QTranslator();
    translator->load(locale, ":/youtuber/locale/");
    return translator;
}

void YouTuber_Plugin::showSettings(QWidget* parent)
{
    if (!m_settings) {
        m_settings = new YouTuber_Settings(m_handler, parent);
    }

    m_settings.data()->show();
    m_settings.data()->raise();
}

void YouTuber_Plugin::populateWebViewMenu(QMenu* menu, WebView* view, const QWebHitTestResult &r)
{
    m_handler->populateWebViewMenu(menu, view, r);
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(YouTuber, YouTuber_Plugin)
#endif