/* ============================================================
* Videoner plugin for QupZilla
* Copyright (C) 2012-2013  David Rosca <nowrep@gmail.com>
* Copyright (C) 2012-2013  Mladen Pejaković <pejakm@gmail.com>
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
#ifndef VIDEONER_HANDLER_H
#define VIDEONER_HANDLER_H

#include <QObject>
#include <QWebElement>
#include <QMessageBox>
#include <QProcess>
#include <QRegExp>
#include <QWebHitTestResult>
#include <QMenu>
#include <QPointer>

class QMouseEvent;
class QWebElement;
class QLabel;
class WebView;
class Videoner_Handler : public QObject
{
    Q_OBJECT
public:
    explicit Videoner_Handler(const QString &sPath, QObject* parent = 0);

    QString settingsFile();
    void loadSettings();
    void populateWebViewMenu(QMenu* menu, WebView* view, const QWebHitTestResult &r);

signals:

public slots:

private slots:
    void startExternalHandler();

private:
    QPointer<WebView> m_view;

    bool m_pageyt;
    bool m_pagevm;
    bool m_pagedm;
    bool m_pagehu;
    bool m_pagemc;
    QString m_extExe;
    QString m_extArgs;
    QString m_settingsFile;
};

#endif // VIDEONER_HANDLER_H
