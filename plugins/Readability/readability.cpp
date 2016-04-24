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
    : QObject()
    , m_view(0)
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
    spec.icon = QPixmap(":/readability/data/icon.png");
    spec.hasSettings = false;

    return spec;
}

void ReadabilityPlugin::init(InitState state, const QString &settingsPath)
{
    m_settingsPath = settingsPath;
    Q_UNUSED(state)
}

void ReadabilityPlugin::unload()
{
}

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

void ReadabilityPlugin::populateWebViewMenu(QMenu* menu, WebView* view, const WebHitTestResult &r)
{
    m_view = view;
    if (r.imageUrl().isEmpty() && r.linkUrl().isEmpty() && r.mediaUrl().isEmpty() && !r.isContentEditable() && !r.isContentSelected()) {
        menu->addAction(tr("Readability"), this, &ReadabilityPlugin::makeReadability);
    }
}

void ReadabilityPlugin::makeReadability()
{
    QString m_close = QzTools::pixmapToDataUrl(QPixmap(":/readability/data/RM-close.png")).toString();
    QString m_close_hover = QzTools::pixmapToDataUrl(QPixmap(":/readability/data/RM-close-hover.png")).toString();
    QString m_delete = QzTools::pixmapToDataUrl(QPixmap(":/readability/data/RM-Delete-24x24.png")).toString();
    QString m_plus = QzTools::pixmapToDataUrl(QPixmap(":/readability/data/RM-Plus-24x24.png")).toString();
    QString m_minus = QzTools::pixmapToDataUrl(QPixmap(":/readability/data/RM-Minus-24x24.png")).toString();
    QString m_arrow = QzTools::readAllFileContents(":/readability/data/RM-Type-Controls-Arrow.png");
    QString m_controls = QzTools::pixmapToDataUrl(QPixmap(":/readability/data/RM-Type-Controls-24x24.png")).toString();
    
    QString css = QzTools::readAllFileContents(":/readability/data/style.css").arg(m_close, m_close_hover, m_delete, m_plus, m_minus, m_arrow, m_controls).remove(QLatin1Char('\n'));

    QString javascript = QzTools::readAllFileContents(":/readability/data/Readability.js");
    QString toolbar = QzTools::readAllFileContents(":/readability/data/Toolbar.js");
    QString call = QzTools::readAllFileContents(":/readability/data/Call.js").arg(javascript, toolbar, css);

    m_view->page()->runJavaScript(call);
}
