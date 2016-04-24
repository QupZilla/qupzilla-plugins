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
#ifndef READABILITYPLUGIN_H
#define READABILITYPLUGIN_H

#include "plugininterface.h"
#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPointer>

class ReadabilityPlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
    Q_PLUGIN_METADATA(IID "QupZilla.Browser.plugin.ReadabilityPlugin")

public:
    explicit ReadabilityPlugin();
    PluginSpec pluginSpec();

    void init(InitState state, const QString &settingsPath);
    void unload();
    bool testPlugin();

    QTranslator* getTranslator(const QString &locale);
    void populateWebViewMenu(QMenu* menu, WebView* view, const WebHitTestResult &r);

private slots:
    void makeReadability();

private:
    QString m_settingsPath;
    WebView* m_view;
};

#endif // READABILITYPLUGIN_H
