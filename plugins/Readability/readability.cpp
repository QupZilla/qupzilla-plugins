/* ============================================================
* QupZilla - WebKit based browser
* Copyright (C) 2016  Jaroslav Bambas <jar.bambas@gmail.com>
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
#include "readability.h"
#include "browserwindow.h"
#include "pluginproxy.h"
#include "mainapplication.h"
#include "webhittestresult.h"
#include "qztools.h"
#include "webview.h"
#include "webpage.h"

#include <QMenu>
#include <QTranslator>
#include <QPushButton>

ReadabilityPlugin::ReadabilityPlugin()
    : QObject(),
    m_view(0)
{
}

PluginSpec ReadabilityPlugin::pluginSpec()
{
    PluginSpec spec;
    spec.name = "Readability";
    spec.info = "Readability plugin";
    spec.description = "Transform webpage to easy reading.";
    spec.version = "0.1";
    spec.author = "Jaroslav Bambas <jar.bambas@gmail.com>";
    spec.icon = QPixmap(":qupzilla.png");
    spec.hasSettings = false;

    return spec;
}

void ReadabilityPlugin::init(InitState state, const QString &settingsPath)
{
    m_settingsPath = settingsPath;
    Q_UNUSED(state)
}

void ReadabilityPlugin::unload()
{}

bool ReadabilityPlugin::testPlugin()
{
    return (Qz::VERSION == QLatin1String(QUPZILLA_VERSION));
}

QTranslator* ReadabilityPlugin::getTranslator(const QString &locale)
{
    QTranslator* translator = new QTranslator(this);
    translator->load(locale, ":/readability/locale/");
    return translator;
}

void ReadabilityPlugin::showSettings(QWidget* parent)
{
}

void ReadabilityPlugin::populateWebViewMenu(QMenu* menu, WebView* view, const WebHitTestResult &r)
{
    m_view=view;
    if (r.imageUrl().isEmpty() && r.linkUrl().isEmpty() && r.mediaUrl().isEmpty() && !r.isContentEditable() && !r.isContentSelected()) {
        menu->addAction(tr("Readability"), this, &ReadabilityPlugin::makeReadability);
    }
}

void ReadabilityPlugin::makeReadability()
{
    QString javascript = QzTools::readAllFileContents(":/readability/data/Readability.js");
    QString call = QzTools::readAllFileContents(":/readability/data/Call.js").arg(javascript);

    m_view->page()->runJavaScript(call);
}
