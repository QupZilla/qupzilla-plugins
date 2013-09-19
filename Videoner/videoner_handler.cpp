/* ============================================================
* Videoner plugin for QupZilla
* Copyright (C) 2012-2013  David Rosca <nowrep@gmail.com>
* Copyright (C) 2012-2013  Mladen Pejaković <pejakm@autistici.org>
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
#include "videoner_handler.h"
#include "webview.h"

#include <QApplication>
#include <QSettings>
#include <QWebPage>
#include <QWebFrame>
#include <QLabel>
#include <QToolTip>

Videoner_Handler::Videoner_Handler(const QString &sPath, QObject* parent)
    : QObject(parent)
    , m_settingsFile(sPath + "extensions.ini")
{
    loadSettings();
}

QString Videoner_Handler::settingsFile()
{
    return m_settingsFile;
}

void Videoner_Handler::loadSettings()
{
    QSettings settings(m_settingsFile, QSettings::IniFormat);

    settings.beginGroup("Videoner");
    m_extExe = settings.value("videonerExtApp", QString()).toString();
    m_extArgs = settings.value("videonerExtArgs", QString()).toString();
    m_pageyt = settings.value("enableYouTube", true).toBool();
    m_pagevm = settings.value("enableVimeo", true).toBool();
    m_pagedm = settings.value("enableDailyMotion", false).toBool();
    m_pagehu = settings.value("enableHulu", false).toBool();
    m_pagemc = settings.value("enableMetaCafe", false).toBool();
    settings.endGroup();
}

void Videoner_Handler::populateWebViewMenu(QMenu* menu, WebView* view, const QWebHitTestResult &r)
{
    m_view = view;
    if (m_pageyt) {
        QRegExp rx1("v=([^&]+)|youtu.be/([^&]+)|y2u.be/([^&]+)|youtube.com/embed/([^&]+)");
        rx1.indexIn(r.linkUrl().toString());

        QString videoId1;
        for (int i = 1; i < 4; ++i) {
            if (!rx1.cap(i).isEmpty()) {
                videoId1 = rx1.cap(i);
                break;
            }
        }

        if (videoId1.isEmpty()) {
            rx1.indexIn(view->url().toString());
            for (int i = 1; i < 4; ++i) {
                if (!rx1.cap(i).isEmpty()) {
                    videoId1 = rx1.cap(i);
                    break;
                }
            }
        }
        if (!videoId1.isEmpty()) {
            QString videoPage1;
            videoPage1 = "http://www.youtube.com/watch?v=" + videoId1;
            menu->addAction(QIcon(":/videoner/data/videoner.png"), tr("Videonize!"), this, SLOT(startExternalHandler()))->setData(videoPage1);
        }
    }

    if (m_pagevm) {
        QRegExp rx2("vimeo.com/([^d]{8})");
        rx2.indexIn(r.linkUrl().toString());

        QString videoId2;
        for (int i = 1; i < 4; ++i) {
            if (!rx2.cap(i).isEmpty()) {
                videoId2 = rx2.cap(i);
                break;
            }
        }

        if (videoId2.isEmpty()) {
            rx2.indexIn(view->url().toString());
            for (int i = 1; i < 4; ++i) {
                if (!rx2.cap(i).isEmpty()) {
                    videoId2 = rx2.cap(i);
                    break;
                }
            }
        }
        if (!videoId2.isEmpty()) {
            QString videoPage2;
            videoPage2 = "http://vimeo.com/" + videoId2;
            menu->addAction(QIcon(":/videoner/data/videoner.png"), tr("Videonize!"), this, SLOT(startExternalHandler()))->setData(videoPage2);
        }
    }
    if (m_pagedm) {
        QRegExp rx3("dailymotion.com/video/([^d]{6}_[^&]+)");
        rx3.indexIn(r.linkUrl().toString());

        QString videoId3;
        for (int i = 1; i < 4; ++i) {
            if (!rx3.cap(i).isEmpty()) {
                videoId3 = rx3.cap(i);
                break;
            }
        }

        if (videoId3.isEmpty()) {
            rx3.indexIn(view->url().toString());
            for (int i = 1; i < 4; ++i) {
                if (!rx3.cap(i).isEmpty()) {
                    videoId3 = rx3.cap(i);
                    break;
                }
            }
        }
        if (!videoId3.isEmpty()) {
            QString videoPage3;
            videoPage3 = "http://www.dailymotion.com/video/" + videoId3;
            menu->addAction(QIcon(":/videoner/data/videoner.png"), tr("Videonize!"), this, SLOT(startExternalHandler()))->setData(videoPage3);
        }
    }
    if (m_pagehu) {
        QRegExp rx4("http://www.hulu.com/watch/([^d]+)");
        rx4.indexIn(r.linkUrl().toString());

        QString videoId4;
        for (int i = 1; i < 4; ++i) {
            if (!rx4.cap(i).isEmpty()) {
                videoId4 = rx4.cap(i);
                break;
            }
        }

        if (videoId4.isEmpty()) {
            rx4.indexIn(view->url().toString());
            for (int i = 1; i < 4; ++i) {
                if (!rx4.cap(i).isEmpty()) {
                    videoId4 = rx4.cap(i);
                    break;
                }
            }
        }
        if (!videoId4.isEmpty()) {
            QString videoPage4;
            videoPage4 = "http://www.hulu.com/watch/" + videoId4;
            menu->addAction(QIcon(":/videoner/data/videoner.png"), tr("Videonize!"), this, SLOT(startExternalHandler()))->setData(videoPage4);
        }
    }
    if (m_pagemc) {
        QRegExp rx5("http://www.metacafe.com/watch/([^d]+)/([^&]+)");
        rx5.indexIn(r.linkUrl().toString());

        QString videoId5;
        for (int i = 1; i < 4; ++i) {
            if (!rx5.cap(i).isEmpty()) {
                videoId5 = rx5.cap(i);
                break;
            }
        }

        if (videoId5.isEmpty()) {
            rx5.indexIn(view->url().toString());
            for (int i = 1; i < 4; ++i) {
                if (!rx5.cap(i).isEmpty()) {
                    videoId5 = rx5.cap(i);
                    break;
                }
            }
        }
        if (!videoId5.isEmpty()) {
            QString videoPage5;
            videoPage5 = "http://www.metacafe.com/watch/" + videoId5;
            menu->addAction(QIcon(":/videoner/data/videoner.png"), tr("Videonize!"), this, SLOT(startExternalHandler()))->setData(videoPage5);
        }
    }
}

void Videoner_Handler::startExternalHandler()
{
    const QUrl &url = qobject_cast<QAction*>(sender())->data().toUrl();
    QStringList arguments = m_extArgs.split(QLatin1Char(' '), QString::SkipEmptyParts);
    arguments << url.toString();

    bool success = QProcess::startDetached(m_extExe, arguments);

    if (!success) {
        QString info = "<ul><li><b>" + tr("Executable: ") + "</b>" + m_extExe + "</li><li><b>" + tr("Arguments: ") + "</b>" + arguments.join(" ") + "</li></ul>";
        QMessageBox::critical(0, tr("Cannot start external viewer"), tr("Cannot start external viewer! %1").arg(info));
    }
}
