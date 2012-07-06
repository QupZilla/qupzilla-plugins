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
#include "youtuber_handler.h"
#include "webview.h"

#include <QApplication>
#include <QSettings>
#include <QWebPage>
#include <QWebFrame>
#include <QLabel>
#include <QToolTip>

YouTuber_Handler::YouTuber_Handler(const QString &sPath, QObject* parent)
    : QObject(parent)
    , m_settingsFile(sPath + "extensions.ini")
{
    loadSettings();
}

QString YouTuber_Handler::settingsFile()
{
    return m_settingsFile;
}

void YouTuber_Handler::loadSettings()
{
    QSettings settings(m_settingsFile, QSettings::IniFormat);

    settings.beginGroup("YouTuber");
    m_extExe = settings.value("Application", "").toString();
    m_extArgs = settings.value("Arguments", "").toString();
    settings.endGroup();
}

void YouTuber_Handler::populateWebViewMenu(QMenu* menu, WebView* view, const QWebHitTestResult &r)
{
    m_view = view;

    QRegExp rx("v=([^&]+)|youtu.be/([^&]+)|y2u.be/([^&]+)");
    rx.indexIn(r.linkUrl().toString());

    QString videoId;
    for (int i = 1; i < 4; ++i) {
        if (!rx.cap(i).isEmpty()) {
            videoId = rx.cap(i);
            break;
        }
    }

    if (videoId.isEmpty()) {
        rx.indexIn(view->url().toString());
        for (int i = 1; i < 4; ++i) {
            if (!rx.cap(i).isEmpty()) {
                videoId = rx.cap(i);
                break;
            }
        }
    }

    if (!videoId.isEmpty()) {
        menu->addAction(QIcon(":/youtuber/data/youtube.png"), tr("View via YouTuber"), this, SLOT(startExternalHandler()))->setData(videoId);
    }
}

void YouTuber_Handler::startExternalHandler()
{
    const QUrl &url = qobject_cast<QAction*>(sender())->data().toUrl();
    QStringList arguments = m_extArgs.split(' ', QString::SkipEmptyParts);
    arguments << url.toString().prepend("http://www.youtube.com/watch?v=");

    bool success = QProcess::startDetached(m_extExe, arguments);

    if (!success) {
        QString info = "<ul><li><b>" + tr("Executable: ") + "</b>" + m_extExe + "</li><li><b>" + tr("Arguments: ") + "</b>" + arguments.join(" ") + "</li></ul>";
        QMessageBox::critical(0, tr("Cannot start external viewer"), tr("Cannot start external viewer! %1").arg(info));
    }
}
